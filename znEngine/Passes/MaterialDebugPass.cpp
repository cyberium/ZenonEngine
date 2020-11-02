#include "stdafx.h"

// General
#include "MaterialDebugPass.h"

// Additional
#include "Materials/MaterialDebug.h"

CMaterial_Debug_Pass::CMaterial_Debug_Pass(IRenderDevice& RenderDevice, std::weak_ptr<IScene> Scene)
	: Base3DPass(RenderDevice, Scene)
{}

CMaterial_Debug_Pass::~CMaterial_Debug_Pass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CMaterial_Debug_Pass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> geometryShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		//geometryShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::GeometryShader, "IDB_SHADER_3D_DEBUG", "GS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableTestDisableWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Wireframe);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	//GetPipeline().SetShader(EShaderType::GeometryShader, geometryShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CMaterial_Debug_Pass::Visit(const ISceneNode3D * SceneNode)
{
	if (SceneNode->GetClass() != cSceneNode3D)
		return EVisitResult::AllowVisitChilds;
	return Base3DPass::Visit(SceneNode);
}

EVisitResult CMaterial_Debug_Pass::Visit(const IModel * Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CMaterial_Debug_Pass::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const MaterialDebug* objMaterial = dynamic_cast<const MaterialDebug*>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	return Base3DPass::Visit(Geometry, Material, GeometryDrawArgs);
}