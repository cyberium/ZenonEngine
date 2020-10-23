#include "stdafx.h"

// General
#include "DrawBoundingBoxPass.h"

CDrawBoundingBoxPass::CDrawBoundingBoxPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> scene)
	: Base3DPass(RenderDevice, scene)
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
	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateBBox();

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
	GetPipeline().GetRasterizerState()->SetMultisampleEnabled(true);
	GetPipeline().GetRasterizerState()->SetAntialiasedLineEnable(true);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CDrawBoundingBoxPass::Visit(const ISceneNode3D * SceneNode3D)
{
	const std::shared_ptr<IColliderComponent3D>& colliderComponent = SceneNode3D->GetComponent<IColliderComponent3D>();
	if (colliderComponent == nullptr)
		return EVisitResult::AllowVisitChilds;

	if (! colliderComponent->GetDebugDrawMode())
		return EVisitResult::AllowVisitChilds;

	BoundingBox bbox = colliderComponent->GetWorldBounds();

	glm::mat4 bboxMatrix = glm::mat4(1.0f);
	bboxMatrix = glm::translate(bboxMatrix, bbox.getMin());
	bboxMatrix = glm::scale(bboxMatrix, bbox.getMax() - bbox.getMin());

	PerObject perObject;
	perObject.Model = bboxMatrix;
	m_PerObjectConstantBuffer->Set(perObject);

	const IShader* vertexShader = GetPipeline().GetShaders().at(EShaderType::VertexShader).get();

	if (m_PerObjectParameter == nullptr)
		m_PerObjectParameter = &(vertexShader->GetShaderParameterByName("PerObject"));

	if (m_PerObjectParameter->IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		m_PerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
		m_PerObjectParameter->Bind();
	}

	m_QuadGeometry->Render(GetRenderEventArgs(), vertexShader);

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
