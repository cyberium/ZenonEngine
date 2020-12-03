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

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
	}
	//vertexShader->LoadInputLayoutFromReflector();

	std::vector<SCustomInputElement> customElements;
	customElements.push_back({ 0,  0, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION,     0 });
	customElements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD,     0 });
	vertexShader->LoadInputLayoutFromCustomElements(customElements);

	// PIPELINES
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Wireframe);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

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