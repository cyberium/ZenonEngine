#include "stdafx.h"

// General
#include "RenderPass.h"

RenderPass::RenderPass(IRenderDevice& RenderDevice)
	: m_Enabled(true)
    , m_RenderEventArgs(nullptr)
    , m_RenderDevice(RenderDevice)
	, m_BaseManager(RenderDevice.GetBaseManager())
{}

RenderPass::~RenderPass()
{}



//
// IRenderPass
//
void RenderPass::SetEnabled(bool Value)
{
	m_Enabled = Value;
}

bool RenderPass::IsEnabled() const
{
	return m_Enabled;
}

void RenderPass::PreRender(RenderEventArgs& e)
{
	m_RenderEventArgs = &e;
}

void RenderPass::PostRender(RenderEventArgs& e)
{}



//
// Protected
//
IBaseManager& RenderPass::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice& RenderPass::GetRenderDevice() const
{
	return m_RenderDevice;
}

const RenderEventArgs& RenderPass::GetRenderEventArgs() const
{
    _ASSERT(m_RenderEventArgs != nullptr);
    return *m_RenderEventArgs;
}
