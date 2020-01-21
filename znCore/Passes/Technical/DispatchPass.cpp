#include "stdafx.h"

// General
#include "DispatchPass.h"

DispatchPass::DispatchPass(IRenderDevice* RenderDevice, IShader* computeShader, const glm::uvec3& numGroups)
	: RenderPass(RenderDevice)
	, m_pComputeShader(computeShader)
	, m_NumGroups(numGroups)
{}

DispatchPass::~DispatchPass()
{}

void DispatchPass::PreRender(RenderEventArgs& e)
{
	m_pComputeShader->Bind();
}

void DispatchPass::Render(RenderEventArgs& e)
{
	m_pComputeShader->Dispatch(m_NumGroups);
}

void DispatchPass::PostRender(RenderEventArgs& e)
{
	m_pComputeShader->UnBind();
}

void DispatchPass::SetNumGroups(const glm::ivec3& numGroups)
{
	m_NumGroups = numGroups;
}

glm::ivec3 DispatchPass::GetNumGroups() const
{
	return m_NumGroups;
}
