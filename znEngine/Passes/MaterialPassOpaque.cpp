#include "stdafx.h"

// General
#include "MaterialPassOpaque.h"

// Additional
#include "Materials/MaterialModel.h"

CMaterialPassOpaque::CMaterialPassOpaque(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{}

CMaterialPassOpaque::~CMaterialPassOpaque()
{}



//
// IRenderPassPipelined
//
void CMaterialPassOpaque::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> g_pVertexShader;
	std::shared_ptr<IShader> g_pPixelShader;

	if (GetRenderDevice()->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_MODEL", IShader::ShaderMacros(), "VS_PTN", "latest");
		g_pPixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_MODEL", IShader::ShaderMacros(), "PS_main", "latest");
	}
	g_pVertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	std::shared_ptr<IPipelineState> Pipeline = GetRenderDevice()->CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(disableBlending);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->GetRasterizerState()->SetViewport(Viewport);
	Pipeline->SetShader(EShaderType::VertexShader, g_pVertexShader);
	Pipeline->SetShader(EShaderType::PixelShader, g_pPixelShader);

	std::shared_ptr<ISamplerState> sampler = GetRenderDevice()->CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	Pipeline->SetSampler(0, sampler);

	std::shared_ptr<ISamplerState> samplerClamp = GetRenderDevice()->CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	Pipeline->SetSampler(1, samplerClamp);

	SetPipeline(Pipeline);
}



//
// IVisitor
//
bool CMaterialPassOpaque::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CMaterialPassOpaque::Visit(IGeometry * Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	const MaterialModel* objMaterial = dynamic_cast<const MaterialModel*>(Material);
	if (objMaterial == nullptr)
		return false;

	//if (objMaterial->GetTexture(0) == nullptr || objMaterial->GetTexture(0)->IsTransparent() == true)
	//	return false;

	return Base3DPass::Visit(Geometry, Material, GeometryPartParams);
}
