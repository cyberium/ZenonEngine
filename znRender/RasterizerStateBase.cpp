#include "stdafx.h"

// General
#include "RasterizerStateBase.h"

RasterizerStateBase::RasterizerStateBase()
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
{}

RasterizerStateBase::~RasterizerStateBase()
{}

void RasterizerStateBase::SetFillMode(FillMode frontFace, FillMode backFace)
{
    m_FrontFaceFillMode = frontFace;
    m_BackFaceFillMode = backFace;
}

void RasterizerStateBase::GetFillMode(FillMode& frontFace, FillMode& backFace) const
{
    frontFace = m_FrontFaceFillMode;
    backFace = m_BackFaceFillMode;
}

void RasterizerStateBase::SetCullMode(CullMode cullMode)
{
    m_CullMode = cullMode;
    m_StateDirty = true;
}

RasterizerStateBase::CullMode RasterizerStateBase::GetCullMode() const
{
    return m_CullMode;
}

void RasterizerStateBase::SetFrontFacing(FrontFace frontFace)
{
    m_FrontFace = frontFace;
    m_StateDirty = true;
}

RasterizerStateBase::FrontFace RasterizerStateBase::GetFrontFacing() const
{
    return m_FrontFace;
}

void RasterizerStateBase::SetDepthBias(float depthBias, float slopeBias, float biasClamp)
{
    m_DepthBias = depthBias;
    m_SlopeBias = slopeBias;
    m_BiasClamp = biasClamp;

    m_StateDirty = true;
}

void RasterizerStateBase::GetDepthBias(float& depthBias, float& slopeBias, float& biasClamp) const
{
    depthBias = m_DepthBias;
    slopeBias = m_SlopeBias;
    biasClamp = m_BiasClamp;
}

void RasterizerStateBase::SetDepthClipEnabled(bool depthClipEnabled)
{
    m_DepthClipEnabled = depthClipEnabled;
    m_StateDirty = true;
}

bool RasterizerStateBase::GetDepthClipEnabled() const
{
    return m_DepthClipEnabled;
}

void RasterizerStateBase::SetViewport(const Viewport * viewport)
{
    m_Viewports[0] = viewport;
    m_ViewportsDirty = true;
}

void RasterizerStateBase::SetViewports(const std::vector<const Viewport *>& viewports)
{
    m_Viewports = viewports;
    m_ViewportsDirty = true;
}

const std::vector<const Viewport *>& RasterizerStateBase::GetViewports()
{
    return m_Viewports;
}

void RasterizerStateBase::SetScissorEnabled(bool scissorEnable)
{
    m_ScissorEnabled = scissorEnable;
    m_StateDirty = true;
}

bool RasterizerStateBase::GetScissorEnabled() const
{
    return m_ScissorEnabled;
}

void RasterizerStateBase::SetScissorRect(const Rect& rect)
{
    m_ScissorRects[0] = rect;
    m_ScissorRectsDirty = true;
}

void RasterizerStateBase::SetScissorRects(const std::vector<Rect>& rects)
{
    m_ScissorRects = rects;
    m_ScissorRectsDirty = true;
}

const std::vector<Rect>& RasterizerStateBase::GetScissorRects() const
{
    return m_ScissorRects;
}

void RasterizerStateBase::SetMultisampleEnabled(bool multisampleEnabled)
{
    m_MultisampleEnabled = multisampleEnabled;
    m_StateDirty = true;
}

bool RasterizerStateBase::GetMultisampleEnabled() const
{
    return m_MultisampleEnabled;
}

void RasterizerStateBase::SetAntialiasedLineEnable(bool antialiasedLineEnabled)
{
    m_AntialiasedLineEnabled = antialiasedLineEnabled;
    m_StateDirty = true;
}

bool RasterizerStateBase::GetAntialiasedLineEnable() const
{
    return m_AntialiasedLineEnabled;
}

void RasterizerStateBase::SetForcedSampleCount(uint8_t sampleCount)
{
    m_ForcedSampleCount = sampleCount;
    m_StateDirty = true;
}

uint8_t RasterizerStateBase::GetForcedSampleCount()
{
    return m_ForcedSampleCount;
}

void RasterizerStateBase::SetConservativeRasterizationEnabled(bool conservativeRasterizationEnabled)
{
    m_ConservativeRasterization = conservativeRasterizationEnabled;
}

bool RasterizerStateBase::GetConservativeRasterizationEnabled() const
{
    // Currently, this implementation always returns false
    // because conservative rasterization is supported since DirectX 11.3 and 12.
    return false;
}
