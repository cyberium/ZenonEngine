#include "stdafx.h"

// General
#include "DefferedRenderFinal.h"

CDefferedRenderFinal::CDefferedRenderFinal(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<CDefferedRender> DefferedRender, std::shared_ptr<IRenderWindow> RenderWindow)
	: AbstractPass(RenderDevice)
	, m_DefferedRender(DefferedRender)
	, m_RenderWindow(RenderWindow)
{
	CreatePipeline();
}

CDefferedRenderFinal::~CDefferedRenderFinal()
{
}



//
// IRenderPass
//
void CDefferedRenderFinal::Render(RenderEventArgs & e)
{
	const IShader* vertexShader = m_DefferedFinalPipeline->GetShader(EShaderType::PixelShader).get();

	m_DefferedFinalPipeline->Bind();

	SGeometryPartParams GeometryPartParams;
	m_QuadMesh->GetGeometry()->Render(&e, nullptr, m_DefferedFinalPipeline->GetShaders(), nullptr, GeometryPartParams);

	m_DefferedFinalPipeline->UnBind();
}



//
// Private
//
void CDefferedRenderFinal::CreatePipeline()
{
	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

	std::shared_ptr<IShader> vertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_DEFFERED", IShader::ShaderMacros(), "VS_ScreenQuad", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_DEFFERED", IShader::ShaderMacros(), "PS_ScreenQuad", "latest");

	// PIPELINES
	m_DefferedFinalPipeline = GetRenderDevice()->CreatePipelineState();
	m_DefferedFinalPipeline->GetBlendState()->SetBlendMode(disableBlending);
	m_DefferedFinalPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	m_DefferedFinalPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	m_DefferedFinalPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	m_DefferedFinalPipeline->SetRenderTarget(m_RenderWindow->GetRenderTarget());
	m_DefferedFinalPipeline->GetRasterizerState()->SetViewport(m_RenderWindow->GetViewport());
	m_DefferedFinalPipeline->SetShader(EShaderType::VertexShader, vertexShader);
	m_DefferedFinalPipeline->SetShader(EShaderType::PixelShader, pixelShader);
	
	std::shared_ptr<ISamplerState> sampler = GetRenderDevice()->CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	m_DefferedFinalPipeline->SetSampler(0, sampler);

	m_DefferedFinalPipeline->SetTexture(0, m_DefferedRender->GetTexture0());
	m_DefferedFinalPipeline->SetTexture(1, m_DefferedRender->GetTexture1());
	m_DefferedFinalPipeline->SetTexture(2, m_DefferedRender->GetTexture2());
	m_DefferedFinalPipeline->SetTexture(3, m_DefferedRender->GetTexture3());

	m_QuadMesh = GetRenderDevice()->GetPrimitiveCollection()->CreateQuad();
}
