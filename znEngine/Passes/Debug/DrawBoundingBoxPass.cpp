#include "stdafx.h"

// General
#include "DrawBoundingBoxPass.h"

// Additional
#include "Materials/MaterialDebug.h"

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
std::shared_ptr<IRenderPassPipelined> CDrawBoundingBoxPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_BBoxGeometry = GetRenderDevice().GetPrimitivesFactory().CreateBBox();
	m_Material = MakeShared(MaterialDebug, GetRenderDevice());

	{
		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		vertexShader->LoadInputLayoutFromReflector();
		GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	}

	{
		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
		GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	}

	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Solid);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CDrawBoundingBoxPass::Visit(const std::shared_ptr<ISceneNode>& CSceneNode)
{
	std::shared_ptr<IColliderComponent> colliderComponent = CSceneNode->GetComponentT<IColliderComponent>();
	if (colliderComponent == nullptr)
		return EVisitResult::AllowVisitChilds;

	if (false == colliderComponent->GetDebugDrawMode())
		return EVisitResult::AllowVisitChilds;

	BoundingBox bbox = colliderComponent->GetWorldBounds();

	glm::mat4 bboxMatrix = glm::mat4(1.0f);
	bboxMatrix = glm::translate(bboxMatrix, bbox.getMin());
	bboxMatrix = glm::scale(bboxMatrix, bbox.getMax() - bbox.getMin());
	BindPerObjectData(PerObject(bboxMatrix));

	m_Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
	m_BBoxGeometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr());
	m_Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());

	return EVisitResult::AllowAll;
}

EVisitResult CDrawBoundingBoxPass::Visit(const std::shared_ptr<IModel>& Model)
{
	return EVisitResult::Block;
}

EVisitResult CDrawBoundingBoxPass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}
