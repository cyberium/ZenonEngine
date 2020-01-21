#pragma once

#include "../RenderPass.h"

class ZN_API BeginQueryPass 
	: public RenderPass
{
public:
	BeginQueryPass(IRenderDevice* RenderDevice, IQuery* query);
	virtual ~BeginQueryPass();

	virtual void Render(RenderEventArgs& e);

private:
	IQuery* m_pQuery;
};