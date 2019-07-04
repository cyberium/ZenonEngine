#include "stdafx.h"

// General
#include "DepthStencilState.h"

DepthStencilState::DepthStencilState()
    : m_bDirty(true)
{
}

DepthStencilState::~DepthStencilState()
{
}

void DepthStencilState::SetDepthMode(const DepthMode& depthMode)
{
    m_DepthMode = depthMode;
    m_bDirty = true;
}

const DepthStencilState::DepthMode& DepthStencilState::GetDepthMode() const
{
    return m_DepthMode;
}

void DepthStencilState::SetStencilMode(const StencilMode& stencilMode)
{
    m_StencilMode = stencilMode;
    m_bDirty = true;
}

const DepthStencilState::StencilMode& DepthStencilState::GetStencilMode() const
{
    return m_StencilMode;
}