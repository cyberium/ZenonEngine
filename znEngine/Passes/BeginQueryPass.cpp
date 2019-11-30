#include <stdafx.h>

// General
#include "BeginQueryPass.h"

BeginQueryPass::BeginQueryPass(std::shared_ptr<IQuery> query)
    : m_pQuery(query)
{}

BeginQueryPass::~BeginQueryPass()
{}

void BeginQueryPass::Render(RenderEventArgs& e)
{
    if (m_pQuery)
    {
        m_pQuery->Begin(e.FrameCounter);
    }
}
