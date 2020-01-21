#include "stdafx.h"

// General
#include "TextureDebugPass.h"

// Additional
#include "Materials/MaterialDebug.h"

CDebugMaterialPass::CDebugMaterialPass(IRenderDevice* RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{}

CDebugMaterialPass::~CDebugMaterialPass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDebugMaterialPass::CreatePipeline(IRenderTarget* RenderTarget, const Viewport * Viewport)
{
	IShader* g_pVertexShader;
	IShader* g_pGeometryShader;
	IShader* g_pPixelShader;

	if (GetRenderDevice()->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		g_pVertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "VS_main", "latest");
		g_pGeometryShader = GetRenderDevice()->CreateShader(EShaderType::GeometryShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "GS_main", "latest");
		g_pPixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "PS_main", "latest");
	}
	else
	{
		g_pVertexShader = GetRenderDevice()->CreateShader(EShaderType::VertexShader, "IDB_SHADER_OGL_3D_DEBUG_VS", IShader::ShaderMacros(), "", "");
		g_pPixelShader = GetRenderDevice()->CreateShader(EShaderType::PixelShader, "IDB_SHADER_OGL_3D_DEBUG_PS", IShader::ShaderMacros(), "", "");
	}
	g_pVertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	IPipelineState* Pipeline = GetRenderDevice()->CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(disableBlending);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->GetRasterizerState()->SetViewport(Viewport);
	Pipeline->SetShader(EShaderType::VertexShader, g_pVertexShader);
	Pipeline->SetShader(EShaderType::GeometryShader, g_pGeometryShader);
	Pipeline->SetShader(EShaderType::PixelShader, g_pPixelShader);

	return SetPipeline(Pipeline);
}



//
// IVisitor
//
bool CDebugMaterialPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return Base3DPass::Visit(Mesh, GeometryPartParams);
}

bool CDebugMaterialPass::Visit(IGeometry * Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	//const MaterialDebug* objMaterial = dynamic_cast<const MaterialDebug*>(Material);
	//if (objMaterial == nullptr)
	//	return false;

	return Base3DPass::Visit(Geometry, Material, GeometryPartParams);
}