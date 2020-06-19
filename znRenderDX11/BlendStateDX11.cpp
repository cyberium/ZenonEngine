#include "stdafx.h"

// General
#include "BlendStateDX11.h"

// FORWARD BEGIN
D3D11_BLEND TranslateBlendFactor(IBlendState::BlendFactor blendFactor);
D3D11_BLEND_OP TranslateBlendOp(IBlendState::BlendOperation blendOperation);
UINT8 TranslateWriteMask(bool red, bool green, bool blue, bool alpha);
D3D11_LOGIC_OP TranslateLogicOperator(IBlendState::LogicOperator logicOp);
// FORWARD END

BlendStateDX11::BlendStateDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
{
	m_BlendModes.resize(8, BlendMode());
}

BlendStateDX11::BlendStateDX11(const BlendStateDX11& copy)
	: m_RenderDeviceDX11(copy.m_RenderDeviceDX11)
{
    m_BlendModes = copy.m_BlendModes;
    m_bAlphaToCoverageEnabled = copy.m_bAlphaToCoverageEnabled;
    m_bIndependentBlendEnabled = copy.m_bIndependentBlendEnabled;
    m_SampleMask = copy.m_SampleMask;
    m_ConstBlendFactor = copy.m_ConstBlendFactor;
    m_bDirty = true;
}

BlendStateDX11::~BlendStateDX11()
{
}

const BlendStateDX11& BlendStateDX11::operator=(const BlendStateDX11& other)
{
	if (this != &other)
	{
		m_BlendModes = other.m_BlendModes;
		m_bAlphaToCoverageEnabled = other.m_bAlphaToCoverageEnabled;
		m_bIndependentBlendEnabled = other.m_bIndependentBlendEnabled;
		m_SampleMask = other.m_SampleMask;
		m_ConstBlendFactor = other.m_ConstBlendFactor;
		m_RenderDeviceDX11 = other.m_RenderDeviceDX11;

		m_bDirty = true;
	}

	return *this;
}



//
// BlendState
//
void BlendStateDX11::Bind()
{
    if (m_bDirty)
    {
		D3D11_BLEND_DESC1 blendDesc = { 0 };
        blendDesc.AlphaToCoverageEnable = m_bAlphaToCoverageEnabled;
        blendDesc.IndependentBlendEnable = m_bIndependentBlendEnabled;

        for (uint32 i = 0; i < 8 && i < m_BlendModes.size(); i++)
        {
            D3D11_RENDER_TARGET_BLEND_DESC1& rtBlendDesc = blendDesc.RenderTarget[i];
            BlendMode& blendMode = m_BlendModes[i];

            rtBlendDesc.BlendEnable = blendMode.BlendEnabled;
            rtBlendDesc.LogicOpEnable = blendMode.LogicOpEnabled;
            rtBlendDesc.SrcBlend = TranslateBlendFactor(blendMode.SrcFactor);
            rtBlendDesc.DestBlend = TranslateBlendFactor(blendMode.DstFactor);
            rtBlendDesc.BlendOp = TranslateBlendOp(blendMode.BlendOp);
            rtBlendDesc.SrcBlendAlpha = TranslateBlendFactor(blendMode.SrcAlphaFactor);
            rtBlendDesc.DestBlendAlpha = TranslateBlendFactor(blendMode.DstAlphaFactor);
            rtBlendDesc.BlendOpAlpha = TranslateBlendOp(blendMode.AlphaOp);
            rtBlendDesc.LogicOp = TranslateLogicOperator(blendMode.LogicOp);
            rtBlendDesc.RenderTargetWriteMask = TranslateWriteMask(blendMode.WriteRed, blendMode.WriteGreen, blendMode.WriteBlue, blendMode.WriteAlpha);
        }

		m_BlendState = NULL;
		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateBlendState1(&blendDesc, &m_BlendState));

        m_bDirty = false;
    }

    m_RenderDeviceDX11.GetDeviceContextD3D11()->OMSetBlendState(m_BlendState, glm::value_ptr(m_ConstBlendFactor), m_SampleMask);
}

void BlendStateDX11::Unbind()
{
	m_RenderDeviceDX11.GetDeviceContextD3D11()->OMSetBlendState(NULL, NULL, 0);
}



//
// Translate
//
D3D11_BLEND TranslateBlendFactor(IBlendState::BlendFactor blendFactor)
{
	D3D11_BLEND result = D3D11_BLEND_ONE;

	switch (blendFactor)
	{
	case IBlendState::BlendFactor::Zero:
		result = D3D11_BLEND_ZERO;
		break;
	case IBlendState::BlendFactor::One:
		result = D3D11_BLEND_ONE;
		break;
	case IBlendState::BlendFactor::SrcColor:
		result = D3D11_BLEND_SRC_COLOR;
		break;
	case IBlendState::BlendFactor::OneMinusSrcColor:
		result = D3D11_BLEND_INV_SRC_COLOR;
		break;
	case IBlendState::BlendFactor::DstColor:
		result = D3D11_BLEND_DEST_COLOR;
		break;
	case IBlendState::BlendFactor::OneMinusDstColor:
		result = D3D11_BLEND_INV_DEST_COLOR;
		break;
	case IBlendState::BlendFactor::SrcAlpha:
		result = D3D11_BLEND_SRC_ALPHA;
		break;
	case IBlendState::BlendFactor::OneMinusSrcAlpha:
		result = D3D11_BLEND_INV_SRC_ALPHA;
		break;
	case IBlendState::BlendFactor::DstAlpha:
		result = D3D11_BLEND_DEST_ALPHA;
		break;
	case IBlendState::BlendFactor::OneMinusDstAlpha:
		result = D3D11_BLEND_INV_DEST_ALPHA;
		break;
	case IBlendState::BlendFactor::SrcAlphaSat:
		result = D3D11_BLEND_SRC_ALPHA_SAT;
		break;
	case IBlendState::BlendFactor::ConstBlendFactor:
		result = D3D11_BLEND_BLEND_FACTOR;
		break;
	case IBlendState::BlendFactor::OneMinusBlendFactor:
		result = D3D11_BLEND_INV_BLEND_FACTOR;
		break;
	case IBlendState::BlendFactor::Src1Color:
		result = D3D11_BLEND_SRC1_COLOR;
		break;
	case IBlendState::BlendFactor::OneMinusSrc1Color:
		result = D3D11_BLEND_INV_SRC1_COLOR;
		break;
	case IBlendState::BlendFactor::Src1Alpha:
		result = D3D11_BLEND_INV_SRC1_ALPHA;
		break;
	case IBlendState::BlendFactor::OneMinusSrc1Alpha:
		result = D3D11_BLEND_INV_SRC1_ALPHA;
		break;
	default:
        throw CException("Unknown blend factor.");
	}

	return result;
}

D3D11_BLEND_OP TranslateBlendOp(IBlendState::BlendOperation blendOperation)
{
	D3D11_BLEND_OP result = D3D11_BLEND_OP_ADD;
	switch (blendOperation)
	{
	case IBlendState::BlendOperation::Add:
		result = D3D11_BLEND_OP_ADD;
		break;
	case IBlendState::BlendOperation::Subtract:
		result = D3D11_BLEND_OP_SUBTRACT;
		break;
	case IBlendState::BlendOperation::ReverseSubtract:
		result = D3D11_BLEND_OP_REV_SUBTRACT;
		break;
	case IBlendState::BlendOperation::Min:
		result = D3D11_BLEND_OP_MIN;
		break;
	case IBlendState::BlendOperation::Max:
		result = D3D11_BLEND_OP_MAX;
		break;
	default:
		throw CException("Unknown blend operation.");
	}

	return result;
}

UINT8 TranslateWriteMask(bool red, bool green, bool blue, bool alpha)
{
	UINT8 writeMask = 0;
	if (red)
	{
		writeMask |= D3D11_COLOR_WRITE_ENABLE_RED;
	}
	if (green)
	{
		writeMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
	}
	if (blue)
	{
		writeMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
	}
	if (alpha)
	{
		writeMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
	}

	return writeMask;
}

D3D11_LOGIC_OP TranslateLogicOperator(IBlendState::LogicOperator logicOp)
{
	D3D11_LOGIC_OP result = D3D11_LOGIC_OP_NOOP;

	switch (logicOp)
	{
	case IBlendState::LogicOperator::None:
		result = D3D11_LOGIC_OP_NOOP;
		break;
	case IBlendState::LogicOperator::Clear:
		result = D3D11_LOGIC_OP_CLEAR;
		break;
	case IBlendState::LogicOperator::Set:
		result = D3D11_LOGIC_OP_SET;
		break;
	case IBlendState::LogicOperator::Copy:
		result = D3D11_LOGIC_OP_SET;
		break;
	case IBlendState::LogicOperator::CopyInverted:
		result = D3D11_LOGIC_OP_COPY_INVERTED;
		break;
	case IBlendState::LogicOperator::Invert:
		result = D3D11_LOGIC_OP_INVERT;
		break;
	case IBlendState::LogicOperator::And:
		result = D3D11_LOGIC_OP_AND;
		break;
	case IBlendState::LogicOperator::Nand:
		result = D3D11_LOGIC_OP_NAND;
		break;
	case IBlendState::LogicOperator::Or:
		result = D3D11_LOGIC_OP_OR;
		break;
	case IBlendState::LogicOperator::Nor:
		result = D3D11_LOGIC_OP_NOR;
		break;
	case IBlendState::LogicOperator::Xor:
		result = D3D11_LOGIC_OP_XOR;
		break;
	case IBlendState::LogicOperator::Equiv:
		result = D3D11_LOGIC_OP_EQUIV;
		break;
	case IBlendState::LogicOperator::AndReverse:
		result = D3D11_LOGIC_OP_AND_REVERSE;
		break;
	case IBlendState::LogicOperator::AndInverted:
		result = D3D11_LOGIC_OP_AND_INVERTED;
		break;
	case IBlendState::LogicOperator::OrReverse:
		result = D3D11_LOGIC_OP_OR_REVERSE;
		break;
	case IBlendState::LogicOperator::OrInverted:
		result = D3D11_LOGIC_OP_OR_INVERTED;
		break;
	default:
        throw CException("Unknown LogicOperator.");
	}

	return result;
}
