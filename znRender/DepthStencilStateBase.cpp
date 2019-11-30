#include "stdafx.h"

// General
#include "DepthStencilStateBase.h"

DepthStencilStateBase::DepthStencilStateBase()
    : m_bDirty(true)
{}

DepthStencilStateBase::~DepthStencilStateBase()
{}

void DepthStencilStateBase::SetDepthMode(const DepthMode& depthMode)
{
    m_DepthMode = depthMode;
    m_bDirty = true;
}

const DepthStencilStateBase::DepthMode& DepthStencilStateBase::GetDepthMode() const
{
    return m_DepthMode;
}

void DepthStencilStateBase::SetStencilMode(const StencilMode& stencilMode)
{
    m_StencilMode = stencilMode;
    m_bDirty = true;
}

const DepthStencilStateBase::StencilMode& DepthStencilStateBase::GetStencilMode() const
{
    return m_StencilMode;
}