#include "stdafx.h"

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(IScene& scene)
    : ScenePassPipelined(scene)
	, m_PerObjectParameter(nullptr)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

Base3DPass::~Base3DPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> Base3DPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	return shared_from_this();
}



//
// IVisitor
//
EVisitResult Base3DPass::Visit(const ISceneNode* SceneNode)
{
	BindPerObjectParameter(SceneNode->GetWorldTransfom());
	return EVisitResult::AllowAll;
}

EVisitResult Base3DPass::Visit(const IModel * Model)
{
	Model->Render();
	return EVisitResult::AllowAll;
}

EVisitResult Base3DPass::Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const auto& shaders = GetPipeline().GetShaders();
	const auto& vertexShader = shaders.at(EShaderType::VertexShader).get();

	Material->Bind(shaders);
	Geometry->Render(vertexShader, GeometryDrawArgs);
	Material->Unbind(shaders);
	return EVisitResult::AllowAll;
}



//
// Protected
//
void Base3DPass::BindPerObjectParameter(const glm::mat4& PerObjectMatrix)
{
	PerObject perObject;
	perObject.Model = PerObjectMatrix;
	m_PerObjectConstantBuffer->Set(perObject);

	if (m_PerObjectParameter == nullptr)
		m_PerObjectParameter = &(GetPipeline().GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject"));

	if (false == m_PerObjectParameter->IsValid())
		throw CException("PerObject parameter is invalid.");

	m_PerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
	m_PerObjectParameter->Bind();
}
