#include "stdafx.h"

// General
#include "InvokeFunctionPass.h"

InvokeFunctionPass::InvokeFunctionPass(IRenderDevice* RenderDevice, std::function<void(void)> func)
	: RenderPass(RenderDevice)
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
