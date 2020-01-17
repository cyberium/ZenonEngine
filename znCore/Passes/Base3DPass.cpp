#include "stdafx.h"

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> scene)
    : ScenePassPipelined(RenderDevice, scene)
{
	m_PerObjectData = (PerObject3D*)_aligned_malloc(sizeof(PerObject3D), 16);
	m_PerObjectConstantBuffer = GetRenderDevice()->CreateConstantBuffer(PerObject3D());
}

Base3DPass::~Base3DPass()
{
	_aligned_free(m_PerObjectData);
	GetRenderDevice()->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}


//
// Inherited from Visitor
//
bool Base3DPass::Visit3D(ISceneNode* sceneNode)
{
	ScenePassPipelined::VisitBase(sceneNode);


	m_PerObjectData->Model               = sceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObject3D));

	return true;
}

bool Base3DPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	GetRenderEventArgs()->Caller = this;

	return Mesh->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer.get(), GeometryPartParams);
}

bool Base3DPass::Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	GetRenderEventArgs()->Caller = this;

	ShaderMap shadersMap;

	if (Material)
		shadersMap = Material->GetShaders();

	if (shadersMap.empty())
		shadersMap = GetRenderEventArgs()->PipelineState->GetShaders();

	Material->Bind(shadersMap);
	bool result = Geometry->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer.get(), shadersMap, Material, GeometryPartParams);
	Material->Unbind(shadersMap);

	return result;
}
