#include <stdafx.h>

// General
#include "RenderTechnique.h"

RenderTechnique::RenderTechnique()
{}

RenderTechnique::~RenderTechnique()
{}

unsigned int RenderTechnique::AddPass(std::shared_ptr<IRenderPass> pass)
{
	// No check for duplicate passes (it may be intended to render the same pass multiple times?)
	m_Passes.push_back(pass);
	return static_cast<unsigned int>(m_Passes.size()) - 1;
}

std::shared_ptr<IRenderPass> RenderTechnique::GetPass(unsigned int ID) const
{
	if (ID < m_Passes.size())
	{
		return m_Passes[ID];
	}

	return std::shared_ptr<IRenderPass>();
}

// Render the scene using the passes that have been configured.
void RenderTechnique::Render(Render3DEventArgs& renderEventArgs)
{
	for (auto pass : m_Passes)
	{
		if (pass->IsEnabled())
		{
			pass->PreRender(renderEventArgs);
			pass->Render(renderEventArgs);
			pass->PostRender(renderEventArgs);
		}
	}
}

void RenderTechnique::UpdateViewport(Viewport _viewport)
{
	for (auto pass : m_Passes)
	{
		pass->UpdateViewport(_viewport);
	}
}


