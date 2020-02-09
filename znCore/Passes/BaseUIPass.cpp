#include "stdafx.h"

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene)
	: ScenePassPipelined(RenderDevice, Scene)
{
	m_PerObjectData = (PerObjectUI*)_aligned_malloc(sizeof(PerObjectUI), 16);
	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObjectUI());
}

BaseUIPass::~BaseUIPass()
{
	_aligned_free(m_PerObjectData);
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
bool BaseUIPass::Visit(const ISceneNodeUI* sceneNode)
{
	m_PerObjectData->Model = sceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObjectUI));

	auto& perObjectParameter = GetPipeline().GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject");
	if (perObjectParameter.IsValid() && m_PerObjectConstantBuffer != nullptr)
	{
		perObjectParameter.SetConstantBuffer(m_PerObjectConstantBuffer);
		perObjectParameter.Bind();
	}

	return true;
}

bool BaseUIPass::Visit(const IModel * Model)
{
	return Model->Render(GetRenderEventArgs());
}

bool BaseUIPass::Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	Material->Bind(GetRenderEventArgs().PipelineState->GetShaders());
	Geometry->Render(GetRenderEventArgs(), GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get(), GeometryDrawArgs);
	Material->Unbind(GetRenderEventArgs().PipelineState->GetShaders());

	return true;
}



//
// Protected
//
void BaseUIPass::FillPerFrameData()
{
	const Viewport* viewport = GetRenderEventArgs().PipelineState->GetRasterizerState()->GetViewports()[0];
	_ASSERT(viewport);

	PerFrame perFrame;
	perFrame.View = mat4(1.0f);
	perFrame.Projection = viewport->GetOrthoMatix();
	SetPerFrameData(perFrame);
}
