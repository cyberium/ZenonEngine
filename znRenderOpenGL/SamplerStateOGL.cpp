#include <stdafx.h>

// General
#include "SamplerStateOGL.h"

// FORWARD BEGIN
GLenum GLTranslateMinFilter(SamplerStateOGL::MinFilter _minFilter);
GLenum GLTranslateMinMipFilter(SamplerStateOGL::MinFilter _minFilter, SamplerStateOGL::MipFilter _mipFilter);
GLenum GLTranslateMagFilter(SamplerStateOGL::MagFilter _magFilter);
GLenum GLTranslateWrapMode(SamplerStateOGL::WrapMode wrapMode);
GLenum GLTranslateCompareMode(SamplerStateOGL::CompareMode mode);
GLenum GLTranslateCompareFunction(SamplerStateOGL::CompareFunc compareFunc);
// FORWARD END

SamplerStateOGL::SamplerStateOGL()
{
	glGenSamplers(1, &m_GLObj);
}

SamplerStateOGL::~SamplerStateOGL()
{
	glDeleteSamplers(1, &m_GLObj);
}



void SamplerStateOGL::Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const 
{
	if (m_bIsDirty)
	{
        //glBindSampler(ID, m_GLObj);

		glSamplerParameteri(m_GLObj, GL_TEXTURE_MIN_FILTER, GLTranslateMinFilter(m_MinFilter));
		//glSamplerParameteri(m_GLObj, GL_TEXTURE_MIN_FILTER, GLTranslateMinMipFilter(m_MinFilter, m_MipFilter));
		glSamplerParameteri(m_GLObj, GL_TEXTURE_MAG_FILTER, GLTranslateMagFilter(m_MagFilter));

		//glSamplerParameteri(m_GLObj, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_AnisotropicFiltering);

		// Lod
		//glSamplerParameterf(m_GLObj, GL_TEXTURE_MIN_LOD, m_fMinLOD);
		//glSamplerParameterf(m_GLObj, GL_TEXTURE_MAX_LOD, m_fMaxLOD);
		//glSamplerParameterf(m_GLObj, GL_TEXTURE_LOD_BIAS, m_fLODBias);
		
		// Wrap
		glSamplerParameteri(m_GLObj, GL_TEXTURE_WRAP_S, GLTranslateWrapMode(m_WrapModeU));
		glSamplerParameteri(m_GLObj, GL_TEXTURE_WRAP_T, GLTranslateWrapMode(m_WrapModeV));
		glSamplerParameteri(m_GLObj, GL_TEXTURE_WRAP_R, GLTranslateWrapMode(m_WrapModeW));

		/*if (m_CompareMode != CompareMode::None)
		{
			glSamplerParameteri(m_GLObj, GL_TEXTURE_COMPARE_MODE, GLTranslateCompareMode(m_CompareMode));
			glSamplerParameteri(m_GLObj, GL_TEXTURE_COMPARE_FUNC, GLTranslateCompareFunction(m_CompareFunc));
		}
		else
		{
			glSamplerParameteri(m_GLObj, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}*/

        //glBindSampler(ID, 0);

		m_bIsDirty = false;
	}

	glBindSampler(ID, m_GLObj);
}

void SamplerStateOGL::UnBind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const
{
	glBindSampler(ID, 0);
}



//
// Translate
//
GLenum GLTranslateMinFilter(SamplerStateOGL::MinFilter _minFilter)
{
    if (_minFilter == SamplerStateOGL::MinFilter::MinLinear)
    {
        return GL_LINEAR;
    }
    else if (_minFilter == SamplerStateOGL::MinFilter::MinNearest)
    {
        return GL_NEAREST;
    }

    throw std::exception("Invalid sampler GLTranslateMinFilter modes.");
}

GLenum GLTranslateMinMipFilter(SamplerStateOGL::MinFilter _minFilter, SamplerStateOGL::MipFilter _mipFilter)
{
    if (_minFilter == SamplerStateOGL::MinFilter::MinLinear && _mipFilter == SamplerStateOGL::MipFilter::MipLinear)
    {
        return GL_LINEAR_MIPMAP_LINEAR;
    }
    else if (_minFilter == SamplerStateOGL::MinFilter::MinLinear && _mipFilter == SamplerStateOGL::MipFilter::MipNearest)
    {
        return GL_LINEAR_MIPMAP_NEAREST;
    }
    else if (_minFilter == SamplerStateOGL::MinFilter::MinNearest && _mipFilter == SamplerStateOGL::MipFilter::MipLinear)
    {
        return GL_NEAREST_MIPMAP_LINEAR;
    }
    else if (_minFilter == SamplerStateOGL::MinFilter::MinNearest && _mipFilter == SamplerStateOGL::MipFilter::MipNearest)
    {
        return GL_NEAREST_MIPMAP_NEAREST;
    }

    throw std::exception("Invalid sampler MinMapFilter modes.");
}

GLenum GLTranslateMagFilter(SamplerStateOGL::MagFilter _magFilter)
{
    if (_magFilter == SamplerStateOGL::MagFilter::MagLinear)
    {
        return GL_LINEAR;
    }
    else if (_magFilter == SamplerStateOGL::MagFilter::MagNearest)
    {
        return GL_NEAREST;
    }

    throw std::exception("Invalid sampler MagFilter modes.");
}

GLenum GLTranslateWrapMode(SamplerStateOGL::WrapMode wrapMode)
{
    switch (wrapMode)
    {
        case SamplerStateOGL::WrapMode::Repeat:
            return GL_REPEAT;

        case SamplerStateOGL::WrapMode::Clamp:
            return GL_CLAMP_TO_EDGE;

        case SamplerStateOGL::WrapMode::Mirror:
            return GL_MIRRORED_REPEAT;

        case SamplerStateOGL::WrapMode::Border:
            return GL_CLAMP_TO_BORDER;
    }

    throw std::exception("Invalid sampler WrapMode modes.");
}

GLenum GLTranslateCompareMode(SamplerStateOGL::CompareMode mode)
{
    switch (mode)
    {
        case SamplerStateOGL::CompareMode::None:
            return GL_NONE;

        case SamplerStateOGL::CompareMode::CompareRefToTexture:
            return GL_COMPARE_R_TO_TEXTURE;
    }

    throw std::exception("Invalid sampler CompareMode modes.");
}

GLenum GLTranslateCompareFunction(SamplerStateOGL::CompareFunc compareFunc)
{
    switch (compareFunc)
    {
        case SamplerStateOGL::CompareFunc::Never:
            return GL_NEVER;

        case SamplerStateOGL::CompareFunc::Less:
            return GL_LESS;

        case SamplerStateOGL::CompareFunc::Equal:
            return GL_EQUAL;

        case SamplerStateOGL::CompareFunc::LessEqual:
            return GL_LEQUAL;

        case SamplerStateOGL::CompareFunc::Greater:
            return GL_GREATER;

        case SamplerStateOGL::CompareFunc::NotEqual:
            return GL_NOTEQUAL;

        case SamplerStateOGL::CompareFunc::GreaterEqual:
            return GL_GEQUAL;

        case SamplerStateOGL::CompareFunc::Always:
            return GL_ALWAYS;
    }

    throw std::exception("Invalid sampler CompareFunction modes.");
}