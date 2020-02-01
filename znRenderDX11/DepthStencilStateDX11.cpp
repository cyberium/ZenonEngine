#include "stdafx.h"

#include "DepthStencilStateDX11.h"

// FORWARD BEGIN
D3D11_DEPTH_WRITE_MASK TranslateDepthWriteMask(IDepthStencilState::DepthWrite depthWrite);
D3D11_COMPARISON_FUNC TranslateCompareFunc(IDepthStencilState::CompareFunction compareFunc);
D3D11_STENCIL_OP TranslateStencilOperation(IDepthStencilState::StencilOperation stencilOperation);
D3D11_DEPTH_STENCILOP_DESC TranslateFaceOperation(IDepthStencilState::FaceOperation faceOperation);
D3D11_DEPTH_STENCIL_DESC TranslateDepthStencilState(const IDepthStencilState::DepthMode& depthMode, const IDepthStencilState::StencilMode& stencilMode);
// FORWARD END

DepthStencilStateDX11::DepthStencilStateDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
{
}

DepthStencilStateDX11::DepthStencilStateDX11(const DepthStencilStateDX11& copy)
	: m_RenderDeviceDX11(copy.m_RenderDeviceDX11)
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
		m_DepthMode = other.m_DepthMode;
		m_StencilMode = other.m_StencilMode;
		m_RenderDeviceDX11 = other.m_RenderDeviceDX11;
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
		if (FAILED(m_RenderDeviceDX11.GetDeviceD3D11()->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState)))
		{
			Log::Error("Failed to create depth stencil state.");
		}

		m_bDirty = false;
	}

	m_RenderDeviceDX11.GetDeviceContextD3D11()->OMSetDepthStencilState(m_pDepthStencilState, m_StencilMode.StencilReference);
}

void DepthStencilStateDX11::Unbind()
{
	m_RenderDeviceDX11.GetDeviceContextD3D11()->OMSetDepthStencilState(NULL, 0);
}



//
// Translate
//
D3D11_DEPTH_WRITE_MASK TranslateDepthWriteMask(IDepthStencilState::DepthWrite depthWrite)
{
	D3D11_DEPTH_WRITE_MASK result = D3D11_DEPTH_WRITE_MASK_ALL;

	switch (depthWrite)
	{
	case IDepthStencilState::DepthWrite::Enable:
		result = D3D11_DEPTH_WRITE_MASK_ALL;
		break;
	case IDepthStencilState::DepthWrite::Disable:
		result = D3D11_DEPTH_WRITE_MASK_ZERO;
		break;
	default:
		Log::Error("Unknown depth write mask.");
		break;
	}

	return result;
}

D3D11_COMPARISON_FUNC TranslateCompareFunc(IDepthStencilState::CompareFunction compareFunc)
{
	D3D11_COMPARISON_FUNC result = D3D11_COMPARISON_LESS;

	switch (compareFunc)
	{
	case IDepthStencilState::CompareFunction::Never:
		result = D3D11_COMPARISON_NEVER;
		break;
	case IDepthStencilState::CompareFunction::Less:
		result = D3D11_COMPARISON_LESS;
		break;
	case IDepthStencilState::CompareFunction::Equal:
		result = D3D11_COMPARISON_EQUAL;
		break;
	case IDepthStencilState::CompareFunction::LessOrEqual:
		result = D3D11_COMPARISON_LESS_EQUAL;
		break;
	case IDepthStencilState::CompareFunction::Greater:
		result = D3D11_COMPARISON_GREATER;
		break;
	case IDepthStencilState::CompareFunction::NotEqual:
		result = D3D11_COMPARISON_NOT_EQUAL;
		break;
	case IDepthStencilState::CompareFunction::GreaterOrEqual:
		result = D3D11_COMPARISON_GREATER_EQUAL;
		break;
	case IDepthStencilState::CompareFunction::Always:
		result = D3D11_COMPARISON_ALWAYS;
		break;
	default:
		Log::Error("Unknown compare function.");
		break;
	}

	return result;
}

D3D11_STENCIL_OP TranslateStencilOperation(IDepthStencilState::StencilOperation stencilOperation)
{
	D3D11_STENCIL_OP result = D3D11_STENCIL_OP_KEEP;

	switch (stencilOperation)
	{
	case IDepthStencilState::StencilOperation::Keep:
		result = D3D11_STENCIL_OP_KEEP;
		break;
	case IDepthStencilState::StencilOperation::Zero:
		result = D3D11_STENCIL_OP_ZERO;
		break;
	case IDepthStencilState::StencilOperation::Reference:
		result = D3D11_STENCIL_OP_REPLACE;
		break;
	case IDepthStencilState::StencilOperation::IncrementClamp:
		result = D3D11_STENCIL_OP_INCR_SAT;
		break;
	case IDepthStencilState::StencilOperation::DecrementClamp:
		result = D3D11_STENCIL_OP_DECR_SAT;
		break;
	case IDepthStencilState::StencilOperation::Invert:
		result = D3D11_STENCIL_OP_INVERT;
		break;
	case IDepthStencilState::StencilOperation::IncrementWrap:
		result = D3D11_STENCIL_OP_INCR;
		break;
	case IDepthStencilState::StencilOperation::DecrementWrap:
		result = D3D11_STENCIL_OP_DECR;
		break;
	default:
		Log::Error("Unknown stencil operation.");
		break;
	}

	return result;
}

D3D11_DEPTH_STENCILOP_DESC TranslateFaceOperation(IDepthStencilState::FaceOperation faceOperation)
{
	D3D11_DEPTH_STENCILOP_DESC result;

	result.StencilFailOp = TranslateStencilOperation(faceOperation.StencilFail);
	result.StencilDepthFailOp = TranslateStencilOperation(faceOperation.StencilPassDepthFail);
	result.StencilPassOp = TranslateStencilOperation(faceOperation.StencilDepthPass);
	result.StencilFunc = TranslateCompareFunc(faceOperation.StencilFunction);

	return result;
}

D3D11_DEPTH_STENCIL_DESC TranslateDepthStencilState(const IDepthStencilState::DepthMode& depthMode, const IDepthStencilState::StencilMode& stencilMode)
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
