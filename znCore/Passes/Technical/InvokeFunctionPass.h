#pragma once

#include "../RenderPass.h"

class ZN_API InvokeFunctionPass 
	: public RenderPass
{
public:
	InvokeFunctionPass(IRenderDevice& RenderDevice, std::function<void(void)> func);
	virtual ~InvokeFunctionPass();

	virtual void Render(RenderEventArgs& e);

private:
	std::function<void(void)> m_Func;
};