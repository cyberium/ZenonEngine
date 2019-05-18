#pragma once

#include "AbstractPass.h"

class InvokeFunctionPass : public AbstractPass
{
public:
	InvokeFunctionPass(std::function<void(void)> func);
	virtual ~InvokeFunctionPass();

	virtual void Render(RenderEventArgs& e);

private:
	std::function<void(void)> m_Func;
};