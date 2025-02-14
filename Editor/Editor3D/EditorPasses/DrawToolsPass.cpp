	#include "stdafx.h"

// General
#include "DrawToolsPass.h"

// Additional
#include "Editor3D/EditorMaterials/MaterialEditorTool.h"

CDrawToolsPass::CDrawToolsPass(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
{}

CDrawToolsPass::~CDrawToolsPass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawToolsPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(vertexShader);
	GetPipeline().SetShader(pixelShader);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CDrawToolsPass::Visit(const std::shared_ptr<IModel>& Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CDrawToolsPass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	auto objMaterial = std::dynamic_pointer_cast<MaterialEditorTool>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	return Base3DPass::Visit(Geometry, Material, GeometryDrawArgs);
}