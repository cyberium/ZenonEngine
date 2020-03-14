#include "stdafx.h"

// General
#include "RenderTechnique.h"

RenderTechnique::RenderTechnique()
{}

RenderTechnique::~RenderTechnique()
{}



//
// IRenderTechnique
//
uint32 RenderTechnique::AddPass(std::shared_ptr<IRenderPass> pass)
{
	_ASSERT_EXPR(pass, L"Pass must not be nullptr.");
	m_Passes.push_back(pass);
	return static_cast<uint32>(m_Passes.size()) - 1;
}

IRenderPass* RenderTechnique::GetPass(uint32 ID) const
{
	if (ID < m_Passes.size())
	{
		return m_Passes[ID].get();
	}

	return nullptr;
}

void RenderTechnique::Render(RenderEventArgs& renderEventArgs)
{
	for (auto pass : m_Passes)
	{
		if (pass && pass->IsEnabled())
		{
			pass->PreRender(renderEventArgs);
			pass->Render(renderEventArgs);
			pass->PostRender(renderEventArgs);
		}
	}
}

