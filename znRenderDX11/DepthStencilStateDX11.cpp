#include <stdafx.h>

#include "DepthStencilStateDX11.h"

// FORWARD BEGIN
D3D11_DEPTH_WRITE_MASK TranslateDepthWriteMask(DepthStencilState::DepthWrite depthWrite);
D3D11_COMPARISON_FUNC TranslateCompareFunc(DepthStencilState::CompareFunction compareFunc);
D3D11_STENCIL_OP TranslateStencilOperation(DepthStencilState::StencilOperation stencilOperation);
D3D11_DEPTH_STENCILOP_DESC TranslateFaceOperation(DepthStencilState::FaceOperation faceOperation);
D3D11_DEPTH_STENCIL_DESC TranslateDepthStencilState(const DepthStencilState::DepthMode& depthMode, const DepthStencilState::StencilMode& stencilMode);
// FORWARD END

DepthStencilStateDX11::DepthStencilStateDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
{
	assert(pDevice);
	m_pDevice->GetImmediateContext2(&m_pDeviceContext);
}

DepthStencilStateDX11::DepthStencilStateDX11(const DepthStencilStateDX11& copy)
	: m_pDevice(copy.m_pDevice)
	, m_pDeviceContext(copy.m_pDeviceContext)
{
    m_DepthMode = copy.m_DepthMode;
    m_StencilMode = copy.m_StencilMode;
    m_bDirty = true;
}

DepthStencilStateDX11::~DepthStencilStateDX11()
{

}

const DepthStencilStateDX11& DepthStencilStateDX11::operator=(const DepthStencilStateDX11& other)
{
	if (this != &other)
	{
		m_pDevice = other.m_pDevice;
		m_pDeviceContext = other.m_pDeviceContext;
		m_DepthMode = other.m_DepthMode;
		m_StencilMode = other.m_StencilMode;
		m_bDirty = true;
	}

	return *this;
}





//
// DepthStencilState
//
void DepthStencilStateDX11::Bind()
{
	if (m_bDirty)
	{
		D3D11_DEPTH_STENCIL_DESC depthStencilDesc = TranslateDepthStencilState(m_DepthMode, m_StencilMode);

		m_pDepthStencilState = NULL;
		if (FAILED(m_pDevice->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState)))
		{
			Log::Error("Failed to create depth stencil state.");
		}

		m_bDirty = false;
	}

	m_pDeviceContext->OMSetDepthStencilState(m_pDepthStencilState, m_StencilMode.StencilReference);
}



//
// Translate
//
D3D11_DEPTH_WRITE_MASK TranslateDepthWriteMask(DepthStencilState::DepthWrite depthWrite)
{
	D3D11_DEPTH_WRITE_MASK result = D3D11_DEPTH_WRITE_MASK_ALL;

	switch (depthWrite)
	{
	case DepthStencilState::DepthWrite::Enable:
		result = D3D11_DEPTH_WRITE_MASK_ALL;
		break;
	case DepthStencilState::DepthWrite::Disable:
		result = D3D11_DEPTH_WRITE_MASK_ZERO;
		break;
	default:
		Log::Error("Unknown depth write mask.");
		break;
	}

	return result;
}

D3D11_COMPARISON_FUNC TranslateCompareFunc(DepthStencilState::CompareFunction compareFunc)
{
	D3D11_COMPARISON_FUNC result = D3D11_COMPARISON_LESS;

	switch (compareFunc)
	{
	case DepthStencilState::CompareFunction::Never:
		result = D3D11_COMPARISON_NEVER;
		break;
	case DepthStencilState::CompareFunction::Less:
		result = D3D11_COMPARISON_LESS;
		break;
	case DepthStencilState::CompareFunction::Equal:
		result = D3D11_COMPARISON_EQUAL;
		break;
	case DepthStencilState::CompareFunction::LessOrEqual:
		result = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case DepthStencilState::CompareFunction::Greater:
		result = D3D11_COMPARISON_GREATER;
		break;
	case DepthStencilState::CompareFunction::NotEqual:
		result = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case DepthStencilState::CompareFunction::GreaterOrEqual:
		result = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case DepthStencilState::CompareFunction::Always:
		result = D3D11_COMPARISON_ALWAYS;
		break;
	default:
		Log::Error("Unknown compare function.");
		break;
	}

	return result;
}

D3D11_STENCIL_OP TranslateStencilOperation(DepthStencilState::StencilOperation stencilOperation)
{
	D3D11_STENCIL_OP result = D3D11_STENCIL_OP_KEEP;

	switch (stencilOperation)
	{
	case DepthStencilState::StencilOperation::Keep:
		result = D3D11_STENCIL_OP_KEEP;
		break;
	case DepthStencilState::StencilOperation::Zero:
		result = D3D11_STENCIL_OP_ZERO;
		break;
	case DepthStencilState::StencilOperation::Reference:
		result = D3D11_STENCIL_OP_REPLACE;
		break;
	case DepthStencilState::StencilOperation::IncrementClamp:
		result = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case DepthStencilState::StencilOperation::DecrementClamp:
		result = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case DepthStencilState::StencilOperation::Invert:
		result = D3D11_STENCIL_OP_INVERT;
		break;
	case DepthStencilState::StencilOperation::IncrementWrap:
		result = D3D11_STENCIL_OP_INCR;
		break;
	case DepthStencilState::StencilOperation::DecrementWrap:
		result = D3D11_STENCIL_OP_DECR;
		break;
	default:
		Log::Error("Unknown stencil operation.");
		break;
	}

	return result;
}

D3D11_DEPTH_STENCILOP_DESC TranslateFaceOperation(DepthStencilState::FaceOperation faceOperation)
{
	D3D11_DEPTH_STENCILOP_DESC result;

	result.StencilFailOp = TranslateStencilOperation(faceOperation.StencilFail);
	result.StencilDepthFailOp = TranslateStencilOperation(faceOperation.StencilPassDepthFail);
	result.StencilPassOp = TranslateStencilOperation(faceOperation.StencilDepthPass);
	result.StencilFunc = TranslateCompareFunc(faceOperation.StencilFunction);

	return result;
}

D3D11_DEPTH_STENCIL_DESC TranslateDepthStencilState(const DepthStencilState::DepthMode& depthMode, const DepthStencilState::StencilMode& stencilMode)
{
	D3D11_DEPTH_STENCIL_DESC result;

	result.DepthEnable = depthMode.DepthEnable;
	result.DepthWriteMask = TranslateDepthWriteMask(depthMode.DepthWriteMask);
	result.DepthFunc = TranslateCompareFunc(depthMode.DepthFunction);
	result.StencilEnable = stencilMode.StencilEnabled;
	result.StencilReadMask = stencilMode.ReadMask;
	result.StencilWriteMask = stencilMode.WriteMask;
	result.FrontFace = TranslateFaceOperation(stencilMode.FrontFace);
	result.BackFace = TranslateFaceOperation(stencilMode.BackFace);

	return result;
}
