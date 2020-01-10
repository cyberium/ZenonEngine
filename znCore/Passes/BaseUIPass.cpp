#include "stdafx.h"

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> pipeline)
	: CBaseScenePass(RenderDevice, Scene, pipeline)
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
// IVisitor
//
bool BaseUIPass::VisitUI(ISceneNode* sceneNode)
{
    GetRenderEventArgs()->Node = sceneNode;

	const ICamera* camera = GetRenderEventArgs()->Camera;
	const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];
	
	if (viewport)
	{
		sceneNode->UpdateCamera(camera);
		sceneNode->UpdateViewport(viewport);

		m_PerObjectData->Model = sceneNode->GetWorldTransfom();
		m_PerObjectData->Projection = viewport->GetOrthoMatix();

		SetPerObjectConstantBufferData();

		return true;
	}

	return false;
}

bool BaseUIPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	GetRenderEventArgs()->Caller = this;

	return Mesh->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer.get(), GeometryPartParams);
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
	bool result = Geometry->Render(GetRenderEventArgs(), GetPerObjectConstantBuffer().get(), shadersMap, Material, GeometryPartParams);
	Material->Unbind(shadersMap);
	return result;
}



//
// PerObject functional
//
void BaseUIPass::SetPerObjectConstantBufferData()
{
	m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObjectUI));
}

std::shared_ptr<IConstantBuffer> BaseUIPass::GetPerObjectConstantBuffer() const
{
	return m_PerObjectConstantBuffer;
}
