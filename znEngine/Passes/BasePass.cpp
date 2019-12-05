#include <stdafx.h>

// Include
#include <Application.h>
#include <SceneFunctional/3D/Scene3D.h>
#include <SceneFunctional/3D/SceneNode3D.h>

// General
#include "BasePass.h"

BasePass::BasePass(std::shared_ptr<Scene3D> scene, std::shared_ptr<IPipelineState> pipeline)
    : base(pipeline)
    , m_Scene(scene)
{}

BasePass::~BasePass()
{}

void BasePass::Render(RenderEventArgs& e)
{
	if (m_Scene)
	{
		m_Scene->Accept(this);
	}
}



//
// Inherited from Visitor
//
bool BasePass::Visit(SceneNode3D* node)
{
	GetRenderEventArgs()->Node = node;

	const ICamera* camera = GetRenderEventArgs()->Camera;
	if (camera)
	{
		node->UpdateCamera(camera);

		PerObject perObjectData;
		perObjectData.Model               = node->GetComponent<CTransformComponentBase>()->GetWorldTransfom();
		perObjectData.ModelView           = camera->GetViewMatrix()       * perObjectData.Model;
		perObjectData.ModelViewProjection = camera->GetProjectionMatrix() * perObjectData.ModelView;

		perObjectData.View                = camera->GetViewMatrix();
		perObjectData.Projection          = camera->GetProjectionMatrix();

		SetPerObjectConstantBufferData(perObjectData);

		return true;
	}

	return false;
}
