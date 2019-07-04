#include <stdafx.h>

// General
#include "DepthStencilStateOGL.h"

// FORWARD BEGIN
GLboolean GLTranslateDepthWriteMask(DepthStencilState::DepthWrite depthWrite);
GLenum GLTranslateCompareFunc(DepthStencilState::CompareFunction compareFunc);
// FORWARD END

DepthStencilStateOGL::DepthStencilStateOGL()
{
}

DepthStencilStateOGL::DepthStencilStateOGL(const DepthStencilStateOGL& copy)
{
    m_DepthMode = copy.m_DepthMode;
    m_StencilMode = copy.m_StencilMode;
    m_bDirty = true;
}

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



//
// DepthStencilState
//
void DepthStencilStateOGL::Bind()
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
}



//
// Translate
//
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