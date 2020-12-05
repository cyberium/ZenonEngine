#include "stdafx.h"

#define ENABLE_HDR

// General
#include "RendererDeffered.h"

// Additional
#include "Passes/Technical/ClearRenderTargetPass.h"
#include "Passes/PostprocessRendering/Postprocess_HDR.h"
#include "Passes/PostprocessRendering/Postprocess_Glow.h"
#include "Passes/PostprocessRendering/Postprocess_Gauss.h"
#include "Passes/PostprocessRendering/Postprocess_AccumTextures.h"
#include "Passes/PostprocessRendering/Postprocess_ApplyTexture.h"

#include "Passes/DebugPass.h"
#include "Passes/ParticlesPass.h"
#include "Passes/DrawBonesPass.h"
#include "Passes/DrawBoundingBoxPass.h"
#include "Passes/DrawLightFrustumPass.h"

#include "Passes/SkyboxPass.h"

#include "Passes/UI/UIFontPass.h"
#include "Passes/UI/UIControlPass.h"

CRendererDeffered::CRendererDeffered(IBaseManager& BaseManager, IScene& Scene)
	: RendererBase(BaseManager, Scene)
{
}

CRendererDeffered::~CRendererDeffered()
{
}



void CRendererDeffered::Initialize(std::shared_ptr<IRenderTarget> OutputRenderTarget)
{
	m_SceneCreateTypelessListPass = MakeShared(CSceneCreateTypelessListPass, m_RenderDevice, m_Scene);

	auto skyboxPass = MakeShared(CSkyboxPass, m_RenderDevice);
	skyboxPass->ConfigurePipeline(OutputRenderTarget);

	m_RenderScenePass = MakeShared(CPassDeffered_DoRenderScene, m_RenderDevice, m_SceneCreateTypelessListPass);
	m_RenderScenePass->ConfigurePipeline(OutputRenderTarget);
	m_RenderScenePass->SetEnviorementTexture(skyboxPass->GetSkyboxCubeTexture());

	m_ShadowMapsPass = MakeShared(CPassDeffered_ShadowMaps, m_RenderDevice, m_SceneCreateTypelessListPass);
	m_ShadowMapsPass->CreateShadowPipeline();

#ifdef ENABLE_HDR
	auto HDRRenderTarget = CreateHDRRenderTarget(m_RenderScenePass->GetGBufferRenderTarget());
#endif

	m_Deffered_UIQuadPass = MakeShared(CPassDeffered_RenderUIQuad, m_RenderDevice, m_RenderScenePass, m_ShadowMapsPass);
	m_Deffered_UIQuadPass->ConfigurePipeline(OutputRenderTarget);

	//
	// BEFORE SCENE
	//
	Add3DPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, OutputRenderTarget));
#ifdef ENABLE_HDR
	Add3DPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, HDRRenderTarget));
#endif

	//
	// SCENE
	//
	Add3DPass(m_SceneCreateTypelessListPass);
	Add3DPass(m_RenderScenePass);
	Add3DPass(m_ShadowMapsPass);
	Add3DPass(m_Deffered_UIQuadPass);


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
	

	//Add3DPass(MakeShared(CPassPostprocess_ApplyTexture, m_RenderDevice, inputTexture)->ConfigurePipeline(OutputRenderTarget));
#endif


	//
	// DEBUG & TECHNICAL
	//

	// GBuffer contains Depth and Stencil buffer with object. We may use this data.
	auto outputRenderTargetWithCustomDepth = m_RenderDevice.GetObjectsFactory().CreateRenderTarget();
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::Color0, OutputRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_RenderScenePass->GetTextureDepthStencil());

	Add3DPass(MakeShared(CSkyboxPass, m_RenderDevice)->ConfigurePipeline(outputRenderTargetWithCustomDepth));
	Add3DPass(MakeShared(CDebugPass, m_RenderDevice, m_Scene)->ConfigurePipeline(outputRenderTargetWithCustomDepth));
	//Add3DPass(MakeShared(CDrawBonesPass, m_Scene)->ConfigurePipeline(outputRenderTargetWithCustomDepth));
	Add3DPass(MakeShared(CDrawBoundingBoxPass, m_RenderDevice, m_Scene)->ConfigurePipeline(outputRenderTargetWithCustomDepth));
	Add3DPass(MakeShared(CDrawLightFrustumPass, m_RenderDevice, m_Scene)->ConfigurePipeline(outputRenderTargetWithCustomDepth));

	//
	// UI
	//
	AddUIPass(MakeShared(CUIControlPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget));
	AddUIPass(MakeShared(CUIFontPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget));
}



//
// Private
//
std::shared_ptr<IRenderTarget> CRendererDeffered::CreateHDRRenderTarget(std::shared_ptr<IRenderTarget> OutputRenderTarget)
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
	auto rt = m_RenderDevice.GetObjectsFactory().CreateRenderTarget();
	rt->AttachTexture(IRenderTarget::AttachmentPoint::Color0, texture);
	rt->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, OutputRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
	return rt;
}
