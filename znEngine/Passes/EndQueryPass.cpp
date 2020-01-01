#include "stdafx.h"

// General
#include "EndQueryPass.h"

EndQueryPass::EndQueryPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IQuery> query)
    : AbstractPass(RenderDevice)
	, m_pQuery(query)
{}

EndQueryPass::~EndQueryPass()
{}

void EndQueryPass::Render(RenderEventArgs& e)
{
    if (m_pQuery)
    {
        m_pQuery->End(e.FrameCounter);
    }
}
