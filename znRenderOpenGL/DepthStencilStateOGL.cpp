#include <stdafx.h>

// General
#include "DepthStencilStateOGL.h"

// FORWARD BEGIN
GLboolean GLTranslateDepthWriteMask(IDepthStencilState::DepthWrite depthWrite);
GLenum GLTranslateCompareFunc(IDepthStencilState::CompareFunction compareFunc);
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
GLboolean GLTranslateDepthWriteMask(IDepthStencilState::DepthWrite depthWrite)
{
    GLboolean result = GL_TRUE;

    switch (depthWrite)
    {
        case IDepthStencilState::DepthWrite::Enable:
            result = GL_TRUE;
            break;
        case IDepthStencilState::DepthWrite::Disable:
            result = GL_FALSE;
            break;
        default:
            std::exception("Unknown depth write mask.");
    }

    return result;
}

GLenum GLTranslateCompareFunc(IDepthStencilState::CompareFunction compareFunc)
{
    GLenum result = GL_LESS;

    switch (compareFunc)
    {
        case IDepthStencilState::CompareFunction::Never:
            result = GL_NEVER;
            break;
        case IDepthStencilState::CompareFunction::Less:
            result = GL_LESS;
            break;
        case IDepthStencilState::CompareFunction::Equal:
            result = GL_EQUAL;
            break;
        case IDepthStencilState::CompareFunction::LessOrEqual:
            result = GL_LEQUAL;
            break;
        case IDepthStencilState::CompareFunction::Greater:
            result = GL_GREATER;
            break;
        case IDepthStencilState::CompareFunction::NotEqual:
            result = GL_NOTEQUAL;
            break;
        case IDepthStencilState::CompareFunction::GreaterOrEqual:
            result = GL_GEQUAL;
            break;
        case IDepthStencilState::CompareFunction::Always:
            result = GL_ALWAYS;
            break;
        default:
            std::exception("Unknown depth write mask.");
    }

    return result;
}