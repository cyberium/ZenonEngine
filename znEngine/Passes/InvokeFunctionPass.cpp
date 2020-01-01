#include "stdafx.h"

// General
#include "InvokeFunctionPass.h"

InvokeFunctionPass::InvokeFunctionPass(std::shared_ptr<IRenderDevice> RenderDevice, std::function<void(void)> func)
	: AbstractPass(RenderDevice)
	, m_Func(func)
{}

InvokeFunctionPass::~InvokeFunctionPass()
{}

void InvokeFunctionPass::Render(RenderEventArgs& e)
{
	if (m_Func)
	{
		m_Func();
	}
}
