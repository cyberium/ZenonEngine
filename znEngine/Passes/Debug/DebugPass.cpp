#include "stdafx.h"

// General
#include "DebugPass.h"

// Additional
#include "Materials/MaterialDebug.h"

CDebugPass::CDebugPass(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
{}

CDebugPass::~CDebugPass()
{}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDebugPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	{
		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		std::vector<SCustomInputElement> customElements;
		customElements.push_back({ 0,  0, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION,     0 });
		customElements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD,     0 });
		vertexShader->LoadInputLayoutFromCustomElements(customElements);
		GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	}

	{
		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
		GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	}

	// PIPELINES
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableTestDisableWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Wireframe);
	
	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CDebugPass::Visit(const std::shared_ptr<ISceneNode>& SceneNode)
{
	if (SceneNode->GetClass() != cSceneNode3D && SceneNode->GetClass() != cSceneNodeRTSPoint && SceneNode->GetClass() != cSceneNodeRTSBullet)
		return EVisitResult::AllowVisitChilds;
	return Base3DPass::Visit(SceneNode);
}

EVisitResult CDebugPass::Visit(const std::shared_ptr<IModel>& Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CDebugPass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	auto objMaterial = std::dynamic_pointer_cast<MaterialDebug>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	return Base3DPass::Visit(Geometry, Material, GeometryDrawArgs);
}