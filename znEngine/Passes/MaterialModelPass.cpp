#include "stdafx.h"

// General
#include "MaterialModelPass.h"

// Additional
#include "Materials/MaterialModel.h"

CMaterialModelPass::CMaterialModelPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{
	
}

CMaterialModelPass::~CMaterialModelPass()
{}


IShaderParameter * CMaterialModelPass::GetLightsShaderParameter() const
{
	return m_ShaderLightsBufferParameter;
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CMaterialModelPass::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_MODEL_FORWARD", "VS_PTN");
		pixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_MODEL_FORWARD", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	auto Pipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(disableBlending);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->SetShader(EShaderType::VertexShader, vertexShader);
	Pipeline->SetShader(EShaderType::PixelShader, pixelShader);

	auto sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	Pipeline->SetSampler(0, sampler);

	auto samplerClamp = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	Pipeline->SetSampler(1, samplerClamp);

	m_ShaderLightsBufferParameter = &pixelShader->GetShaderParameterByName("Lights");
	_ASSERT(m_ShaderLightsBufferParameter->IsValid());

	return SetPipeline(Pipeline);
}



//
// IVisitor
//
EVisitResult CMaterialModelPass::Visit(const IModel * Model)
{
	EVisitResult visitResult = AllowVisitChilds;

		visitResult = Base3DPass::Visit(Model);
	
	return visitResult;
}

EVisitResult CMaterialModelPass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const MaterialModel* objMaterial = dynamic_cast<const MaterialModel*>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	//if (objMaterial->GetTexture(0) == nullptr || objMaterial->GetTexture(0)->IsTransparent() == true)
	//	return false;
	if (Material)
		Material->Bind(GetRenderEventArgs().PipelineState->GetShaders());

	m_ShaderLightsBufferParameter->Bind();

	Geometry->Render(GetRenderEventArgs(), GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get(), GeometryDrawArgs);

	m_ShaderLightsBufferParameter->Unbind();

	if (Material)
		Material->Unbind(GetRenderEventArgs().PipelineState->GetShaders());

	return AllowAll;
}
