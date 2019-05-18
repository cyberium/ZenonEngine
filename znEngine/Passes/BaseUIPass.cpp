#include <stdafx.h>

#include "SceneFunctional/UI/SceneNodeUI.h"

// General
#include "BaseUIPass.h"

// Additional
#include <Application.h>

BaseUIPass::BaseUIPass(std::shared_ptr<SceneUI> uiScene, std::shared_ptr<PipelineState> pipeline)
	: base(pipeline)
    , m_UIScene(uiScene)
{
	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = _RenderDevice->CreateConstantBuffer(PerObject());
}

BaseUIPass::~BaseUIPass()
{
	if (m_PerObjectData)
	{
		_aligned_free(m_PerObjectData);
		_RenderDevice->DestroyConstantBuffer(m_PerObjectConstantBuffer);
	}
}

void BaseUIPass::PreRender(RenderEventArgs & e)
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

void BaseUIPass::Render(RenderEventArgs & e)
{
    if (m_UIScene)
    {
        m_UIScene->Accept(*this);
    }
}

void BaseUIPass::PostRender(RenderEventArgs & e)
{
    std::shared_ptr<PipelineState> pipelineState = GetPipelineState();
    _ASSERT(pipelineState);

    if (pipelineState)
    {
        pipelineState->UnBind();
    }
}



//
// IVisitor
//
bool BaseUIPass::Visit(std::shared_ptr<CUIBaseNode> nodeUI)
{
    GetRenderEventArgs()->Node = nodeUI.get();

	const Viewport* viewport = GetRenderEventArgs()->Viewport;
	if (viewport)
	{
		nodeUI->UpdateViewport(viewport);

		PerObject perObjectData;
		perObjectData.Model = nodeUI->GetComponent<CTransformComponentBase>()->GetWorldTransfom();
		perObjectData.ModelView = mat4(1.0f);
		perObjectData.ModelViewProjection = viewport->OrthoMatrix * perObjectData.Model;

		SetPerObjectConstantBufferData(perObjectData);

		return true;
	}

	return false;
}



//
// Update viewport (need for texture resizing)
//
void BaseUIPass::UpdateViewport(Viewport _viewport)
{
	GetPipelineState()->GetRasterizerState().SetViewport(_viewport);
}
