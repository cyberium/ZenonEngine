#include <stdafx.h>

// General
#include "EndQueryPass.h"

EndQueryPass::EndQueryPass( std::shared_ptr<Query> query )
    : m_pQuery( query )
{}

EndQueryPass::~EndQueryPass()
{}

void EndQueryPass::Render( Render3DEventArgs& e )
{
    if ( m_pQuery )
    {
        m_pQuery->End( e.FrameCounter );
    }
}
