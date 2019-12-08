#include <stdafx.h>

// General
#include "BaseUIPass.h"

BaseUIPass::BaseUIPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> pipeline)
	: CBaseScenePass(RenderDevice, Scene, pipeline)
{}

BaseUIPass::~BaseUIPass()
{}

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

		PerObject perObjectData;
		perObjectData.Model = sceneNode->GetComponent<ITransformComponent>()->GetWorldTransfom();
		perObjectData.ModelView = mat4(1.0f);
		perObjectData.ModelViewProjection = viewport->GetOrthoMatix() * perObjectData.Model;

		SetPerObjectConstantBufferData(perObjectData);

		return true;
	}

	return false;
}
