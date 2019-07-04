#include <stdafx.h>

// Genral
#include "SamplerStateDX11.h"

// FORWARD BEGIN
D3D11_FILTER DX11TranslateFilter(bool IsAnisotropicFilteringEnabled, SamplerState::CompareMode CompareMode, SamplerState::MinFilter MinFilter, SamplerState::MagFilter MagFilter, SamplerState::MipFilter MipFilter);
D3D11_TEXTURE_ADDRESS_MODE DX11TranslateWrapMode(SamplerState::WrapMode wrapMode);
D3D11_COMPARISON_FUNC DX11TranslateComparisonFunction(SamplerState::CompareFunc compareFunc);
// FORWARD END

SamplerStateDX11::SamplerStateDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
	, m_pDeviceContext(nullptr)
	, m_pSamplerState(nullptr)
{
	if (m_pDevice)
	{
		m_pDevice->GetImmediateContext2(&m_pDeviceContext);
	}
}

SamplerStateDX11::~SamplerStateDX11()
{}



//
// SamplerState 
//
void SamplerStateDX11::Bind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
    if (m_bIsDirty || m_pSamplerState == nullptr)
    {
        D3D11_SAMPLER_DESC samplerDesc;
        samplerDesc.Filter = DX11TranslateFilter(m_bIsAnisotropicFilteringEnabled, m_CompareMode, m_MinFilter, m_MagFilter, m_MipFilter);
        samplerDesc.AddressU = DX11TranslateWrapMode(m_WrapModeU);
        samplerDesc.AddressV = DX11TranslateWrapMode(m_WrapModeV);
        samplerDesc.AddressW = DX11TranslateWrapMode(m_WrapModeW);
        samplerDesc.MipLODBias = m_fLODBias;
        samplerDesc.MaxAnisotropy = m_AnisotropicFiltering;
        samplerDesc.ComparisonFunc = DX11TranslateComparisonFunction(m_CompareFunc);
        samplerDesc.BorderColor[0] = m_BorderColor.r;
        samplerDesc.BorderColor[1] = m_BorderColor.g;
        samplerDesc.BorderColor[2] = m_BorderColor.b;
        samplerDesc.BorderColor[3] = m_BorderColor.a;
        samplerDesc.MinLOD = m_fMinLOD;
        samplerDesc.MaxLOD = m_fMaxLOD;

        m_pSamplerState = NULL;
        m_pDevice->CreateSamplerState(&samplerDesc, &m_pSamplerState);

        m_bIsDirty = false;
    }

    std::shared_ptr<Shader> pShader = shader.lock();
    _ASSERT(pShader != NULL);
    ID3D11SamplerState* pSamplers[] = { m_pSamplerState };

    switch (pShader->GetType())
    {
        case Shader::VertexShader:
            m_pDeviceContext->VSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::TessellationControlShader:
            m_pDeviceContext->HSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::TessellationEvaluationShader:
            m_pDeviceContext->DSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::GeometryShader:
            m_pDeviceContext->GSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::PixelShader:
            m_pDeviceContext->PSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::ComputeShader:
            m_pDeviceContext->CSSetSamplers(ID, 1, pSamplers);
            break;
    }
}

void SamplerStateDX11::UnBind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
    std::shared_ptr<Shader> pShader = shader.lock();
    _ASSERT(pShader != NULL);
    ID3D11SamplerState* pSamplers[] = { nullptr };

    switch (pShader->GetType())
    {
        case Shader::VertexShader:
            m_pDeviceContext->VSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::TessellationControlShader:
            m_pDeviceContext->HSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::TessellationEvaluationShader:
            m_pDeviceContext->DSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::GeometryShader:
            m_pDeviceContext->GSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::PixelShader:
            m_pDeviceContext->PSSetSamplers(ID, 1, pSamplers);
            break;
        case Shader::ComputeShader:
            m_pDeviceContext->CSSetSamplers(ID, 1, pSamplers);
            break;
    }
}


//
// Translate
//
D3D11_FILTER DX11TranslateFilter(bool IsAnisotropicFilteringEnabled, SamplerState::CompareMode CompareMode, SamplerState::MinFilter MinFilter, SamplerState::MagFilter MagFilter, SamplerState::MipFilter MipFilter)
{
	D3D11_FILTER filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	if (IsAnisotropicFilteringEnabled)
	{
		filter = (CompareMode == SamplerState::CompareMode::None) ? D3D11_FILTER_ANISOTROPIC : D3D11_FILTER_COMPARISON_ANISOTROPIC;
		return filter;
	}

	if (MinFilter == SamplerState::MinFilter::MinNearest && MagFilter == SamplerState::MagFilter::MagNearest && MipFilter == SamplerState::MipFilter::MipNearest)
	{
		filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	}
	else if (MinFilter == SamplerState::MinFilter::MinNearest && MagFilter == SamplerState::MagFilter::MagNearest && MipFilter == SamplerState::MipFilter::MipLinear)
	{
		filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
	}
	else if (MinFilter == SamplerState::MinFilter::MinNearest && MagFilter == SamplerState::MagFilter::MagLinear && MipFilter == SamplerState::MipFilter::MipNearest)
	{
		filter = D3D11_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
	}
	else if (MinFilter == SamplerState::MinFilter::MinNearest && MagFilter == SamplerState::MagFilter::MagLinear && MipFilter == SamplerState::MipFilter::MipLinear)
	{
		filter = D3D11_FILTER_MIN_POINT_MAG_MIP_LINEAR;
	}
	else if (MinFilter == SamplerState::MinFilter::MinLinear && MagFilter == SamplerState::MagFilter::MagNearest && MipFilter == SamplerState::MipFilter::MipNearest)
	{
		filter = D3D11_FILTER_MIN_LINEAR_MAG_MIP_POINT;
	}
	else if (MinFilter == SamplerState::MinFilter::MinLinear && MagFilter == SamplerState::MagFilter::MagNearest && MipFilter == SamplerState::MipFilter::MipLinear)
	{
		filter = D3D11_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
	}
	else if (MinFilter == SamplerState::MinFilter::MinLinear && MagFilter == SamplerState::MagFilter::MagLinear && MipFilter == SamplerState::MipFilter::MipNearest)
	{
		filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	}
	else if (MinFilter == SamplerState::MinFilter::MinLinear && MagFilter == SamplerState::MagFilter::MagLinear && MipFilter == SamplerState::MipFilter::MipLinear)
	{
		filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	}
	else
	{
		throw std::exception("Invalid texture filter modes.");
	}

	if (CompareMode != SamplerState::CompareMode::None)
	{
		*(reinterpret_cast<int*>(&filter)) += static_cast<int>(D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT);
	}

	return filter;
}

D3D11_TEXTURE_ADDRESS_MODE DX11TranslateWrapMode(SamplerState::WrapMode wrapMode)
{
	D3D11_TEXTURE_ADDRESS_MODE addressMode = D3D11_TEXTURE_ADDRESS_WRAP;

	switch (wrapMode)
	{
	case SamplerState::WrapMode::Repeat:
		addressMode = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case SamplerState::WrapMode::Clamp:
		addressMode = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case SamplerState::WrapMode::Mirror:
		addressMode = D3D11_TEXTURE_ADDRESS_MIRROR;
		break;
	case SamplerState::WrapMode::Border:
		addressMode = D3D11_TEXTURE_ADDRESS_BORDER;
		break;
	}

	return addressMode;
}

D3D11_COMPARISON_FUNC DX11TranslateComparisonFunction(SamplerState::CompareFunc compareFunc)
{
	D3D11_COMPARISON_FUNC compareFuncD3D11 = D3D11_COMPARISON_ALWAYS;
	switch (compareFunc)
	{
	case SamplerState::CompareFunc::Never:
		compareFuncD3D11 = D3D11_COMPARISON_NEVER;
		break;
	case SamplerState::CompareFunc::Less:
		compareFuncD3D11 = D3D11_COMPARISON_LESS;
		break;
	case SamplerState::CompareFunc::Equal:
		compareFuncD3D11 = D3D11_COMPARISON_EQUAL;
		break;
	case SamplerState::CompareFunc::LessEqual:
		compareFuncD3D11 = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case SamplerState::CompareFunc::Greater:
		compareFuncD3D11 = D3D11_COMPARISON_GREATER;
		break;
	case SamplerState::CompareFunc::NotEqual:
		compareFuncD3D11 = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case SamplerState::CompareFunc::GreaterEqual:
		compareFuncD3D11 = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case SamplerState::CompareFunc::Always:
		compareFuncD3D11 = D3D11_COMPARISON_ALWAYS;
		break;
	}

	return compareFuncD3D11;
}
