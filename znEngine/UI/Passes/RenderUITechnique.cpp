#include <stdafx.h>

// General
#include "RenderUITechnique.h"

RenderUITechnique::RenderUITechnique()
{}

RenderUITechnique::~RenderUITechnique()
{}

unsigned int RenderUITechnique::AddPass(std::shared_ptr<IRenderUIPass> pass)
{
    // No check for duplicate passes (it may be intended to render the same pass multiple times?)
    m_Passes.push_back(pass);
    return static_cast<unsigned int>(m_Passes.size()) - 1;
}

std::shared_ptr<IRenderUIPass> RenderUITechnique::GetPass(unsigned int ID) const
{
    if (ID < m_Passes.size())
    {
        return m_Passes[ID];
    }

    return std::shared_ptr<IRenderUIPass>();
}

// Render the scene using the passes that have been configured.
void RenderUITechnique::RenderUI(RenderUIEventArgs& renderEventArgs)
{
    for (auto pass : m_Passes)
    {
        if (pass->IsEnabled())
        {
            pass->RenderUI(renderEventArgs);
        }
    }
}

void RenderUITechnique::UpdateViewport(Viewport _viewport)
{
    for (auto pass : m_Passes)
    {
        if (pass->IsEnabled())
        {
            pass->UpdateViewport(_viewport);
        }
    }
}


