#include <stdafx.h>

// General
#include "AbstractPass.h"

// Additional
#include "Application.h"

AbstractPass::AbstractPass()
	: m_Enabled(true)
{
	m_PerObjectData = (PerObject*)_aligned_malloc(sizeof(PerObject), 16);
	m_PerObjectConstantBuffer = _RenderDevice->CreateConstantBuffer(PerObject());
}

AbstractPass::~AbstractPass()
{
	_aligned_free(m_PerObjectData);
	_RenderDevice->DestroyConstantBuffer(m_PerObjectConstantBuffer);
}

// Enable or disable the pass. If a pass is disabled, the technique will skip it.
void AbstractPass::SetEnabled(bool enabled)
{
	m_Enabled = enabled;
}

bool AbstractPass::IsEnabled() const
{
	return m_Enabled;
}

// Render the pass. This should only be called by the RenderTechnique.
void AbstractPass::PreRender(Render3DEventArgs& e)
{}

void AbstractPass::PostRender(Render3DEventArgs& e)
{}


// Inherited from Visitor

bool AbstractPass::Visit(SceneNode3D& node)
{
	return false;
}

bool AbstractPass::Visit(CUIBaseNode & nodeUI)
{
	fail1();
	return false;
}

bool AbstractPass::Visit(IMesh& mesh, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	return false;
}

bool AbstractPass::Visit(CLight3D& light)
{
	return false;
}

void AbstractPass::UpdateViewport(Viewport _viewport)
{
	// do nothing
}

void AbstractPass::SetRenderEventArgs(Render3DEventArgs& e)
{
	fail1();
}

Render3DEventArgs& AbstractPass::GetRenderEventArgs() const
{
	fail1();
}

void AbstractPass::SetPerObjectConstantBufferData(PerObject& perObjectData)
{
	m_PerObjectConstantBuffer->Set(perObjectData);
}

std::shared_ptr<ConstantBuffer> AbstractPass::GetPerObjectConstantBuffer() const
{
	return m_PerObjectConstantBuffer;
}

void AbstractPass::BindPerObjectConstantBuffer(std::shared_ptr<Shader> shader)
{
	if (shader)
	{
		shader->GetShaderParameterByName("PerObject").Set(m_PerObjectConstantBuffer);
	}
}
