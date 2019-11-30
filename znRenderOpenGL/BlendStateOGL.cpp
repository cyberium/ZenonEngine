#include <stdafx.h>

// General
#include "BlendStateOGL.h"

// FORWARD BEGIN
GLenum TranslateBlendFactor(IBlendState::BlendFactor blendFactor);
GLenum TranslateBlendOp(IBlendState::BlendOperation blendOperation);
UINT8 TranslateWriteMaskOGL(bool red, bool green, bool blue, bool alpha);
GLenum TranslateLogicOperator(IBlendState::LogicOperator logicOp);
// FORWARD END

BlendStateOGL::BlendStateOGL()
{
	m_BlendModes.resize(8, BlendMode());
}

BlendStateOGL::BlendStateOGL(const BlendStateOGL& copy)
{
    m_BlendModes = copy.m_BlendModes;
    m_bAlphaToCoverageEnabled = copy.m_bAlphaToCoverageEnabled;
    m_bIndependentBlendEnabled = copy.m_bIndependentBlendEnabled;
    m_SampleMask = copy.m_SampleMask;
    m_ConstBlendFactor = copy.m_ConstBlendFactor;
    m_bDirty = true;
}

BlendStateOGL::~BlendStateOGL()
{

}

const BlendStateOGL& BlendStateOGL::operator=(const BlendStateOGL& other)
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
void BlendStateOGL::Bind()
{
	BlendMode& blendMode = m_BlendModes[0];

	// Blend state
	if (blendMode.BlendEnabled)
	{
		glEnable(GL_BLEND);
		glBlendFuncSeparate(
			TranslateBlendFactor(blendMode.SrcFactor),
			TranslateBlendFactor(blendMode.DstFactor),
			TranslateBlendFactor(blendMode.SrcAlphaFactor),
			TranslateBlendFactor(blendMode.DstAlphaFactor)
		);
	}
	else
	{
		glDisable(GL_BLEND);
	}

	// Color mask
	glColorMask(
		blendMode.WriteRed ? GL_TRUE : GL_FALSE, 
		blendMode.WriteGreen ? GL_TRUE : GL_FALSE, 
		blendMode.WriteBlue ? GL_TRUE : GL_FALSE, 
		blendMode.WriteAlpha ? GL_TRUE : GL_FALSE
	);

	if (m_SampleMask != 0)
	{
		glEnable(GL_SAMPLE_MASK);
		glSampleMaski(0, m_SampleMask);
	}
	else
	{
		glDisable(GL_SAMPLE_MASK);
	}


	// Alpha to coverage
	if (m_bAlphaToCoverageEnabled)
	{
		glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}
	else
	{
		glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
	}
}



//
// Translate
//
GLenum TranslateBlendFactor(IBlendState::BlendFactor blendFactor)
{
    GLenum result = GL_ONE;

    switch (blendFactor)
    {
        case IBlendState::BlendFactor::Zero:
            result = GL_ZERO;
            break;
        case IBlendState::BlendFactor::One:
            result = GL_ONE;
            break;
        case IBlendState::BlendFactor::SrcColor:
            result = GL_SRC_COLOR;
            break;
        case IBlendState::BlendFactor::OneMinusSrcColor:
            result = GL_ONE_MINUS_SRC_COLOR;
            break;
        case IBlendState::BlendFactor::DstColor:
            result = GL_DST_COLOR;
            break;
        case IBlendState::BlendFactor::OneMinusDstColor:
            result = GL_ONE_MINUS_DST_COLOR;
            break;
        case IBlendState::BlendFactor::SrcAlpha:
            result = GL_SRC_ALPHA;
            break;
        case IBlendState::BlendFactor::OneMinusSrcAlpha:
            result = GL_ONE_MINUS_SRC_ALPHA;
            break;
        case IBlendState::BlendFactor::DstAlpha:
            result = GL_DST_ALPHA;
            break;
        case IBlendState::BlendFactor::OneMinusDstAlpha:
            result = GL_ONE_MINUS_DST_ALPHA;
            break;
        case IBlendState::BlendFactor::SrcAlphaSat:
            result = GL_SRC_ALPHA_SATURATE;
            break;
        case IBlendState::BlendFactor::ConstBlendFactor:
            throw std::exception("Not implemented!");
            break;
        case IBlendState::BlendFactor::OneMinusBlendFactor:
            throw std::exception("Not implemented!");
            break;
        case IBlendState::BlendFactor::Src1Color:
            result = GL_SRC1_COLOR;
            break;
        case IBlendState::BlendFactor::OneMinusSrc1Color:
            result = GL_ONE_MINUS_SRC1_COLOR;
            break;
        case IBlendState::BlendFactor::Src1Alpha:
            throw std::exception("Not implemented!");
            break;
        case IBlendState::BlendFactor::OneMinusSrc1Alpha:
            result = GL_ONE_MINUS_SRC1_ALPHA;
            break;
        default:
            Log::Error("Unknown blend factor.");
            break;
    }

    return result;
}

GLenum TranslateBlendOp(IBlendState::BlendOperation blendOperation)
{
    /*D3D11_BLEND_OP result = D3D11_BLEND_OP_ADD;
    switch (blendOperation)
    {
    case BlendOperation::Add:
        result = D3D11_BLEND_OP_ADD;
        break;
    case BlendOperation::Subtract:
        result = D3D11_BLEND_OP_SUBTRACT;
        break;
    case BlendOperation::ReverseSubtract:
        result = D3D11_BLEND_OP_REV_SUBTRACT;
        break;
    case BlendOperation::Min:
        result = D3D11_BLEND_OP_MIN;
        break;
    case BlendOperation::Max:
        result = D3D11_BLEND_OP_MAX;
        break;
    default:
        Log::Error("Unknown blend operation.");
        break;
    }*/

    throw std::exception("Not implemented!");
}

UINT8 TranslateWriteMaskOGL(bool red, bool green, bool blue, bool alpha)
{
    /*UINT8 writeMask = 0;
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
    }*/
    throw std::exception("Not implemented!");
}

GLenum TranslateLogicOperator(IBlendState::LogicOperator logicOp)
{
    /*D3D11_LOGIC_OP result = D3D11_LOGIC_OP_NOOP;

    switch (logicOp)
    {
    case LogicOperator::None:
        result = D3D11_LOGIC_OP_NOOP;
        break;
    case LogicOperator::Clear:
        result = D3D11_LOGIC_OP_CLEAR;
        break;
    case LogicOperator::Set:
        result = D3D11_LOGIC_OP_SET;
        break;
    case LogicOperator::Copy:
        result = D3D11_LOGIC_OP_SET;
        break;
    case LogicOperator::CopyInverted:
        result = D3D11_LOGIC_OP_COPY_INVERTED;
        break;
    case LogicOperator::Invert:
        result = D3D11_LOGIC_OP_INVERT;
        break;
    case LogicOperator::And:
        result = D3D11_LOGIC_OP_AND;
        break;
    case LogicOperator::Nand:
        result = D3D11_LOGIC_OP_NAND;
        break;
    case LogicOperator::Or:
        result = D3D11_LOGIC_OP_OR;
        break;
    case LogicOperator::Nor:
        result = D3D11_LOGIC_OP_NOR;
        break;
    case LogicOperator::Xor:
        result = D3D11_LOGIC_OP_XOR;
        break;
    case LogicOperator::Equiv:
        result = D3D11_LOGIC_OP_EQUIV;
        break;
    case LogicOperator::AndReverse:
        result = D3D11_LOGIC_OP_AND_REVERSE;
        break;
    case LogicOperator::AndInverted:
        result = D3D11_LOGIC_OP_AND_INVERTED;
        break;
    case LogicOperator::OrReverse:
        result = D3D11_LOGIC_OP_OR_REVERSE;
        break;
    case LogicOperator::OrInverted:
        result = D3D11_LOGIC_OP_OR_INVERTED;
        break;
    default:
        break;
    }*/
    throw std::exception("Not implemented!");
}