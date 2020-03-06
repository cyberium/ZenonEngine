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
std::shared_ptr<IRenderPassPipelined> CDrawBoundingBoxPass::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateBBox();

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::VertexShader, "IDB_SHADER_3D_DEBUG", "VS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().CreateShader(EShaderType::PixelShader, "IDB_SHADER_3D_DEBUG", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	auto Pipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	Pipeline->GetBlendState()->SetBlendMode(disableBlending);
	Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe);
	Pipeline->GetRasterizerState()->SetMultisampleEnabled(true);
	Pipeline->GetRasterizerState()->SetAntialiasedLineEnable(true);
	Pipeline->SetRenderTarget(RenderTarget);
	Pipeline->GetRasterizerState()->SetViewport(Viewport);
	Pipeline->SetShader(EShaderType::VertexShader, vertexShader);
	Pipeline->SetShader(EShaderType::PixelShader, pixelShader);

	return SetPipeline(Pipeline);
}



//
// IVisitor
//
bool CDrawBoundingBoxPass::Visit(const ISceneNode3D * SceneNode3D)
{
	const std::shared_ptr<IColliderComponent3D>& colliderComponent = SceneNode3D->GetColliderComponent();
	if (colliderComponent == nullptr)
		return false;

	if (! colliderComponent->GetDebugDrawMode())
		return false;

	BoundingBox bbox = colliderComponent->GetWorldBounds();

	glm::mat4 bboxMatrix = glm::mat4(1.0f);
	bboxMatrix = glm::translate(bboxMatrix, bbox.getMin());
	bboxMatrix = glm::scale(bboxMatrix, bbox.getMax() - bbox.getMin());

	PerObject3D perObject3D;
	perObject3D.Model = bboxMatrix;
	m_PerObjectConstantBuffer->Set(perObject3D);

	const IShader* vertexShader = GetPipeline().GetShaders().at(EShaderType::VertexShader).get();

	if (m_PerObjectParameter == nullptr)
		m_PerObjectParameter = &(vertexShader->GetShaderParameterByName("PerObject"));

	if (m_PerObjectParameter->IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		m_PerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
		m_PerObjectParameter->Bind();
	}

	m_QuadGeometry->Render(GetRenderEventArgs(), vertexShader);

	return true;
}

bool CDrawBoundingBoxPass::Visit(const IModel * Model)
{
	return false;
}

bool CDrawBoundingBoxPass::Visit(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return false;
}
