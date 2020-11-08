#include "stdafx.h"

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass(IScene& Scene)
	: ScenePassPipelined(Scene)
	, m_PerObjectParameter(nullptr)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

BaseUIPass::~BaseUIPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> BaseUIPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	return shared_from_this();
}



//
// IVisitor
//
EVisitResult BaseUIPass::Visit(const IUIControl* sceneNode)
{
	PerObject perObject;
	perObject.Model = sceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(perObject);

	if (m_PerObjectParameter == nullptr)
		m_PerObjectParameter = &(GetPipeline().GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject"));

	if (m_PerObjectParameter->IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		m_PerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
		m_PerObjectParameter->Bind();
	}

	return EVisitResult::AllowAll;
}

EVisitResult BaseUIPass::Visit(const IModel * Model)
{
	Model->Render();
	return EVisitResult::AllowAll;
}

EVisitResult BaseUIPass::Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
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
void BaseUIPass::FillPerFrameData()
{
	const Viewport& viewport = GetRenderEventArgs().PipelineState->GetRenderTarget()->GetViewport();

	PerFrame perFrame(
		glm::mat4(1.0f), 
		viewport.GetOrthoMatix(), 
		glm::vec2(viewport.GetSize())
	);
	SetPerFrameData(perFrame);
}
