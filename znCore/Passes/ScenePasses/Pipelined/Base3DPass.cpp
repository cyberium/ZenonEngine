#include "stdafx.h"

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(IScene& scene)
    : ScenePassPipelined(scene)
{

}

Base3DPass::~Base3DPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> Base3DPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult Base3DPass::Visit(const std::shared_ptr<ISceneNode>& SceneNode)
{
	if (false == SceneNode->IsEnabled())
		return EVisitResult::Block;

	BindPerObjectData(PerObject(SceneNode->GetWorldTransfom()));
	return EVisitResult::AllowAll;
}

EVisitResult Base3DPass::Visit(const std::shared_ptr<IModel>& Model)
{
	Model->Render(GetRenderEventArgs().PipelineState->GetShaders());
	return EVisitResult::AllowAll;
}

EVisitResult Base3DPass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
	Geometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), GeometryDrawArgs);
	Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
	return EVisitResult::AllowAll;
}

