#include "stdafx.h"

// General
#include "DrawBoundingBoxPass.h"

CDrawBoundingBoxPass::CDrawBoundingBoxPass(IRenderDevice& RenderDevice, IScene& scene)
	: Base3DPass(scene)
{
}

CDrawBoundingBoxPass::~CDrawBoundingBoxPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawBoundingBoxPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_BBoxGeometry = GetRenderDevice().GetPrimitivesFactory().CreateBBox();

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CDrawBoundingBoxPass::Visit(const ISceneNode * CSceneNode)
{
	const std::shared_ptr<IColliderComponent3D>& colliderComponent = CSceneNode->GetComponentT<IColliderComponent3D>();
	if (colliderComponent == nullptr)
		return EVisitResult::AllowVisitChilds;

	if (false == colliderComponent->GetDebugDrawMode())
		return EVisitResult::AllowVisitChilds;

	BoundingBox bbox = colliderComponent->GetWorldBounds();

	glm::mat4 bboxMatrix = glm::mat4(1.0f);
	bboxMatrix = glm::translate(bboxMatrix, bbox.getMin());
	bboxMatrix = glm::scale(bboxMatrix, bbox.getMax() - bbox.getMin());

	BindPerObjectParameter(bboxMatrix);

	const IShader* vertexShader = GetPipeline().GetShaders().at(EShaderType::VertexShader).get();
	m_BBoxGeometry->Render(vertexShader);

	return EVisitResult::AllowAll;
}

EVisitResult CDrawBoundingBoxPass::Visit(const IModel * Model)
{
	return EVisitResult::Block;
}

EVisitResult CDrawBoundingBoxPass::Visit(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}
