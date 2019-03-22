#include <stdafx.h>

// General
#include "DepthStencilStateOGL.h"

GLboolean GLTranslateDepthWriteMask(DepthStencilState::DepthWrite depthWrite)
{
	GLboolean result = GL_TRUE;

	switch (depthWrite)
	{
	case DepthStencilState::DepthWrite::Enable:
		result = GL_TRUE;
		break;
	case DepthStencilState::DepthWrite::Disable:
		result = GL_FALSE;
		break;
	default:
		std::exception("Unknown depth write mask.");
	}

	return result;
}

GLenum GLTranslateCompareFunc(DepthStencilState::CompareFunction compareFunc)
{
	GLenum result = GL_LESS;

	switch (compareFunc)
	{
	case DepthStencilState::CompareFunction::Never:
		result = GL_NEVER;
		break;
	case DepthStencilState::CompareFunction::Less:
		result = GL_LESS;
		break;
	case DepthStencilState::CompareFunction::Equal:
		result = GL_EQUAL;
		break;
	case DepthStencilState::CompareFunction::LessOrEqual:
		result = GL_LEQUAL;
		break;
	case DepthStencilState::CompareFunction::Greater:
		result = GL_GREATER;
		break;
	case DepthStencilState::CompareFunction::NotEqual:
		result = GL_NOTEQUAL;
		break;
	case DepthStencilState::CompareFunction::GreaterOrEqual:
		result = GL_GEQUAL;
		break;
	case DepthStencilState::CompareFunction::Always:
		result = GL_ALWAYS;
		break;
	default:
		std::exception("Unknown depth write mask.");
	}

	return result;
}

/*GLenum TranslateStencilOperation(DepthStencilState::StencilOperation stencilOperation)
{
	GLenum result = D3D11_STENCIL_OP_KEEP;

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
}*/

/*GLenum TranslateFaceOperation(DepthStencilState::FaceOperation faceOperation)
{
	D3D11_DEPTH_STENCILOP_DESC result;

	result.StencilFailOp = TranslateStencilOperation(faceOperation.StencilFail);
	result.StencilDepthFailOp = TranslateStencilOperation(faceOperation.StencilPassDepthFail);
	result.StencilPassOp = TranslateStencilOperation(faceOperation.StencilDepthPass);
	result.StencilFunc = TranslateCompareFunc(faceOperation.StencilFunction);

	return result;
}*/

/*GLenum TranslateDepthStencilState(const DepthStencilState::DepthMode& depthMode, const DepthStencilState::StencilMode& stencilMode) const
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
}*/

DepthStencilStateOGL::DepthStencilStateOGL()
	: m_bDirty(true)
{
}

DepthStencilStateOGL::DepthStencilStateOGL(const DepthStencilStateOGL& copy)
	: m_DepthMode(copy.m_DepthMode)
	, m_StencilMode(copy.m_StencilMode)
	, m_bDirty(true)
{}

DepthStencilStateOGL::~DepthStencilStateOGL()
{}

const DepthStencilStateOGL& DepthStencilStateOGL::operator=(const DepthStencilStateOGL& other)
{
	if (this != &other)
	{
		m_DepthMode = other.m_DepthMode;
		m_StencilMode = other.m_StencilMode;
		m_bDirty = true;
	}

	return *this;
}

void DepthStencilStateOGL::SetDepthMode(const DepthMode& depthMode)
{
	m_DepthMode = depthMode;
	m_bDirty = true;
}

const DepthStencilState::DepthMode& DepthStencilStateOGL::GetDepthMode() const
{
	return m_DepthMode;
}

void DepthStencilStateOGL::SetStencilMode(const StencilMode& stencilMode)
{
	m_StencilMode = stencilMode;
	m_bDirty = true;
}

const DepthStencilState::StencilMode& DepthStencilStateOGL::GetStencilMode() const
{
	return m_StencilMode;
}

void DepthStencilStateOGL::Bind()
{
	//if (m_bDirty)
	{
		
		// DEPTH
		{
			// Depth mode
			if (m_DepthMode.DepthEnable)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GLTranslateCompareFunc(m_DepthMode.DepthFunction));
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}

			// Depth mask
			glDepthMask(GLTranslateDepthWriteMask(m_DepthMode.DepthWriteMask));
		}

		// STENCIL
		/*{
			if (m_StencilMode.StencilEnabled)
			{
				glEnable(GL_STENCIL_TEST);
			}
			else
			{
				glDisable(GL_STENCIL_TEST);
			}
		//glStencilFunc(GLTranslateCompareFunc(m_StencilMode.));

		// (GLenum face, GLuint mask);
			//GL_FRONT, GL_BACK, and GL_FRONT_AND_BACK.
			//glStencilMaskSeparate(GL_FRONT_AND_BACK, m_StencilMode.BackFace.StencilFail)
		}*/
		

		m_bDirty = false;
	}
}