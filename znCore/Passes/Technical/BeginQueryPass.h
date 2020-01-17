#pragma once

#include "../RenderPass.h"

class ZN_API BeginQueryPass 
	: public RenderPass
{
public:
	BeginQueryPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IQuery> query);
	virtual ~BeginQueryPass();

	virtual void Render(RenderEventArgs& e);

private:
	std::shared_ptr<IQuery> m_pQuery;
};