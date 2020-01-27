#include "stdafx.h"

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(IRenderDevice* RenderDevice, std::shared_ptr<IScene> scene)
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
// IVisitor
//
bool Base3DPass::Visit(ISceneNode3D* sceneNode)
{
	const ICameraComponent3D* camera = GetRenderEventArgs()->Camera;
	if (camera)
		sceneNode->UpdateCamera(camera);

	const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];
	if (viewport)
		sceneNode->UpdateViewport(viewport);

	m_PerObjectData->Model = sceneNode->GetWorldTransfom();
	m_PerObjectConstantBuffer->Set(m_PerObjectData, sizeof(PerObject3D));

	return true;
}

bool Base3DPass::Visit(IMesh * Mesh, SGeometryPartParams GeometryPartParams)
{
	return Mesh->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer, GeometryPartParams);
}

bool Base3DPass::Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
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
