#include <stdafx.h>

// General
#include "BlendStateDX11.h"

// FORWARD BEGIN
D3D11_BLEND                                     TranslateBlendFactor(BlendState::BlendFactor blendFactor);
D3D11_BLEND_OP                                  TranslateBlendOp(BlendState::BlendOperation blendOperation);
UINT8                                           TranslateWriteMask(bool red, bool green, bool blue, bool alpha);
D3D11_LOGIC_OP                                  TranslateLogicOperator(BlendState::LogicOperator logicOp);
// FORWARD END

BlendStateDX11::BlendStateDX11(ID3D11Device2* pDevice)
	: m_pDevice(pDevice)
{
	if (m_pDevice)
		m_pDevice->GetImmediateContext2(&m_pDeviceContext);

	m_BlendModes.resize(8, BlendMode());
}

BlendStateDX11::BlendStateDX11(const BlendStateDX11& copy)
	: m_pDevice(copy.m_pDevice)
	, m_pDeviceContext(copy.m_pDeviceContext)
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
	// Avoid copy to self..
	if (this != &other)
	{
		m_BlendModes = other.m_BlendModes;
		m_bAlphaToCoverageEnabled = other.m_bAlphaToCoverageEnabled;
		m_bIndependentBlendEnabled = other.m_bIndependentBlendEnabled;
		m_SampleMask = other.m_SampleMask;
		m_ConstBlendFactor = other.m_ConstBlendFactor;

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
        D3D11_BLEND_DESC1 blendDesc;

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

        m_pBlendState = NULL;
        m_pDevice->CreateBlendState1(&blendDesc, &m_pBlendState);

        m_bDirty = false;
    }

    m_pDeviceContext->OMSetBlendState(m_pBlendState, glm::value_ptr(m_ConstBlendFactor), m_SampleMask);
}



//
// Translate
//
D3D11_BLEND TranslateBlendFactor(BlendState::BlendFactor blendFactor)
{
	D3D11_BLEND result = D3D11_BLEND_ONE;

	switch (blendFactor)
	{
	case BlendState::BlendFactor::Zero:
		result = D3D11_BLEND_ZERO;
		break;
	case BlendState::BlendFactor::One:
		result = D3D11_BLEND_ONE;
		break;
	case BlendState::BlendFactor::SrcColor:
		result = D3D11_BLEND_SRC_COLOR;
		break;
	case BlendState::BlendFactor::OneMinusSrcColor:
		result = D3D11_BLEND_INV_SRC_COLOR;
		break;
	case BlendState::BlendFactor::DstColor:
		result = D3D11_BLEND_DEST_COLOR;
		break;
	case BlendState::BlendFactor::OneMinusDstColor:
		result = D3D11_BLEND_INV_DEST_COLOR;
		break;
	case BlendState::BlendFactor::SrcAlpha:
		result = D3D11_BLEND_SRC_ALPHA;
		break;
	case BlendState::BlendFactor::OneMinusSrcAlpha:
		result = D3D11_BLEND_INV_SRC_ALPHA;
		break;
	case BlendState::BlendFactor::DstAlpha:
		result = D3D11_BLEND_DEST_ALPHA;
		break;
	case BlendState::BlendFactor::OneMinusDstAlpha:
		result = D3D11_BLEND_INV_DEST_ALPHA;
		break;
	case BlendState::BlendFactor::SrcAlphaSat:
		result = D3D11_BLEND_SRC_ALPHA_SAT;
		break;
	case BlendState::BlendFactor::ConstBlendFactor:
		result = D3D11_BLEND_BLEND_FACTOR;
		break;
	case BlendState::BlendFactor::OneMinusBlendFactor:
		result = D3D11_BLEND_INV_BLEND_FACTOR;
		break;
	case BlendState::BlendFactor::Src1Color:
		result = D3D11_BLEND_SRC1_COLOR;
		break;
	case BlendState::BlendFactor::OneMinusSrc1Color:
		result = D3D11_BLEND_INV_SRC1_COLOR;
		break;
	case BlendState::BlendFactor::Src1Alpha:
		result = D3D11_BLEND_INV_SRC1_ALPHA;
		break;
	case BlendState::BlendFactor::OneMinusSrc1Alpha:
		result = D3D11_BLEND_INV_SRC1_ALPHA;
		break;
	default:
        throw std::exception("Unknown blend factor.");
	}

	return result;
}

D3D11_BLEND_OP TranslateBlendOp(BlendState::BlendOperation blendOperation)
{
	D3D11_BLEND_OP result = D3D11_BLEND_OP_ADD;
	switch (blendOperation)
	{
	case BlendState::BlendOperation::Add:
		result = D3D11_BLEND_OP_ADD;
		break;
	case BlendState::BlendOperation::Subtract:
		result = D3D11_BLEND_OP_SUBTRACT;
		break;
	case BlendState::BlendOperation::ReverseSubtract:
		result = D3D11_BLEND_OP_REV_SUBTRACT;
		break;
	case BlendState::BlendOperation::Min:
		result = D3D11_BLEND_OP_MIN;
		break;
	case BlendState::BlendOperation::Max:
		result = D3D11_BLEND_OP_MAX;
		break;
	default:
		throw std::exception("Unknown blend operation.");
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

D3D11_LOGIC_OP TranslateLogicOperator(BlendState::LogicOperator logicOp)
{
	D3D11_LOGIC_OP result = D3D11_LOGIC_OP_NOOP;

	switch (logicOp)
	{
	case BlendState::LogicOperator::None:
		result = D3D11_LOGIC_OP_NOOP;
		break;
	case BlendState::LogicOperator::Clear:
		result = D3D11_LOGIC_OP_CLEAR;
		break;
	case BlendState::LogicOperator::Set:
		result = D3D11_LOGIC_OP_SET;
		break;
	case BlendState::LogicOperator::Copy:
		result = D3D11_LOGIC_OP_SET;
		break;
	case BlendState::LogicOperator::CopyInverted:
		result = D3D11_LOGIC_OP_COPY_INVERTED;
		break;
	case BlendState::LogicOperator::Invert:
		result = D3D11_LOGIC_OP_INVERT;
		break;
	case BlendState::LogicOperator::And:
		result = D3D11_LOGIC_OP_AND;
		break;
	case BlendState::LogicOperator::Nand:
		result = D3D11_LOGIC_OP_NAND;
		break;
	case BlendState::LogicOperator::Or:
		result = D3D11_LOGIC_OP_OR;
		break;
	case BlendState::LogicOperator::Nor:
		result = D3D11_LOGIC_OP_NOR;
		break;
	case BlendState::LogicOperator::Xor:
		result = D3D11_LOGIC_OP_XOR;
		break;
	case BlendState::LogicOperator::Equiv:
		result = D3D11_LOGIC_OP_EQUIV;
		break;
	case BlendState::LogicOperator::AndReverse:
		result = D3D11_LOGIC_OP_AND_REVERSE;
		break;
	case BlendState::LogicOperator::AndInverted:
		result = D3D11_LOGIC_OP_AND_INVERTED;
		break;
	case BlendState::LogicOperator::OrReverse:
		result = D3D11_LOGIC_OP_OR_REVERSE;
		break;
	case BlendState::LogicOperator::OrInverted:
		result = D3D11_LOGIC_OP_OR_INVERTED;
		break;
	default:
        throw std::exception("Unknown LogicOperator.");
		break;
	}

	return result;
}
