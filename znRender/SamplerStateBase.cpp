#include "stdafx.h"

// General
#include "SamplerStateBase.h"

SamplerStateBase::SamplerStateBase()
    : m_MinFilter(MinFilter::MinNearest)
    , m_MagFilter(SamplerStateBase::MagFilter::MagNearest)
    , m_MipFilter(SamplerStateBase::MipFilter::MipNearest)
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

SamplerStateBase::~SamplerStateBase()
{
}

void SamplerStateBase::SetFilter(MinFilter minFilter, MagFilter magFilter, MipFilter mipFilter)
{
    m_MinFilter = minFilter;
    m_MagFilter = magFilter;
    m_MipFilter = mipFilter;
    m_bIsDirty = true;
}

void SamplerStateBase::GetFilter(MinFilter& minFilter, MagFilter& magFilter, MipFilter& mipFilter) const
{
    minFilter = m_MinFilter;
    magFilter = m_MagFilter;
    mipFilter = m_MipFilter;
}

void SamplerStateBase::SetWrapMode(WrapMode u, WrapMode v, WrapMode w)
{
    m_WrapModeU = u;
    m_WrapModeV = v;
    m_WrapModeW = w;
    m_bIsDirty = true;
}

void SamplerStateBase::GetWrapMode(WrapMode& u, WrapMode& v, WrapMode& w) const
{
    u = m_WrapModeU;
    v = m_WrapModeV;
    w = m_WrapModeW;
}

void SamplerStateBase::SetCompareFunction(CompareFunc compareFunc)
{
    m_CompareFunc = compareFunc;
    m_bIsDirty = true;
}

SamplerStateBase::CompareFunc SamplerStateBase::GetCompareFunc() const
{
    return m_CompareFunc;
}

void SamplerStateBase::SetLODBias(float lodBias)
{
    m_fLODBias = lodBias;
    m_bIsDirty = true;
}

float SamplerStateBase::GetLODBias() const
{
    return m_fLODBias;
}

void SamplerStateBase::SetMinLOD(float minLOD)
{
    m_fMinLOD = minLOD;
    m_bIsDirty = true;
}

float SamplerStateBase::GetMinLOD() const
{
    return m_fMinLOD;
}

void SamplerStateBase::SetMaxLOD(float maxLOD)
{
    m_fMaxLOD = maxLOD;
    m_bIsDirty = true;
}

float SamplerStateBase::GetMaxLOD() const
{
    return m_fMaxLOD;
}

void SamplerStateBase::SetBorderColor(const glm::vec4& borderColor)
{
    m_BorderColor = borderColor;
    m_bIsDirty = true;
}

glm::vec4 SamplerStateBase::GetBorderColor() const
{
    return m_BorderColor;
}

void SamplerStateBase::EnableAnisotropicFiltering(bool enabled)
{
    m_bIsAnisotropicFilteringEnabled = enabled;
    m_bIsDirty = true;
}

bool SamplerStateBase::IsAnisotropicFilteringEnabled() const
{
    return m_bIsAnisotropicFilteringEnabled;
}


void SamplerStateBase::SetMaxAnisotropy(uint8_t maxAnisotropy)
{
    m_AnisotropicFiltering = glm::clamp<uint8_t>(maxAnisotropy, 1, 16);
    m_bIsDirty = true;
}

uint8_t SamplerStateBase::GetMaxAnisotropy() const
{
    return m_AnisotropicFiltering;
}