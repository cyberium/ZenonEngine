#include "stdafx.h"

// Include
#include <Application.h>
#include <3D//Scene3D.h>
#include <3D//SceneNode3D.h>

// General
#include "FontPass.h"

CFontPass::CFontPass()
	: m_pRenderEventArgs(nullptr)
	, m_RenderDevice(_RenderDevice)
{

}

CFontPass::CFontPass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
	: m_pRenderEventArgs(nullptr)
	, m_Scene(scene)
	, m_Pipeline(pipeline)
	, m_RenderDevice(_RenderDevice)
{

}

CFontPass::~CFontPass()
{
}


void CFontPass::PreRender(Render3DEventArgs& e)
{
	e.PipelineState = m_Pipeline.get();
	SetRenderEventArgs(e);

	if (m_Pipeline)
	{
		m_Pipeline->Bind();
	}
}

void CFontPass::Render(Render3DEventArgs& e)
{
	if (m_Scene)
	{
		m_Scene->Accept(*this);
	}
}

void CFontPass::PostRender(Render3DEventArgs& e)
{
	if (m_Pipeline)
	{
		m_Pipeline->UnBind();
	}
}

// Inherited from Visitor

bool CFontPass::Visit(SceneNode3D& node)
{
	Object& nodeAsObject = reinterpret_cast<Object&>(node);
	m_pRenderEventArgs->Node = &nodeAsObject;

	const Camera* camera = GetRenderEventArgs().Camera;
	if (camera)
	{
		node.UpdateCamera(camera);

		PerObject perObjectData;
		perObjectData.Model = node.GetWorldTransfom();
		perObjectData.ModelView = camera->GetViewMatrix()       * perObjectData.Model;
		perObjectData.ModelViewProjection = camera->GetProjectionMatrix() * perObjectData.ModelView;
		//perObjectData.Model = node.GetWorldTransfom();
		//perObjectData.ModelView = perObjectData.Model;
		//perObjectData.ModelViewProjection = camera->GetViewport().OrthoMatrix * perObjectData.ModelView;

		perObjectData.View = mat4(1.0f);
		perObjectData.Projection = camera->GetViewport().OrthoMatrix;

		// Update the constant buffer data
		SetPerObjectConstantBufferData(perObjectData);

		return true;
	}

	return false;
}

bool CFontPass::Visit(IMesh& mesh, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	if (m_pRenderEventArgs && mesh.GetType() == SN_TYPE_FONT)
	{
		return mesh.Render(*m_pRenderEventArgs, m_PerObjectConstantBuffer, indexStartLocation, indexCnt, vertexStartLocation, vertexCnt);
	}

	return false;
}

bool CFontPass::Visit(CLight3D& light)
{
	return false;
}

void CFontPass::UpdateViewport(Viewport _viewport)
{
	m_Pipeline->GetRasterizerState().SetViewport(_viewport);
}

//----------------------------------------------------------------------

void CFontPass::SetRenderEventArgs(Render3DEventArgs& e)
{
	m_pRenderEventArgs = &e;
}

Render3DEventArgs& CFontPass::GetRenderEventArgs() const
{
	assert(m_pRenderEventArgs);
	return *m_pRenderEventArgs;
}

//----------------------------------------------------------------------

std::shared_ptr<IRenderDevice> CFontPass::GetRenderDevice() const
{
	return m_RenderDevice.lock();
}

std::shared_ptr<PipelineState> CFontPass::GetPipelineState() const
{
	return m_Pipeline;
}