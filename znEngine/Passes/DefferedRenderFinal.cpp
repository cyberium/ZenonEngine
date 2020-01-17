#include "stdafx.h"

// General
#include "DefferedRenderFinal.h"

CDefferedRenderFinal::CDefferedRenderFinal(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<CDefferedRender> DefferedRender)
	: RenderPassPipelined(RenderDevice)
	, m_DefferedRender(DefferedRender)
{
}

CDefferedRenderFinal::~CDefferedRenderFinal()
{
}



//
// IRenderPass
//
void CDefferedRenderFinal::Render(RenderEventArgs & e)
{
	SGeometryPartParams GeometryPartParams;
	m_QuadMesh->GetGeometry()->Render(&e, nullptr, GetPipeline()->GetShaders(), nullptr, GeometryPartParams);
}



//
// IRenderPassPipelined
//
void CDefferedRenderFinal::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> vertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_DEFFERED", IShader::ShaderMacros(), "VS_ScreenQuad", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_DEFFERED", IShader::ShaderMacros(), "PS_ScreenQuad", "latest");

	// PIPELINES
	std::shared_ptr<IPipelineState> defferedFinalPipeline = GetRenderDevice()->CreatePipelineState();
	defferedFinalPipeline->GetBlendState()->SetBlendMode(disableBlending);
	defferedFinalPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	defferedFinalPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	defferedFinalPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	defferedFinalPipeline->SetRenderTarget(RenderTarget);
	defferedFinalPipeline->GetRasterizerState()->SetViewport(Viewport);
	defferedFinalPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	defferedFinalPipeline->SetShader(EShaderType::PixelShader, pixelShader);

	std::shared_ptr<ISamplerState> sampler = GetRenderDevice()->CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	defferedFinalPipeline->SetSampler(0, sampler);

	defferedFinalPipeline->SetTexture(0, m_DefferedRender->GetTexture0());
	defferedFinalPipeline->SetTexture(1, m_DefferedRender->GetTexture1());
	defferedFinalPipeline->SetTexture(2, m_DefferedRender->GetTexture2());
	defferedFinalPipeline->SetTexture(3, m_DefferedRender->GetTexture3());
	
	SetPipeline(defferedFinalPipeline);

	m_QuadMesh = GetRenderDevice()->GetPrimitiveCollection()->CreateQuad();
}
