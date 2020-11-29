#include "stdafx.h"

#define ENABLE_HDR

// General
#include "RendererDeffered.h"

// Additional
#include "Passes/Technical/ClearRenderTargetPass.h"
#include "Passes/PostprocessRendering/Postprocess_HDR.h"

#include "Passes/DebugPass.h"
#include "Passes/ParticlesPass.h"
#include "Passes/DrawBonesPass.h"

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

	m_Deffered_ScenePass = MakeShared(CPassDeffered_DoRenderScene, m_RenderDevice, m_SceneCreateTypelessListPass);
	m_Deffered_ScenePass->ConfigurePipeline(OutputRenderTarget);

	// GBuffer contains Depth and Stencil buffer with object. We may use this data.
	auto outputRenderTargetWithCustomDepth = m_RenderDevice.GetObjectsFactory().CreateRenderTarget();
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::Color0, OutputRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, m_Deffered_ScenePass->GetTextureDepthStencil());

	m_Deffered_Lights = MakeShared(CPassDeffered_ProcessLights, m_RenderDevice, m_SceneCreateTypelessListPass);
	m_Deffered_Lights->CreateShadowPipeline();

#ifdef ENABLE_HDR
	auto HDRRenderTarget = CreateHDRRenderTarget(OutputRenderTarget);
#endif

	m_Deffered_UIQuadPass = MakeShared(CPassDeffered_RenderUIQuad, m_RenderDevice, m_Deffered_ScenePass, m_Deffered_Lights);
	m_Deffered_UIQuadPass->ConfigurePipeline(
#ifdef ENABLE_HDR
		HDRRenderTarget
#else
		OutputRenderTarget
#endif
	);


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
	Add3DPass(m_Deffered_ScenePass);
	Add3DPass(m_Deffered_Lights);
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
#ifdef ENABLE_HDR
	Add3DPass(MakeShared(CPassPostprocess_HDR, m_RenderDevice, HDRRenderTarget)->ConfigurePipeline(OutputRenderTarget));
#endif


	//
	// DEBUG & TECHNICAL
	//
	Add3DPass(MakeShared(CSkyboxPass, m_RenderDevice)->ConfigurePipeline(outputRenderTargetWithCustomDepth));
	Add3DPass(MakeShared(CDebugPass, m_RenderDevice, m_Scene)->ConfigurePipeline(outputRenderTargetWithCustomDepth));
	Add3DPass(MakeShared(CDrawBonesPass, m_Scene)->ConfigurePipeline(outputRenderTargetWithCustomDepth));
	

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
		32, 32, 32, 32, 0, 0
	);
	auto texture = m_RenderDevice.GetObjectsFactory().CreateTexture2D(OutputRenderTarget->GetViewport().GetWidth(), OutputRenderTarget->GetViewport().GetHeight(), 1, colorTextureFormat);

	// GBuffer contains Depth and Stencil buffer with object. We may use this data.
	auto outputRenderTargetWithCustomDepth = m_RenderDevice.GetObjectsFactory().CreateRenderTarget();
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::Color0, texture);
	outputRenderTargetWithCustomDepth->AttachTexture(IRenderTarget::AttachmentPoint::DepthStencil, OutputRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));
	return outputRenderTargetWithCustomDepth;
}
