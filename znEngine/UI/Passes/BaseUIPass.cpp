#include <stdafx.h>

#include "UI//UIBaseNode.h"

// General
#include "BaseUIPass.h"

// Additional
#include <Application.h>

BaseUIPass::BaseUIPass()
	: m_Enabled(true)
	, m_RenderDevice(_RenderDevice)
	, m_pRenderEventArgs(nullptr)
{
	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = _RenderDevice->CreateConstantBuffer(PerObject());
}

BaseUIPass::BaseUIPass(std::shared_ptr<CUIScene> uiScene, std::shared_ptr<PipelineState> pipeline)
	: m_Enabled(true)
	, m_UIScene(uiScene)
	, m_Pipeline(pipeline)
	, m_RenderDevice(_RenderDevice)
	, m_pRenderEventArgs(nullptr)
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

void BaseUIPass::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

bool BaseUIPass::IsEnabled() const
{
	return m_Enabled;
}

void BaseUIPass::RenderUI(RenderUIEventArgs& e)
{
	e.PipelineState = m_Pipeline.get();
	SetRenderEventArgs(e);

	if (m_Pipeline)
	{
		m_Pipeline->Bind();
	}

	if (m_UIScene)
	{
		m_UIScene->Accept(*this);
	}

	if (m_Pipeline)
	{
		m_Pipeline->UnBind();
	}
}



//
// Inherited from Visitor
//
bool BaseUIPass::Visit(SceneNode3D & node3D)
{
	fail1();
	return false;
}

bool BaseUIPass::Visit(CUIBaseNode & nodeUI)
{
	const Viewport* viewport = GetRenderEventArgs().Viewport;
	if (viewport)
	{
		nodeUI.UpdateViewport(viewport);

		PerObject perObjectData;
		perObjectData.Model = nodeUI.GetWorldTransfom();
		perObjectData.ModelView = mat4(1.0f);
		perObjectData.ModelViewProjection = viewport->OrthoMatrix * perObjectData.Model;

		// Update the constant buffer data
		SetPerObjectConstantBufferData(perObjectData);

		return true;
	}

	return false;
}

bool BaseUIPass::Visit(IMesh& mesh, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	if (m_pRenderEventArgs)
	{
		return mesh.Render(*m_pRenderEventArgs, m_PerObjectConstantBuffer, indexStartLocation, indexCnt, vertexStartLocation, vertexCnt);
	}

	return false;
}

bool BaseUIPass::Visit(CLight3D & light)
{
	fail1();
	return false;
}

void BaseUIPass::UpdateViewport(Viewport _viewport)
{
	m_Pipeline->GetRasterizerState().SetViewport(_viewport);
}



//
// Store render event args
//
void BaseUIPass::SetRenderEventArgs(RenderUIEventArgs& e)
{
	m_pRenderEventArgs = &e;
}
RenderUIEventArgs& BaseUIPass::GetRenderEventArgs() const
{
	assert(m_pRenderEventArgs);
	return *m_pRenderEventArgs;
}

void BaseUIPass::SetPerObjectConstantBufferData(PerObject& perObjectData)
{
	m_PerObjectConstantBuffer->Set(perObjectData);
}

std::shared_ptr<ConstantBuffer> BaseUIPass::GetPerObjectConstantBuffer() const
{
	return m_PerObjectConstantBuffer;
}

void BaseUIPass::BindPerObjectConstantBuffer(std::shared_ptr<Shader> shader)
{
	if (shader)
	{
		shader->GetShaderParameterByName("PerObject").Set(m_PerObjectConstantBuffer);
	}
}