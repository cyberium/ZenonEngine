#include "stdafx.h"

// General
#include "InvokeFunctionPass.h"

InvokeFunctionPass::InvokeFunctionPass(IRenderDevice& RenderDevice, std::function<void(void)> func)
	: RenderPass(RenderDevice)
{}

InvokeFunctionPass::~InvokeFunctionPass()
{}

void InvokeFunctionPass::SetFunc(std::function<void(void)> func)
{
	m_Func = func;
}

void InvokeFunctionPass::Render(RenderEventArgs& e)
{
	if (m_Func)
	{
		m_Func();
	}
}
