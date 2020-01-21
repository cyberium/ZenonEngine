#include "stdafx.h"

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass(IRenderDevice* RenderDevice, std::shared_ptr<IScene> Scene)
	: ScenePassPipelined(RenderDevice, Scene)
{
	m_PerObjectData = (PerObjectUI*)_aligned_malloc(sizeof(PerObjectUI), 16);
	m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObjectUI());
}

BaseUIPass::~BaseUIPass()
{
	_aligned_free(m_PerObjectData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> BaseUIPass::CreatePipeline(IRenderTarget* RenderTarget, const Viewport * Viewport)
{
	IPipelineState* UIPipeline = GetRenderDevice()->CreatePipelineState();
	UIPipeline->GetBlendState()->SetBlendMode(alphaBlending);
	UIPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	UIPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	UIPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	UIPipeline->GetRasterizerState()->SetAntialiasedLineEnable(true);
	UIPipeline->GetRasterizerState()->SetMultisampleEnabled(true);
	UIPipeline->SetRenderTarget(RenderTarget);
	UIPipeline->GetRasterizerState()->SetViewport(Viewport);

	return SetPipeline(UIPipeline);
}



//
// IVisitor
//
bool BaseUIPass::VisitUI(ISceneNode* sceneNode)
{
    GetRenderEventArgs()->Node = sceneNode;

	const ICameraComponent3D* camera = GetRenderEventArgs()->Camera;
	const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];
	
	if (viewport)
	{
		sceneNode->UpdateCamera(camera);
		sceneNode->UpdateViewport(viewport);

		m_PerObjectData->Model = sceneNode->GetWorldTransfom();
		m_PerObjectData->Projection = viewport->GetOrthoMatix();
		m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObjectUI));

		return true;
	}

	return false;
}

bool BaseUIPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	GetRenderEventArgs()->Caller = this;

	return Mesh->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer, GeometryPartParams);
}

bool BaseUIPass::Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	GetRenderEventArgs()->Caller = this;

	ShaderMap shadersMap;

	if (Material)
		shadersMap = Material->GetShaders();

	if (shadersMap.empty())
		shadersMap = GetRenderEventArgs()->PipelineState->GetShaders();

	Material->Bind(shadersMap);
	bool result = Geometry->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer, shadersMap, Material, GeometryPartParams);
	Material->Unbind(shadersMap);
	return result;
}
