#include "stdafx.h"

// General
#include "RendererForward.h"

// Additional
#include "Passes/Technical/ClearRenderTargetPass.h"
#include "Passes/Technical/InvokeFunctionPass.h"

#include "Passes/MaterialDebugPass.h"
#include "Passes/MaterialTexturedPass.h"
#include "Passes/MaterialParticlePass.h"
#include "Passes/MaterialModelPass.h"
#include "Passes/MaterialModelSkeletonPass.h"
#include "Passes/DrawBonesPass.h"
#include "Passes/UIFontPass.h"
#include "Passes/UIColorPass.h"

CRendererForward::CRendererForward(IBaseManager& BaseManager, const std::weak_ptr<IScene>& Scene)
	: m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager.GetApplication().GetRenderDevice())
	, m_Scene(Scene)
{
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

	//m_FinalRenderTarget->Resize(NewWidth, NewHeight);
}

void CRendererForward::Initialize(std::shared_ptr<IRenderTarget> OutputRenderTarget, const Viewport * Viewport)
{
	//m_FinalRenderTarget = OutputRenderTarget;


	m_SceneCreateTypelessListPass = MakeShared(CSceneCreateTypelessListPass, m_RenderDevice, m_Scene);

	m_LightsBuffer = m_RenderDevice.GetObjectsFactory().CreateStructuredBuffer(nullptr, 8, sizeof(SLight), EAccess::CPUWrite);

	auto materialModelPass = m_BaseManager.GetManager<IRenderPassFactory>()->CreateRenderPass("MaterialModelPass", m_RenderDevice, OutputRenderTarget, Viewport, m_Scene.lock());
	m_MaterialModelPass = std::dynamic_pointer_cast<IMaterialModelPass>(materialModelPass);

	
	auto invokePass = m_BaseManager.GetManager<IRenderPassFactory>()->CreateRenderPass("InvokePass", m_RenderDevice, OutputRenderTarget, Viewport, m_Scene.lock());
	std::dynamic_pointer_cast<IInvokeFunctionPass>(invokePass)->SetFunc([this]() {

		std::vector<SLight> lights;

		for (auto light : m_SceneCreateTypelessListPass->GetLightList())
		{
			auto lightStruct = std::dynamic_pointer_cast<ILight3D>(light.SceneNode->GetComponent<ILightComponent3D>())->GetLightStruct();
			lightStruct.PositionVS = m_Scene.lock()->GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightStruct.PositionWS.xyz(), 1.0f);
			lightStruct.DirectionVS = glm::normalize(m_Scene.lock()->GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightStruct.DirectionWS.xyz(), 0.0f));
			lights.push_back(lightStruct);
		}

		if (lights.size() > m_LightsBuffer->GetElementCount())
			m_LightsBuffer = m_RenderDevice.GetObjectsFactory().CreateStructuredBuffer(lights, EAccess::CPUWrite);
		else
			m_LightsBuffer->Set(lights);
		m_LightsCnt = lights.size();

		m_MaterialModelPass->GetLightsShaderParameter()->Set(m_LightsBuffer);
	});

	

	glm::vec4 color = glm::vec4(0.0, 0.0f, 0.0f, 1.0f);
	AddPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, OutputRenderTarget, ClearFlags::All, color, 1.0f, 0));	
	AddPass(m_SceneCreateTypelessListPass);
	AddPass(invokePass);
	AddPass(materialModelPass);

	AddPass(m_BaseManager.GetManager<IRenderPassFactory>()->CreateRenderPass("DebugPass", m_RenderDevice, OutputRenderTarget, Viewport, m_Scene.lock()));
	
	m_UIPasses.push_back(MakeShared(CUIFontPass, m_RenderDevice, m_Scene)->CreatePipeline(OutputRenderTarget, Viewport));
	m_UIPasses.push_back(MakeShared(CUIColorPass, m_RenderDevice, m_Scene)->CreatePipeline(OutputRenderTarget, Viewport));
}
