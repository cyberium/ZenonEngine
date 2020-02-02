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
bool BaseUIPass::Visit(ISceneNodeUI* sceneNode)
{
	m_PerObjectData->Model = sceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObjectUI));

	return true;
}

bool BaseUIPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return Mesh->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer, GeometryPartParams);
}

bool BaseUIPass::Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	ShaderMap shadersMap;

	if (Material)
		shadersMap = Material->GetShaders();

	if (shadersMap.empty())
		shadersMap = GetRenderEventArgs().PipelineState->GetShaders();

	Material->Bind(shadersMap);
	bool result = Geometry->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer, shadersMap, Material, GeometryPartParams);
	Material->Unbind(shadersMap);
	return result;
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
