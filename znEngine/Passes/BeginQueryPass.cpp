#include "stdafx.h"

// General
#include "BeginQueryPass.h"

BeginQueryPass::BeginQueryPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IQuery> query)
    : AbstractPass(RenderDevice)
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
