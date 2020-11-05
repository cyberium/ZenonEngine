#include "stdafx.h"

// General
#include "RendererForward.h"

// Additional
#include "Passes/Technical/ClearRenderTargetPass.h"
#include "Passes/Technical/InvokeFunctionPass.h"

#include "Passes/MaterialDebugPass.h"
#include "Passes/MaterialTexturedPass.h"
#include "Passes/MaterialParticlePass.h"
#include "Passes/DrawBonesPass.h"

#include "Passes/ForwardRendering/PassForward_DoRenderScene.h"

#include "Passes/Technical/InvokeFunctionPass.h"

#include "Passes/UI/UIFontPass.h"
#include "Passes/UI/UIColorPass.h"


struct __declspec(align(16)) SLightVS
{
	SLight Light;
	glm::vec4 LightPositionVS;
	glm::vec4 LightDirectionVS;
};


CRendererForward::CRendererForward(IBaseManager& BaseManager, IScene& Scene)
	: m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager.GetApplication().GetRenderDevice())
	, m_Scene(Scene)
{
	m_LightsBuffer = m_RenderDevice.GetObjectsFactory().CreateStructuredBuffer(nullptr, 1, sizeof(SLightVS), EAccess::CPUWrite);
}

CRendererForward::~CRendererForward()
{
}



//
// IRenderer
//
uint32 CRendererForward::AddPass(std::shared_ptr<IRenderPass> pass)
{
	_ASSERT_EXPR(pass, L"Pass must not be nullptr.");
	m_Passes.push_back(pass);
	return static_cast<uint32>(m_Passes.size()) - 1;
}

IRenderPass * CRendererForward::GetPass(uint32 ID) const
{
	if (ID < m_Passes.size())
		return m_Passes[ID].get();

	return nullptr;
}

void CRendererForward::Render3D(RenderEventArgs & renderEventArgs)
{
	for (auto pass : m_Passes)
	{
		if (pass && pass->IsEnabled())
		{
			pass->PreRender(renderEventArgs);
			pass->Render(renderEventArgs);
			pass->PostRender(renderEventArgs);
		}
	}
}

void CRendererForward::RenderUI(RenderEventArgs & renderEventArgs)
{
	for (auto pass : m_UIPasses)
	{
		if (pass && pass->IsEnabled())
		{
			pass->PreRender(renderEventArgs);
			pass->Render(renderEventArgs);
			pass->PostRender(renderEventArgs);
		}
	}
}

void CRendererForward::Resize(uint32 NewWidth, uint32 NewHeight)
{
	std::dynamic_pointer_cast<IRenderPassPipelined>(m_MaterialModelPass)->GetPipeline().GetRenderTarget()->Resize(NewWidth, NewHeight);
	//std::dynamic_pointer_cast<IRenderPassPipelined>(m_MaterialModelPassInstanced)->GetPipeline().GetRenderTarget()->Resize(NewWidth, NewHeight);
	std::dynamic_pointer_cast<IRenderPassPipelined>(m_RTSGroundPassInstanced)->GetPipeline().GetRenderTarget()->Resize(NewWidth, NewHeight);

	//m_FinalRenderTarget->Resize(NewWidth, NewHeight);
}

void CRendererForward::Initialize(std::shared_ptr<IRenderTarget> OutputRenderTarget, const Viewport * Viewport)
{
	AddPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, OutputRenderTarget));

	m_SceneCreateTypelessListPass = MakeShared(CSceneCreateTypelessListPass, m_RenderDevice, m_Scene);
	AddPass(m_SceneCreateTypelessListPass);

	m_MaterialModelPass = MakeShared(CPassForward_DoRenderScene, m_RenderDevice, m_Scene);
	m_MaterialModelPass->ConfigurePipeline(OutputRenderTarget, Viewport);

	//m_MaterialModelPassInstanced = MakeShared(CPassForward_DoRenderSceneInstanced, m_RenderDevice, m_SceneCreateTypelessListPass);
	//m_MaterialModelPassInstanced->ConfigurePipeline(OutputRenderTarget, Viewport);

	m_RTSGroundPassInstanced = MakeShared(CRTSGround_Pass, m_RenderDevice, m_Scene);
	m_RTSGroundPassInstanced->ConfigurePipeline(OutputRenderTarget, Viewport);

	std::shared_ptr<InvokeFunctionPass> invokePass = MakeShared(InvokeFunctionPass, m_RenderDevice, std::bind(&CRendererForward::DoUpdateLights, this));

	
	AddPass(m_SceneCreateTypelessListPass);
	AddPass(invokePass);
	AddPass(m_MaterialModelPass);
	//AddPass(m_MaterialModelPassInstanced);
	AddPass(m_RTSGroundPassInstanced);

	AddPass(MakeShared(CMaterial_Debug_Pass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget, Viewport));
	//AddPass(MakeShared(CDrawBonesPass, m_RenderDevice, m_Scene.lock())->ConfigurePipeline(OutputRenderTarget, Viewport));
	
	m_UIPasses.push_back(MakeShared(CUIFontPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget, Viewport));
	m_UIPasses.push_back(MakeShared(CUIColorPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget, Viewport));
}

void CRendererForward::DoUpdateLights()
{
	std::vector<SLightVS> lightsVS;
	for (const auto& light : m_SceneCreateTypelessListPass->GetLightList())
	{
		const ISceneNode* lightOwner = light.SceneNode;
		const SLight& lightStruct = light.Light->GetLightStruct();

		SLightVS lightVS;
		lightVS.Light = lightStruct;
		lightVS.LightPositionVS = m_Scene.GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightOwner->GetTranslation(), 1.0f);
		lightVS.LightDirectionVS = glm::normalize(m_Scene.GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightOwner->GetRotation(), 0.0f));
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
	m_RTSGroundPassInstanced->GetLightsShaderParameter()->Set(m_LightsBuffer);
}
