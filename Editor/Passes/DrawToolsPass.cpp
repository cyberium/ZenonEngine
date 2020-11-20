	#include "stdafx.h"

// General
#include "DrawToolsPass.h"

// Additional
#include "Materials/MaterialEditorTool.h"

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
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CDrawToolsPass::Visit(const IModel * Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CDrawToolsPass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const MaterialEditorTool* objMaterial = dynamic_cast<const MaterialEditorTool*>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	return Base3DPass::Visit(Geometry, Material, GeometryDrawArgs);
}