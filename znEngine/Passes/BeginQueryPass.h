#pragma once

#include "AbstractPass.h"

class BeginQueryPass : public AbstractPass
{
public:
    BeginQueryPass( std::shared_ptr<Query> query );
    virtual ~BeginQueryPass();

    virtual void Render( RenderEventArgs& e );

private:
    std::shared_ptr<Query> m_pQuery;
};