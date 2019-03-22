#include <stdafx.h>

// General
#include "DispatchPass.h"

DispatchPass::DispatchPass(std::shared_ptr<Shader> computeShader, const glm::uvec3& numGroups)
	: m_pComputeShader(computeShader)
	, m_NumGroups(numGroups)
{}

DispatchPass::~DispatchPass()
{}

// Render the pass. This should only be called by the RenderTechnique.
void DispatchPass::PreRender(Render3DEventArgs& e)
{
	m_pComputeShader->Bind();
}

void DispatchPass::Render(Render3DEventArgs& e)
{
	m_pComputeShader->Dispatch(m_NumGroups);
}

void DispatchPass::PostRender(Render3DEventArgs& e)
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
