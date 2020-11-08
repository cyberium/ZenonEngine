#include "stdafx.h"

// General
#include "RenderPass.h"


IBlendState::BlendMode RenderPass::alphaBlending = IBlendState::BlendMode(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
IBlendState::BlendMode RenderPass::additiveBlending = IBlendState::BlendMode(true, false, IBlendState::BlendFactor::One, IBlendState::BlendFactor::One);
IBlendState::BlendMode RenderPass::disableBlending = IBlendState::BlendMode(false);
IDepthStencilState::DepthMode RenderPass::enableDepthWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Enable);
IDepthStencilState::DepthMode RenderPass::enableTestDisableWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Disable);
IDepthStencilState::DepthMode RenderPass::disableDepthWrites = IDepthStencilState::DepthMode(false, IDepthStencilState::DepthWrite::Disable);




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
