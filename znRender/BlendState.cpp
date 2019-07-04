#include "stdafx.h"

// General
#include "BlendState.h"

BlendState::BlendState()
    : m_bAlphaToCoverageEnabled(false)
    , m_bIndependentBlendEnabled(false)
    , m_SampleMask(0xffffffff)
    , m_ConstBlendFactor(1)
    , m_bDirty(true)
{
}

BlendState::~BlendState()
{
}

void BlendState::SetBlendMode(const BlendState::BlendMode& blendMode)
{
    m_BlendModes[0] = blendMode;
    m_bDirty = true;
}

void BlendState::SetBlendModes(const std::vector<BlendMode>& blendModes)
{
    m_BlendModes = blendModes;
    m_bDirty;
}

const std::vector<BlendState::BlendMode>& BlendState::GetBlendModes() const
{
    return m_BlendModes;
}

void BlendState::SetAlphaCoverage(bool enabled)
{
    if (m_bAlphaToCoverageEnabled != enabled)
    {
        m_bAlphaToCoverageEnabled = enabled;
        m_bDirty = true;
    }

}

bool BlendState::GetAlphaCoverage() const
{
    return m_bAlphaToCoverageEnabled;
}

void BlendState::SetIndependentBlend(bool enabled)
{
    if (m_bAlphaToCoverageEnabled != enabled)
    {
        m_bAlphaToCoverageEnabled = enabled;
        m_bDirty = true;
    }
}

bool BlendState::GetIndependentBlend() const
{
    return m_bIndependentBlendEnabled;
}

void BlendState::SetConstBlendFactor(cvec4 constantBlendFactor)
{
    m_ConstBlendFactor = constantBlendFactor;
    // No need to set the dirty flag as this value is not used to create the blend state object.
    // It is only used when activating the blend state of the output merger.
}

cvec4 BlendState::GetConstBlendFactor() const
{
    return m_ConstBlendFactor;
}

void BlendState::SetSampleMask(uint32_t sampleMask)
{
    m_SampleMask = sampleMask;
    // No need to set the dirty flag as this value is not used to create the blend state object.
    // It is only used when activating the blend state of the output merger.
}

uint32_t BlendState::GetSampleMask() const
{
    return m_SampleMask;
}