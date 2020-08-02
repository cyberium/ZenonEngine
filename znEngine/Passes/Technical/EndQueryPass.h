#pragma once

class ZN_API EndQueryPass 
	: public RenderPass
{
public:
    EndQueryPass(IRenderDevice& RenderDevice, IQuery* query);
    virtual ~EndQueryPass();

    virtual void Render(RenderEventArgs& e);

private:
    IQuery* m_pQuery;
};