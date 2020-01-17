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
	const IShader* vertexShader = m_DefferedFinalPipeline->GetShader(SShaderType::PixelShader).get();

	m_DefferedFinalPipeline->Bind();

	m_Sampler->Bind(0, vertexShader, IShaderParameter::Type::Sampler);

	m_DefferedRender->GetTexture0()->Bind(0, vertexShader, IShaderParameter::Type::Texture);
	m_DefferedRender->GetTexture1()->Bind(1, vertexShader, IShaderParameter::Type::Texture);
	m_DefferedRender->GetTexture2()->Bind(2, vertexShader, IShaderParameter::Type::Texture);
	m_DefferedRender->GetTexture3()->Bind(3, vertexShader, IShaderParameter::Type::Texture);

	SGeometryPartParams GeometryPartParams;
	m_QuadMesh->GetGeometry()->Render(&e, nullptr, m_DefferedFinalPipeline->GetShaders(), nullptr, GeometryPartParams);

	m_DefferedRender->GetTexture0()->UnBind(0, vertexShader, IShaderParameter::Type::Texture);
	m_DefferedRender->GetTexture1()->UnBind(1, vertexShader, IShaderParameter::Type::Texture);
	m_DefferedRender->GetTexture2()->UnBind(2, vertexShader, IShaderParameter::Type::Texture);
	m_DefferedRender->GetTexture3()->UnBind(3, vertexShader, IShaderParameter::Type::Texture);

	m_Sampler->UnBind(0, vertexShader, IShaderParameter::Type::Sampler);

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

	std::shared_ptr<IShader> vertexShader = GetRenderDevice()->CreateShader(SShaderType::VertexShader, "IDB_SHADER_3D_DEFFERED", IShader::ShaderMacros(), "VS_ScreenQuad", "latest");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice()->CreateShader(SShaderType::PixelShader, "IDB_SHADER_3D_DEFFERED", IShader::ShaderMacros(), "PS_ScreenQuad", "latest");

	// PIPELINES
	m_DefferedFinalPipeline = GetRenderDevice()->CreatePipelineState();
	m_DefferedFinalPipeline->GetBlendState()->SetBlendMode(disableBlending);
	m_DefferedFinalPipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	m_DefferedFinalPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	m_DefferedFinalPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	m_DefferedFinalPipeline->SetRenderTarget(m_RenderWindow->GetRenderTarget());
	m_DefferedFinalPipeline->GetRasterizerState()->SetViewport(m_RenderWindow->GetViewport());
	m_DefferedFinalPipeline->SetShader(SShaderType::VertexShader, vertexShader);
	m_DefferedFinalPipeline->SetShader(SShaderType::PixelShader, pixelShader);

	m_Sampler = GetRenderDevice()->CreateSamplerState();
	m_Sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	m_Sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);

	m_QuadMesh = GetRenderDevice()->GetPrimitiveCollection()->CreateQuad();
}
