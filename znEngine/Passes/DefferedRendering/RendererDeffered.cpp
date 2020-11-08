#include "stdafx.h"

// General
#include "RendererDeffered.h"

// Additional
#include "Passes/Technical/ClearRenderTargetPass.h"
#include "Passes/MaterialDebugPass.h"
#include "Passes/MaterialTexturedPass.h"
#include "Passes/MaterialParticlePass.h"
#include "Passes/DrawBonesPass.h"

#include "Passes/UI/UIFontPass.h"
#include "Passes/UI/UIColorPass.h"
#include "Passes/UI/UIControlConsolePass.h"

CRendererDeffered::CRendererDeffered(IBaseManager& BaseManager, IScene& Scene)
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
	m_Deffered_ScenePass->GetPipeline().GetRenderTarget()->Resize(NewWidth, NewHeight);
	m_Deffered_UIQuadPass->GetPipeline().GetRenderTarget()->Resize(NewWidth, NewHeight);
	m_Deffered_HDR->GetPipeline().GetRenderTarget()->Resize(NewWidth, NewHeight);

	//m_FinalRenderTarget->Resize(NewWidth, NewHeight);
}



void CRendererDeffered::Initialize(std::shared_ptr<IRenderTarget> OutputRenderTarget, const Viewport * Viewport)
{
	AddPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, OutputRenderTarget));
	
	m_SceneCreateTypelessListPass = MakeShared(CSceneCreateTypelessListPass, m_RenderDevice, m_Scene);

	m_Deffered_ScenePass = MakeShared(CPassDeffered_DoRenderScene, m_RenderDevice, m_SceneCreateTypelessListPass);
	m_Deffered_ScenePass->ConfigurePipeline(OutputRenderTarget, Viewport);

	// GBuffer contains Depth and Stencil buffer with object. We may use this data.
	auto outputRenderTargetWithCustomDepth = m_RenderDevice.GetObjectsFactory().CreateRenderTarget();
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::Color0, OutputRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_Deffered_ScenePass->GetTextureDepthStencil());


	m_Deffered_Lights = MakeShared(CPassDeffered_ProcessLights, m_RenderDevice, m_SceneCreateTypelessListPass);
	m_Deffered_Lights->CreateShadowPipeline();

	//auto HDRRenderTarget = CreateHDRRenderTarget(OutputRenderTarget, Viewport);
	//m_Deffered_HDR = MakeShared(CPassPostprocess_HDR, m_RenderDevice, HDRRenderTarget);
	//m_Deffered_HDR->ConfigurePipeline(OutputRenderTarget, Viewport);

	m_Deffered_UIQuadPass = MakeShared(CPassDeffered_RenderUIQuad, m_RenderDevice, m_Deffered_ScenePass, m_Deffered_Lights);
	m_Deffered_UIQuadPass->ConfigurePipeline(/*HDRRenderTarget*/OutputRenderTarget, Viewport);



	AddPass(m_SceneCreateTypelessListPass);
	AddPass(m_Deffered_ScenePass);
	AddPass(m_Deffered_Lights);
	//AddPass(m_BaseManager.GetManager<IRenderPassFactory>()->CreateRenderPass("DebugPass", m_RenderDevice, outputRenderTargetWithCustomDepth, Viewport, m_Scene.lock()));

	// Final UI Pass
	m_UIPasses.push_back(m_Deffered_UIQuadPass);

	// HDR
	//AddPass(MakeShared(ClearRenderTargetPass, m_RenderDevice, HDRRenderTarget));
	//m_UIPasses.push_back(m_Deffered_HDR);

	m_UIPasses.push_back(MakeShared(CUIFontPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget, Viewport));
	m_UIPasses.push_back(MakeShared(CUIColorPass, m_RenderDevice, m_Scene)->ConfigurePipeline(OutputRenderTarget, Viewport));
	m_UIPasses.push_back(MakeShared(CUIControlConsolePass, m_RenderDevice)->ConfigurePipeline(OutputRenderTarget, Viewport));
}



//
// Private
//
std::shared_ptr<IRenderTarget> CRendererDeffered::CreateHDRRenderTarget(std::shared_ptr<IRenderTarget> OutputRenderTarget, const Viewport * Viewport)
{
	ITexture::TextureFormat colorTextureFormat
	(
		ITexture::Components::RGBA,
		ITexture::Type::Float,
		OutputRenderTarget->GetSamplesCount(),
		32, 32, 32, 32, 0, 0
	);
	auto texture = m_RenderDevice.GetObjectsFactory().CreateTexture2D(Viewport->GetWidth(), Viewport->GetHeight(), 1, colorTextureFormat);

	// GBuffer contains Depth and Stencil buffer with object. We may use this data.
	auto outputRenderTargetWithCustomDepth = m_RenderDevice.GetObjectsFactory().CreateRenderTarget();
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::Color0, texture);
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, OutputRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
	return outputRenderTargetWithCustomDepth;
}
