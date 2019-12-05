#include <stdafx.h>

#include "SceneFunctional/UI/SceneNodeUI.h"

// General
#include "BaseUIPass.h"

// Additional
#include <Application.h>

BaseUIPass::BaseUIPass(std::shared_ptr<SceneUI> uiScene, std::shared_ptr<IPipelineState> pipeline)
	: base(pipeline)
    , m_UIScene(uiScene)
{}

BaseUIPass::~BaseUIPass()
{}

void BaseUIPass::Render(RenderEventArgs & e)
{
    if (m_UIScene)
    {
        m_UIScene->Accept(this);
    }
}



//
// IVisitor
//
bool BaseUIPass::Visit(CUIBaseNode* nodeUI)
{
    GetRenderEventArgs()->Node = nodeUI;

	const Viewport* viewport = GetRenderEventArgs()->PipelineState->GetRasterizerState()->GetViewports()[0];
	if (viewport)
	{
		nodeUI->UpdateViewport(viewport);

		PerObject perObjectData;
		perObjectData.Model = nodeUI->GetComponent<CTransformComponentBase>()->GetWorldTransfom();
		perObjectData.ModelView = mat4(1.0f);
		perObjectData.ModelViewProjection = viewport->GetOrthoMatix() * perObjectData.Model;

		SetPerObjectConstantBufferData(perObjectData);

		return true;
	}

	return false;
}
