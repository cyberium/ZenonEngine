#include "stdafx.h"

// General
#include "BlendStateBase.h"

BlendStateBase::BlendStateBase()
    : m_bAlphaToCoverageEnabled(false)
    , m_bIndependentBlendEnabled(false)
    , m_SampleMask(0xffffffff)
    , m_ConstBlendFactor(1)
    , m_bDirty(true)
{
}

BlendStateBase::~BlendStateBase()
{
}

void BlendStateBase::SetBlendMode(const BlendStateBase::BlendMode& blendMode)
{
    m_BlendModes[0] = blendMode;
    m_bDirty = true;
}

void BlendStateBase::SetBlendModes(const std::vector<BlendMode>& blendModes)
{
    m_BlendModes = blendModes;
    m_bDirty = true;
}

const std::vector<BlendStateBase::BlendMode>& BlendStateBase::GetBlendModes() const
{
    return m_BlendModes;
}

void BlendStateBase::SetAlphaCoverage(bool enabled)
{
    if (m_bAlphaToCoverageEnabled != enabled)
    {
        m_bAlphaToCoverageEnabled = enabled;
        m_bDirty = true;
    }

}

bool BlendStateBase::GetAlphaCoverage() const
{
    return m_bAlphaToCoverageEnabled;
}

void BlendStateBase::SetIndependentBlend(bool enabled)
{
    if (m_bAlphaToCoverageEnabled != enabled)
    {
        m_bAlphaToCoverageEnabled = enabled;
        m_bDirty = true;
    }
}

bool BlendStateBase::GetIndependentBlend() const
{
    return m_bIndependentBlendEnabled;
}

void BlendStateBase::SetConstBlendFactor(cvec4 constantBlendFactor)
{
    m_ConstBlendFactor = constantBlendFactor;
    // No need to set the dirty flag as this value is not used to create the blend state object.
    // It is only used when activating the blend state of the output merger.
}

cvec4 BlendStateBase::GetConstBlendFactor() const
{
    return m_ConstBlendFactor;
}

void BlendStateBase::SetSampleMask(uint32_t sampleMask)
{
    m_SampleMask = sampleMask;
    // No need to set the dirty flag as this value is not used to create the blend state object.
    // It is only used when activating the blend state of the output merger.
}

uint32_t BlendStateBase::GetSampleMask() const
{
    return m_SampleMask;
}