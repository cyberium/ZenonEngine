#pragma once

class ZN_API EndQueryPass : public AbstractPass
{
public:
    EndQueryPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IQuery> query);
    virtual ~EndQueryPass();

    virtual void Render(RenderEventArgs& e);

private:
    std::shared_ptr<IQuery> m_pQuery;
};