#include "stdafx.h"

// General
#include "MaterialDebugPass.h"

// Additional
#include "Materials/MaterialDebug.h"

CMaterial_Debug_Pass::CMaterial_Debug_Pass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{}

CMaterial_Debug_Pass::~CMaterial_Debug_Pass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CMaterial_Debug_Pass::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> geometryShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_DEBUG", "VS_main");
		//geometryShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::GeometryShader, "IDB_SHADER_3D_DEBUG", "GS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_DEBUG", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	auto Pipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(disableBlending);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Wireframe);
	Pipeline->GetRasterizerState()->SetAntialiasedLineEnable(true);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->SetShader(EShaderType::VertexShader, vertexShader);
	//Pipeline->SetShader(EShaderType::GeometryShader, geometryShader);
	Pipeline->SetShader(EShaderType::PixelShader, pixelShader);

	return SetPipeline(Pipeline);
}



//
// IVisitor
//
EVisitResult CMaterial_Debug_Pass::Visit(const IModel * Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CMaterial_Debug_Pass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	//const MaterialDebug* objMaterial = dynamic_cast<const MaterialDebug*>(Material);
	//if (objMaterial == nullptr)
	//	return false;

	return Base3DPass::Visit(Geometry, Material, GeometryDrawArgs);
}