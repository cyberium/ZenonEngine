#include "stdafx.h"

// General
#include "MaterialPassTransperent.h"

// Additional
#include "Materials/MaterialModel.h"

CMaterialPassTransperent::CMaterialPassTransperent(IRenderDevice* RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{}

CMaterialPassTransperent::~CMaterialPassTransperent()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CMaterialPassTransperent::CreatePipeline(IRenderTarget* RenderTarget, const Viewport * Viewport)
{
	IShader* g_pVertexShader;
	IShader* g_pPixelShader;

	if (GetRenderDevice()->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_MODEL_FORWARD", IShader::ShaderMacros(), "VS_main", "latest");
		g_pPixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_MODEL_FORWARD", IShader::ShaderMacros(), "PS_main", "latest");
	}
	g_pVertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	IPipelineState* Pipeline = GetRenderDevice()->CreatePipelineState();
	Pipeline->GetBlendState().SetBlendMode(alphaBlending);
	Pipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState().SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState().SetFillMode(IRasterizerState::FillMode::Solid);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->GetRasterizerState().SetViewport(Viewport);
	Pipeline->SetShader(EShaderType::VertexShader, g_pVertexShader);
	Pipeline->SetShader(EShaderType::PixelShader, g_pPixelShader);

	ISamplerState* sampler = GetRenderDevice()->CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	Pipeline->SetSampler(0, sampler);

	ISamplerState* samplerClamp = GetRenderDevice()->CreateSamplerState();
	samplerClamp->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	samplerClamp->SetWrapMode(ISamplerState::WrapMode::Clamp, ISamplerState::WrapMode::Clamp);
	Pipeline->SetSampler(1, samplerClamp);

	return SetPipeline(Pipeline);
}



//
// IVisitor
//
bool CMaterialPassTransperent::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CMaterialPassTransperent::Visit(IGeometry * Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	const MaterialModel* objMaterial = dynamic_cast<const MaterialModel*>(Material);
	if (objMaterial == nullptr)
		return false;

	//if (objMaterial->GetTexture(0) == nullptr || objMaterial->GetTexture(0)->IsTransparent() == false)
		return false;

	return Base3DPass::Visit(Geometry, Material, GeometryPartParams);
}
