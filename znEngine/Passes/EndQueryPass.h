#pragma once

#include "AbstractPass.h"

class OW_ENGINE_API EndQueryPass : public AbstractPass
{
public:
    EndQueryPass(std::shared_ptr<Query> query);
    virtual ~EndQueryPass();

    virtual void Render(RenderEventArgs& e);

private:
    std::shared_ptr<Query> m_pQuery;
};