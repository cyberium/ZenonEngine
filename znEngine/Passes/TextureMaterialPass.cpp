#include "stdafx.h"

// General
#include "TextureMaterialPass.h"

// Additional
#include "Materials/MaterialTextured.h"

CTexturedMaterialPass::CTexturedMaterialPass(IRenderDevice* RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{}

CTexturedMaterialPass::~CTexturedMaterialPass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CTexturedMaterialPass::CreatePipeline(IRenderTarget* RenderTarget, const Viewport * Viewport)
{
	IShader* g_pVertexShader;
	IShader* g_pPixelShader;

	if (GetRenderDevice()->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "VS_main", "latest");
		g_pPixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "PS_main", "latest");
	}
	else
	{
		g_pVertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_OGL_3D_TEXTURED_VS", IShader::ShaderMacros(), "", "");
		g_pPixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_OGL_3D_TEXTURED_PS", IShader::ShaderMacros(), "", "");
	}
	g_pVertexShader->LoadInputLayoutFromReflector();

	//std::vector<SCustomVertexElement> elements;
	//elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	//elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	//elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	//g_pVertexShader->LoadInputLayoutFromCustomElements(elements);

	// PIPELINES
	IPipelineState* Pipeline = GetRenderDevice()->CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(disableBlending);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->GetRasterizerState()->SetViewport(Viewport);
	Pipeline->SetShader(EShaderType::VertexShader, g_pVertexShader);
	Pipeline->SetShader(EShaderType::PixelShader, g_pPixelShader);

	ISamplerState* sampler = GetRenderDevice()->CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	Pipeline->SetSampler(0, sampler);

	return SetPipeline(Pipeline);
}



//
// IVisitor
//
bool CTexturedMaterialPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CTexturedMaterialPass::Visit(IGeometry * Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	const MaterialTextured* objMaterial = dynamic_cast<const MaterialTextured*>(Material);
	if (objMaterial == nullptr)
		return false;

	return Base3DPass::Visit(Geometry, Material, GeometryPartParams);
}