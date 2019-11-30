#pragma once

#include "AbstractPass.h"

class OW_ENGINE_API BeginQueryPass : public AbstractPass
{
public:
    BeginQueryPass( std::shared_ptr<IQuery> query );
    virtual ~BeginQueryPass();

    virtual void Render( RenderEventArgs& e );

private:
    std::shared_ptr<IQuery> m_pQuery;
};