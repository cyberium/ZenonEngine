#include <stdafx.h>

// General
#include "InvokeFunctionPass.h"

InvokeFunctionPass::InvokeFunctionPass(std::function<void(void)> func)
	: m_Func(func)
{}

InvokeFunctionPass::~InvokeFunctionPass()
{}

void InvokeFunctionPass::Render(Render3DEventArgs& e)
{
	if (m_Func)
	{
		m_Func();
	}
}
