#include "stdafx.h"

#define ENABLE_HDR

// General
#include "RendererForward.h"

// Additional
#include "Passes/Technical/ClearRenderTargetPass.h"
#include "Passes/Technical/InvokeFunctionPass.h"
#include "Passes/PostprocessRendering/Postprocess_HDR.h"
#include "Passes/PostprocessRendering/Postprocess_Glow.h"
#include "Passes/PostprocessRendering/Postprocess_Gauss.h"
#include "Passes/PostprocessRendering/Postprocess_AccumTextures.h"
#include "Passes/PostprocessRendering/Postprocess_ApplyTexture.h"

#include "Passes/DebugPass.h"
#include "Passes/ParticlesPass.h"
#include "Passes/DrawBonesPass.h"
#include "Passes/DrawBoundingBoxPass.h"

#include "Passes/SkyboxPass.h"
#include "Passes/ForwardRendering/PassForward_DoRenderScene.h"

#include "Passes/UI/UIFontPass.h"
#include "Passes/UI/UIControlPass.h"


struct __declspec(align(16)) SLightVS
{
	SLight Light;
	glm::vec4 LightPositionVS;
	glm::vec4 LightDirectionVS;
};


CRendererForward::CRendererForward(IBaseManager& BaseManager, IScene& Scene)
	: RendererBase(BaseManager, Scene)
{
	m_LightsBuffer = m_RenderDevice.GetObjectsFactory().CreateStructuredBuffer(nullptr, 1, sizeof(SLightVS), EAccess::CPUWrite);
}

CRendererForward::~CRendererForward()
{
}



void CRendererForward::Initialize(std::shared_ptr<IRenderTarget> OutputRenderTarget)
{
#ifdef ENABLE_HDR
	auto HDRRenderTarget = CreateHDRRenderTarget(OutputRenderTarget);
#endif

	m_SceneCreateTypelessListPass = MakeShared(CSceneCreateTypelessListPass, m_RenderDevice, m_Scene);

	auto skyboxPass = MakeShared(CSkyboxPass, m_RenderDevice);
	skyboxPass->ConfigurePipeline(HDRRenderTarget);

	m_MaterialModelPass = MakeShared(CPassForward_DoRenderScene, m_RenderDevice, m_Scene);
	m_MaterialModelPass->ConfigurePipeline(
#ifdef ENABLE_HDR
		HDRRenderTarget
#else
		OutputRenderTarget
#endif
	);
	m_MaterialModelPass->SetEnviorementTexture(skyboxPass->GetSkyboxCubeTexture());

	//m_MaterialModelPassInstanced = MakeShared(CPassForward_DoRenderSceneInstanced, m_RenderDevice, m_SceneCreateTypelessListPass);
	//m_MaterialModelPassInstanced->ConfigurePipeline(OutputRenderTarget, Viewport);


	//
	// BEFORE SCENE
	//
	Add3DPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, OutputRenderTarget));
#ifdef ENABLE_HDR
	Add3DPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, HDRRenderTarget));
#endif

	Add3DPass(skyboxPass);

	//
	// SCENE
	//
	Add3DPass(m_SceneCreateTypelessListPass);
	Add3DPass(MakeShared(InvokeFunctionPass, m_RenderDevice, std::bind(&CRendererForward::DoUpdateLights, this)));
	Add3DPass(m_MaterialModelPass);
	//AddPass(m_MaterialModelPassInstanced);


	for (const auto& it : m_BaseManager.GetManager<IznPluginsManager>()->GetAllPlugins())
		if (auto ext = std::dynamic_pointer_cast<IRendererExtender>(it))
			ext->Extend3DPasses(*this, m_RenderDevice, m_SceneCreateTypelessListPass, OutputRenderTarget);


	//
	// AFTER SCENE
	//
	Add3DPass(MakeShared(CParticlesPass, m_RenderDevice, m_Scene)->ConfigurePipeline(
#ifdef ENABLE_HDR
		HDRRenderTarget
#else
		OutputRenderTarget
#endif
	));



	//
	// POSTPROCESS
	//
	auto inputTexture = HDRRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0);

#ifdef ENABLE_HDR

	auto glowPass = MakeShared(CPassPostprocess_Glow, m_RenderDevice, inputTexture);
	glowPass->ConfigurePipeline(OutputRenderTarget);
	Add3DPass(glowPass);

	auto gaussHorizontal = MakeShared(CPassPostprocess_Gauss, m_RenderDevice, glowPass->GetOutputTexture(), true);
	gaussHorizontal->ConfigurePipeline(OutputRenderTarget);
	Add3DPass(gaussHorizontal);

	auto gaussVertical = MakeShared(CPassPostprocess_Gauss, m_RenderDevice, gaussHorizontal->GetOutputTexture(), false);
	gaussVertical->ConfigurePipeline(OutputRenderTarget);
	Add3DPass(gaussVertical);

	auto accumTextures = MakeShared(CPassPostprocess_AccumTextures, m_RenderDevice, std::vector<std::shared_ptr<ITexture>>({ inputTexture, gaussVertical->GetOutputTexture() }));
	accumTextures->ConfigurePipeline(OutputRenderTarget);
	Add3DPass(accumTextures);

	//auto hdrPass = MakeShared(CPassPostprocess_HDR, m_RenderDevice, accumTextures->GetOutputTexture());
	//hdrPass->ConfigurePipeline(OutputRenderTarget);
	//Add3DPass(hdrPass);

	Add3DPass(MakeShared(CPassPostprocess_ApplyTexture, m_RenderDevice, accumTextures->GetOutputTexture())->ConfigurePipeline(OutputRenderTarget));

#endif



	//
	// DEBUG
	//
	Add3DPass(MakeShared(CDebugPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget));
	//Add3DPass(MakeShared(CDrawBonesPass, m_Scene)->ConfigurePipeline(OutputRenderTarget));
	//AddPass(MakeShared(CDrawBoundingBoxPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget, Viewport));


	//
	// UI
	//
	AddUIPass(MakeShared(CUIControlPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget));
	AddUIPass(MakeShared(CUIFontPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget));
}



//
// Private
//
void CRendererForward::DoUpdateLights()
{
	std::vector<SLightVS> lightsVS;
	for (const auto& light : m_SceneCreateTypelessListPass->GetLightList())
	{
		auto lightOwner = light.SceneNode;
		const auto& lightStruct = light.Light->GetLightStruct();

		SLightVS lightVS;
		lightVS.Light = lightStruct;
		lightVS.LightPositionVS = m_Scene.GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightOwner->GetPosition(), 1.0f);
		lightVS.LightDirectionVS = glm::normalize(m_Scene.GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightOwner->GetRotationEuler(), 0.0f));
		lightsVS.push_back(lightVS);
	}

	m_LightsCnt = lightsVS.size();
	if (m_LightsCnt == 0)
		return;

	if (lightsVS.size() > m_LightsBuffer->GetElementCount())
		m_LightsBuffer = m_RenderDevice.GetObjectsFactory().CreateStructuredBuffer(lightsVS, EAccess::CPUWrite);
	else
		m_LightsBuffer->Set(lightsVS);

	m_MaterialModelPass->GetLightsShaderParameter()->Set(m_LightsBuffer);
	//m_MaterialModelPassInstanced->GetLightsShaderParameter()->Set(m_LightsBuffer);
}

std::shared_ptr<IRenderTarget> CRendererForward::CreateHDRRenderTarget(std::shared_ptr<IRenderTarget> OutputRenderTarget)
{
	ITexture::TextureFormat colorTextureFormat
	(
		ITexture::Components::RGBA,
		ITexture::Type::Float,
		OutputRenderTarget->GetSamplesCount(),
		16, 16, 16, 16, 0, 0
	);
	auto texture = m_RenderDevice.GetObjectsFactory().CreateTexture2D(OutputRenderTarget->GetViewport().GetWidth(), OutputRenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);

	// GBuffer contains Depth and Stencil buffer with object. We may use this data.
	auto outputRenderTargetWithCustomDepth = m_RenderDevice.GetObjectsFactory().CreateRenderTarget();
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::Color0, texture);
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, OutputRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
	return outputRenderTargetWithCustomDepth;
}
