#include <stdafx.h>

// General
#include "Base3DPass.h"

Base3DPass::Base3DPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> scene, std::shared_ptr<IPipelineState> pipeline)
    : CBaseScenePass(RenderDevice, scene, pipeline)
{}

Base3DPass::~Base3DPass()
{}


//
// Inherited from Visitor
//
bool Base3DPass::Visit3D(ISceneNode* sceneNode)
{
	CBaseScenePass::VisitBase(sceneNode);

	const ICamera* camera = GetRenderEventArgs()->Camera;
	//const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];

	if (camera)
	{
		PerObject perObjectData;
		perObjectData.Model               = sceneNode->GetComponent<ITransformComponent>()->GetWorldTransfom();
		perObjectData.ModelView           = camera->GetViewMatrix()       * perObjectData.Model;
		perObjectData.ModelViewProjection = camera->GetProjectionMatrix() * perObjectData.ModelView;

		perObjectData.View                = camera->GetViewMatrix();
		perObjectData.Projection          = camera->GetProjectionMatrix();

		SetPerObjectConstantBufferData(perObjectData);

		return true;
	}

	return false;
}

bool Base3DPass::Visit(IMesh * Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	return CBaseScenePass::Visit(Mesh, IndexStartLocation, IndexCnt, VertexStartLocation, VertexCnt);
}
