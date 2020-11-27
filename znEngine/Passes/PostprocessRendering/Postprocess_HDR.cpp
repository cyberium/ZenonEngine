#include "stdafx.h"

// General
#include "Postprocess_HDR.h"

CPassPostprocess_HDR::CPassPostprocess_HDR(IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> HDRRenderTarget)
	: RenderPassPipelined(RenderDevice)
	, m_HDRRenderTarget(HDRRenderTarget)
{}

CPassPostprocess_HDR::~CPassPostprocess_HDR()
{}



//
// IRenderPass
//
void CPassPostprocess_HDR::Render(RenderEventArgs& e)
{
	m_QuadGeometry->Render(GetPipeline().GetShaders().at(EShaderType::VertexShader).get());
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassPostprocess_HDR::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateQuad();

	auto samplesCnt = std::to_string(RenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Deffered_HDR.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt } });
	vertexShader->LoadInputLayoutFromReflector();

	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Deffered_HDR.hlsl", "PS_ScreenQuad", { {"MULTISAMPLED", samplesCnt }});

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(additiveBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	
	GetPipeline().SetTexture(0, m_HDRRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	//GetPipeline().SetTexture(1, m_HDRRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::DepthStencil));

	return shared_from_this();
}
