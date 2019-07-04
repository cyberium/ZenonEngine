#include "stdafx.h"

// General
#include "RasterizerState.h"


RasterizerState::RasterizerState()
    : m_FrontFaceFillMode(FillMode::Solid)
    , m_BackFaceFillMode(FillMode::Solid)
    , m_CullMode(CullMode::Back)
    , m_FrontFace(FrontFace::CounterClockwise)
    , m_DepthBias(0.0f)
    , m_SlopeBias(0.0f)
    , m_BiasClamp(0.0f)
    , m_DepthClipEnabled(true)
    , m_ScissorEnabled(false)
    , m_MultisampleEnabled(false)
    , m_AntialiasedLineEnabled(false)
    , m_ConservativeRasterization(false)
    , m_ForcedSampleCount(0)
    , m_StateDirty(true)
    , m_ViewportsDirty(true)
    , m_ScissorRectsDirty(true)
{
}

RasterizerState::~RasterizerState()
{
}

void RasterizerState::SetFillMode(FillMode frontFace, FillMode backFace)
{
    m_FrontFaceFillMode = frontFace;
    m_BackFaceFillMode = backFace;
}

void RasterizerState::GetFillMode(FillMode& frontFace, FillMode& backFace) const
{
    frontFace = m_FrontFaceFillMode;
    backFace = m_BackFaceFillMode;
}

void RasterizerState::SetCullMode(CullMode cullMode)
{
    m_CullMode = cullMode;
    m_StateDirty = true;
}

RasterizerState::CullMode RasterizerState::GetCullMode() const
{
    return m_CullMode;
}

void RasterizerState::SetFrontFacing(FrontFace frontFace)
{
    m_FrontFace = frontFace;
    m_StateDirty = true;
}

RasterizerState::FrontFace RasterizerState::GetFrontFacing() const
{
    return m_FrontFace;
}

void RasterizerState::SetDepthBias(float depthBias, float slopeBias, float biasClamp)
{
    m_DepthBias = depthBias;
    m_SlopeBias = slopeBias;
    m_BiasClamp = biasClamp;

    m_StateDirty = true;
}

void RasterizerState::GetDepthBias(float& depthBias, float& slopeBias, float& biasClamp) const
{
    depthBias = m_DepthBias;
    slopeBias = m_SlopeBias;
    biasClamp = m_BiasClamp;
}

void RasterizerState::SetDepthClipEnabled(bool depthClipEnabled)
{
    m_DepthClipEnabled = depthClipEnabled;
    m_StateDirty = true;
}

bool RasterizerState::GetDepthClipEnabled() const
{
    return m_DepthClipEnabled;
}

void RasterizerState::SetViewport(const Viewport& viewport)
{
    m_Viewports[0] = viewport;
    m_ViewportsDirty = true;
}

void RasterizerState::SetViewports(const std::vector<Viewport>& viewports)
{
    m_Viewports = viewports;
    m_ViewportsDirty = true;
}

const std::vector<Viewport>& RasterizerState::GetViewports()
{
    return m_Viewports;
}

void RasterizerState::SetScissorEnabled(bool scissorEnable)
{
    m_ScissorEnabled = scissorEnable;
    m_StateDirty = true;
}

bool RasterizerState::GetScissorEnabled() const
{
    return m_ScissorEnabled;
}

void RasterizerState::SetScissorRect(const Rect& rect)
{
    m_ScissorRects[0] = rect;
    m_ScissorRectsDirty = true;
}

void RasterizerState::SetScissorRects(const std::vector<Rect>& rects)
{
    m_ScissorRects = rects;
    m_ScissorRectsDirty = true;
}

const std::vector<Rect>& RasterizerState::GetScissorRects() const
{
    return m_ScissorRects;
}

void RasterizerState::SetMultisampleEnabled(bool multisampleEnabled)
{
    m_MultisampleEnabled = multisampleEnabled;
    m_StateDirty = true;
}

bool RasterizerState::GetMultisampleEnabled() const
{
    return m_MultisampleEnabled;
}

void RasterizerState::SetAntialiasedLineEnable(bool antialiasedLineEnabled)
{
    m_AntialiasedLineEnabled = antialiasedLineEnabled;
    m_StateDirty = true;
}

bool RasterizerState::GetAntialiasedLineEnable() const
{
    return m_AntialiasedLineEnabled;
}

void RasterizerState::SetForcedSampleCount(uint8_t sampleCount)
{
    m_ForcedSampleCount = sampleCount;
    m_StateDirty = true;
}

uint8_t RasterizerState::GetForcedSampleCount()
{
    return m_ForcedSampleCount;
}

void RasterizerState::SetConservativeRasterizationEnabled(bool conservativeRasterizationEnabled)
{
    m_ConservativeRasterization = conservativeRasterizationEnabled;
}

bool RasterizerState::GetConservativeRasterizationEnabled() const
{
    // Currently, this implementation always returns false
    // because conservative rasterization is supported since DirectX 11.3 and 12.
    return false;
}