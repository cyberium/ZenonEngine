#include <stdafx.h>

// General
#include "BeginQueryPass.h"

BeginQueryPass::BeginQueryPass( std::shared_ptr<Query> query )
    : m_pQuery( query )
{}

BeginQueryPass::~BeginQueryPass()
{}

void BeginQueryPass::Render( Render3DEventArgs& e )
{
    if ( m_pQuery )
    {
        m_pQuery->Begin( e.FrameCounter );
    }
}
