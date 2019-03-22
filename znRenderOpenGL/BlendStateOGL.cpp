#include <stdafx.h>

// General
#include "BlendStateOGL.h"

GLenum TranslateBlendFactor(BlendState::BlendFactor blendFactor)
{
	GLenum result = GL_ONE;

	switch (blendFactor)
	{
	case BlendState::BlendFactor::Zero:
		result = GL_ZERO;
		break;
	case BlendState::BlendFactor::One:
		result = GL_ONE;
		break;
	case BlendState::BlendFactor::SrcColor:
		result = GL_SRC_COLOR;
		break;
	case BlendState::BlendFactor::OneMinusSrcColor:
		result = GL_ONE_MINUS_SRC_COLOR;
		break;
	case BlendState::BlendFactor::DstColor:
		result = GL_DST_COLOR;
		break;
	case BlendState::BlendFactor::OneMinusDstColor:
		result = GL_ONE_MINUS_DST_COLOR;
		break;
	case BlendState::BlendFactor::SrcAlpha:
		result = GL_SRC_ALPHA;
		break;
	case BlendState::BlendFactor::OneMinusSrcAlpha:
		result = GL_ONE_MINUS_SRC_ALPHA;
		break;
	case BlendState::BlendFactor::DstAlpha:
		result = GL_DST_ALPHA;
		break;
	case BlendState::BlendFactor::OneMinusDstAlpha:
		result = GL_ONE_MINUS_DST_ALPHA;
		break;
	case BlendState::BlendFactor::SrcAlphaSat:
		result = GL_SRC_ALPHA_SATURATE;
		break;
	case BlendState::BlendFactor::ConstBlendFactor:
		throw std::exception("Not implemented!");
		break;
	case BlendState::BlendFactor::OneMinusBlendFactor:
		throw std::exception("Not implemented!");
		break;
	case BlendState::BlendFactor::Src1Color:
		result = GL_SRC1_COLOR;
		break;
	case BlendState::BlendFactor::OneMinusSrc1Color:
		result = GL_ONE_MINUS_SRC1_COLOR;
		break;
	case BlendState::BlendFactor::Src1Alpha:
		throw std::exception("Not implemented!");
		break;
	case BlendState::BlendFactor::OneMinusSrc1Alpha:
		result = GL_ONE_MINUS_SRC1_ALPHA;
		break;
	default:
		Log::Error("Unknown blend factor.");
		break;
	}

	return result;
}

GLenum TranslateBlendOp(BlendState::BlendOperation blendOperation)
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

UINT8 TranslateWriteMask(bool red, bool green, bool blue, bool alpha)
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

GLenum TranslateLogicOperator(BlendState::LogicOperator logicOp)
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

BlendStateOGL::BlendStateOGL()
	: m_bAlphaToCoverageEnabled(false)
	, m_bIndependentBlendEnabled(false)
	, m_SampleMask(0xffffffff)
	, m_ConstBlendFactor(1)
	, m_bDirty(true)
{
	m_BlendModes.resize(8, BlendMode());
}

BlendStateOGL::BlendStateOGL(const BlendStateOGL& copy)
	: m_BlendModes(copy.m_BlendModes)
	, m_bAlphaToCoverageEnabled(copy.m_bAlphaToCoverageEnabled)
	, m_bIndependentBlendEnabled(copy.m_bIndependentBlendEnabled)
	, m_SampleMask(copy.m_SampleMask)
	, m_ConstBlendFactor(copy.m_ConstBlendFactor)
	, m_bDirty(true)
{

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

void BlendStateOGL::SetBlendMode(const BlendState::BlendMode& blendMode)
{
	m_BlendModes[0] = blendMode;
	m_bDirty = true;
}

void BlendStateOGL::SetBlendModes(const std::vector<BlendMode>& blendModes)
{
	m_BlendModes = blendModes;
	m_bDirty;
}

const std::vector<BlendState::BlendMode>& BlendStateOGL::GetBlendModes() const
{
	return m_BlendModes;
}

void BlendStateOGL::SetAlphaCoverage(bool enabled)
{
	if (m_bAlphaToCoverageEnabled != enabled)
	{
		m_bAlphaToCoverageEnabled = enabled;
		m_bDirty = true;
	}
}

bool BlendStateOGL::GetAlphaCoverage() const
{
	return m_bAlphaToCoverageEnabled;
}

void BlendStateOGL::SetIndependentBlend(bool enabled)
{
	if (m_bAlphaToCoverageEnabled != enabled)
	{
		m_bAlphaToCoverageEnabled = enabled;
		m_bDirty = true;
	}

	throw std::exception("Not implemented!");
}

bool BlendStateOGL::GetIndependentBlend() const
{
	throw std::exception("Not implemented!");

	return m_bIndependentBlendEnabled;
}

void BlendStateOGL::SetConstBlendFactor(cvec4 constantBlendFactor)
{
	m_ConstBlendFactor = constantBlendFactor;
	// No need to set the dirty flag as this value is not used to create the blend state object. It is only used when activating the blend state of the output merger.
	throw std::exception("Not implemented!");
}

cvec4 BlendStateOGL::GetConstBlendFactor() const
{
	throw std::exception("Not implemented!");

	return m_ConstBlendFactor;
}

void BlendStateOGL::SetSampleMask(uint32_t sampleMask)
{
	m_SampleMask = sampleMask;
	// No need to set the dirty flag as this value is not used to create the blend state object. It is only used when activating the blend state of the output merger.
}

uint32_t BlendStateOGL::GetSampleMask() const
{
	throw std::exception("Not implemented!");

	return m_SampleMask;
}

void BlendStateOGL::Bind()
{
	//if (m_bDirty)
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

		/*if (m_SampleMask != 0)
		{
			glEnable(GL_SAMPLE_MASK);
			glSampleMaski(0, m_SampleMask);
		}
		else
		{
			glDisable(GL_SAMPLE_MASK);
		}*/


		// Alpha to coverage
		if (m_bAlphaToCoverageEnabled)
		{
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		else
		{
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}

		m_bDirty = false;
	}
}

