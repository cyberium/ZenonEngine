#include "stdafx.h"

// General
#include "RendererDeffered.h"

// Additional
#include "Passes/Technical/ClearRenderTargetPass.h"
#include "Passes/MaterialDebugPass.h"
#include "Passes/MaterialTexturedPass.h"
#include "Passes/MaterialParticlePass.h"
#include "Passes/MaterialModelPass.h"
#include "Passes/MaterialModelSkeletonPass.h"
#include "Passes/DrawBonesPass.h"
#include "Passes/UIFontPass.h"
#include "Passes/UIColorPass.h"

CRendererDeffered::CRendererDeffered(IBaseManager& BaseManager, const std::weak_ptr<IScene>& Scene)
	: m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager.GetApplication().GetRenderDevice())
	, m_Scene(Scene)
{
}

CRendererDeffered::~CRendererDeffered()
{
}



//
// IRenderer
//
uint32 CRendererDeffered::AddPass(std::shared_ptr<IRenderPass> pass)
{
	_ASSERT_EXPR(pass, L"Pass must not be nullptr.");
	m_Passes.push_back(pass);
	return static_cast<uint32>(m_Passes.size()) - 1;
}

IRenderPass * CRendererDeffered::GetPass(uint32 ID) const
{
	if (ID < m_Passes.size())
		return m_Passes[ID].get();

	return nullptr;
}

std::shared_ptr<IRenderTarget> CRendererDeffered::GetRenderTarget() const
{
	return m_FinalRenderTarget;
}

void CRendererDeffered::Render3D(RenderEventArgs & renderEventArgs)
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

void CRendererDeffered::RenderUI(RenderEventArgs & renderEventArgs)
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

void CRendererDeffered::Resize(uint32 NewWidth, uint32 NewHeight)
{
	m_DefferedRenderPass->GetPipeline().GetRenderTarget()->Resize(NewWidth, NewHeight);
	m_DefferedFinalRenderPass->GetPipeline().GetRenderTarget()->Resize(NewWidth, NewHeight);

	m_FinalRenderTarget->Resize(NewWidth, NewHeight);
}



void CRendererDeffered::Initialize(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_SceneCreateTypelessListPass = MakeShared(CSceneCreateTypelessListPass, m_RenderDevice, m_Scene);

	m_DefferedRenderPass = MakeShared(CPassDeffered_DoRenderScene, m_RenderDevice, m_SceneCreateTypelessListPass);
	m_DefferedRenderPass->CreatePipeline(RenderTarget, Viewport);

	m_DefferedRenderPrepareLights = MakeShared(CPassDeffered_ProcessLights, m_RenderDevice, m_SceneCreateTypelessListPass);
	m_DefferedRenderPrepareLights->CreatePipeline(nullptr, nullptr);

	m_DefferedFinalRenderPass = MakeShared(CPassDeffered_RenderUIQuad, m_RenderDevice, m_DefferedRenderPass, m_DefferedRenderPrepareLights);
	m_DefferedFinalRenderPass->CreatePipeline(RenderTarget, Viewport);

	m_FinalRenderTarget = m_RenderDevice.GetObjectsFactory().CreateRenderTarget();
	m_FinalRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::Color0, RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	m_FinalRenderTarget->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_DefferedRenderPass->GetTextureDepthStencil());

	glm::vec4 color = glm::vec4(0.0, 0.0f, 0.0f, 1.0f);
	AddPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, RenderTarget, ClearFlags::All, color, 1.0f, 0));
	AddPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, m_FinalRenderTarget, ClearFlags::All, color, 1.0f, 0));
	
	AddPass(m_SceneCreateTypelessListPass);
	AddPass(m_DefferedRenderPass);
	AddPass(m_DefferedRenderPrepareLights);
	AddPass(m_BaseManager.GetManager<IRenderPassFactory>()->CreateRenderPass("DebugPass", m_RenderDevice, m_FinalRenderTarget, Viewport, m_Scene.lock()));

	m_UIPasses.push_back(m_DefferedFinalRenderPass);

	//AddPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, m_FinalRenderTarget, ClearFlags::Color, color, 1.0f, 0));
	
	m_UIPasses.push_back(MakeShared(CUIFontPass, m_RenderDevice, m_Scene)->CreatePipeline(RenderTarget, Viewport));
	m_UIPasses.push_back(MakeShared(CUIColorPass, m_RenderDevice, m_Scene)->CreatePipeline(RenderTarget, Viewport));
}
