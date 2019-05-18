#include <stdafx.h>

// Include
#include <Application.h>
#include <SceneFunctional/3D/Scene3D.h>
#include <SceneFunctional/3D/SceneNode3D.h>

// General
#include "BasePass.h"

BasePass::BasePass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
    : base(pipeline)
    , m_Scene(scene)
{
}

BasePass::~BasePass()
{
}

void BasePass::PreRender(RenderEventArgs& e)
{
    std::shared_ptr<PipelineState> pipelineState = GetPipelineState();
    _ASSERT(pipelineState);

	e.PipelineState = pipelineState.get();
	SetRenderEventArgs(&e);

	if (pipelineState)
	{
        pipelineState->Bind();
	}
}

void BasePass::Render(RenderEventArgs& e)
{
	if (m_Scene)
	{
		m_Scene->Accept(*this);
	}
}

void BasePass::PostRender(RenderEventArgs& e)
{
    std::shared_ptr<PipelineState> pipelineState = GetPipelineState();
    _ASSERT(pipelineState);

	if (pipelineState)
	{
        pipelineState->UnBind();
	}
}



//
// Inherited from Visitor
//
bool BasePass::Visit(std::shared_ptr<SceneNode3D> node)
{
	GetRenderEventArgs()->Node = node.get();

	const Camera* camera = GetRenderEventArgs()->Camera;
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



//
// Update viewport (need for texture resizing)
//
void BasePass::UpdateViewport(Viewport _viewport)
{
	GetPipelineState()->GetRasterizerState().SetViewport(_viewport);
}