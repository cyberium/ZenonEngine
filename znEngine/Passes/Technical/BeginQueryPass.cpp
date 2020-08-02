#include "stdafx.h"

// General
#include "BeginQueryPass.h"

BeginQueryPass::BeginQueryPass(IRenderDevice& RenderDevice, IQuery* query)
    : RenderPass(RenderDevice)
	, m_pQuery(query)
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
