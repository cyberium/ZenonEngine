#include "stdafx.h"

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass(IRenderDevice& RenderDevice, const std::shared_ptr<IScene>& Scene)
	: ScenePassPipelined(RenderDevice, Scene)
{
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObjectUI());
}

BaseUIPass::~BaseUIPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> BaseUIPass::CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	std::shared_ptr<IPipelineState> UIPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
	UIPipeline->GetBlendState()->SetBlendMode(alphaBlending);
	UIPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	UIPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	UIPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	UIPipeline->GetRasterizerState()->SetAntialiasedLineEnable(false);
	UIPipeline->GetRasterizerState()->SetMultisampleEnabled(false);
	UIPipeline->SetRenderTarget(RenderTarget);
	UIPipeline->GetRasterizerState()->SetViewport(Viewport);

	return SetPipeline(UIPipeline);
}



//
// IVisitor
//
EVisitResult BaseUIPass::Visit(const ISceneNodeUI* sceneNode)
{
	PerObjectUI perObjectUI;
	perObjectUI.Model = sceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(perObjectUI);

	auto& perObjectParameter = GetPipeline().GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject");
	if (perObjectParameter.IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		perObjectParameter.SetConstantBuffer(m_PerObjectConstantBuffer);
		perObjectParameter.Bind();
	}

	return EVisitResult::AllowAll;
}

EVisitResult BaseUIPass::Visit(const IModel * Model)
{
	Model->Render(GetRenderEventArgs());
	return EVisitResult::AllowAll;
}

EVisitResult BaseUIPass::Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	Material->Bind(GetRenderEventArgs().PipelineState->GetShaders());
	Geometry->Render(GetRenderEventArgs(), GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get(), GeometryDrawArgs);
	Material->Unbind(GetRenderEventArgs().PipelineState->GetShaders());

	return EVisitResult::AllowAll;
}



//
// Protected
//
void BaseUIPass::FillPerFrameData()
{
	const Viewport* viewport = GetRenderEventArgs().PipelineState->GetRasterizerState()->GetViewports()[0];
	_ASSERT(viewport);

	PerFrame perFrame;
	perFrame.View = glm::mat4(1.0f);
	perFrame.Projection = viewport->GetOrthoMatix();
	SetPerFrameData(perFrame);
}
