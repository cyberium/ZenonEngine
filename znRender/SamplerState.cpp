#include "stdafx.h"

// General
#include "SamplerState.h"

SamplerState::SamplerState()
    : m_MinFilter(MinFilter::MinNearest)
    , m_MagFilter(SamplerState::MagFilter::MagNearest)
    , m_MipFilter(SamplerState::MipFilter::MipNearest)
    , m_WrapModeU(WrapMode::Repeat)
    , m_WrapModeV(WrapMode::Repeat)
    , m_WrapModeW(WrapMode::Repeat)
    , m_CompareMode(CompareMode::None)
    , m_CompareFunc(CompareFunc::Always)
    , m_fLODBias(0.0f)
    , m_fMinLOD(0.0f)
    , m_fMaxLOD(Math::MaxFloat)
    , m_bIsAnisotropicFilteringEnabled(false)
    , m_AnisotropicFiltering(1)
    , m_bIsDirty(true)
{
}

SamplerState::~SamplerState()
{
}

void SamplerState::SetFilter(MinFilter minFilter, MagFilter magFilter, MipFilter mipFilter)
{
    m_MinFilter = minFilter;
    m_MagFilter = magFilter;
    m_MipFilter = mipFilter;
    m_bIsDirty = true;
}

void SamplerState::GetFilter(MinFilter& minFilter, MagFilter& magFilter, MipFilter& mipFilter) const
{
    minFilter = m_MinFilter;
    magFilter = m_MagFilter;
    mipFilter = m_MipFilter;
}

void SamplerState::SetWrapMode(WrapMode u, WrapMode v, WrapMode w)
{
    m_WrapModeU = u;
    m_WrapModeV = v;
    m_WrapModeW = w;
    m_bIsDirty = true;
}

void SamplerState::GetWrapMode(WrapMode& u, WrapMode& v, WrapMode& w) const
{
    u = m_WrapModeU;
    v = m_WrapModeV;
    w = m_WrapModeW;
}

void SamplerState::SetCompareFunction(CompareFunc compareFunc)
{
    m_CompareFunc = compareFunc;
    m_bIsDirty = true;
}

SamplerState::CompareFunc SamplerState::GetCompareFunc() const
{
    return m_CompareFunc;
}

void SamplerState::SetLODBias(float lodBias)
{
    m_fLODBias = lodBias;
    m_bIsDirty = true;
}

float SamplerState::GetLODBias() const
{
    return m_fLODBias;
}

void SamplerState::SetMinLOD(float minLOD)
{
    m_fMinLOD = minLOD;
    m_bIsDirty = true;
}

float SamplerState::GetMinLOD() const
{
    return m_fMinLOD;
}

void SamplerState::SetMaxLOD(float maxLOD)
{
    m_fMaxLOD = maxLOD;
    m_bIsDirty = true;
}

float SamplerState::GetMaxLOD() const
{
    return m_fMaxLOD;
}

void SamplerState::SetBorderColor(cvec4 borderColor)
{
    m_BorderColor = borderColor;
    m_bIsDirty = true;
}

vec4 SamplerState::GetBorderColor() const
{
    return m_BorderColor;
}

void SamplerState::EnableAnisotropicFiltering(bool enabled)
{
    m_bIsAnisotropicFilteringEnabled = enabled;
    m_bIsDirty = true;
}

bool SamplerState::IsAnisotropicFilteringEnabled() const
{
    return m_bIsAnisotropicFilteringEnabled;
}


void SamplerState::SetMaxAnisotropy(uint8_t maxAnisotropy)
{
    m_AnisotropicFiltering = glm::clamp<uint8_t>(maxAnisotropy, 1, 16);
    m_bIsDirty = true;
}

uint8_t SamplerState::GetMaxAnisotropy() const
{
    return m_AnisotropicFiltering;
}