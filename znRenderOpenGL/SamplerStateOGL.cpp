#include <stdafx.h>

// General
#include "SamplerStateOGL.h"

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
	else if (_magFilter == SamplerStateOGL::MagFilter::MagLinear)
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

SamplerStateOGL::SamplerStateOGL()
	: m_MinFilter(MinFilter::MinNearest)
	, m_MagFilter(SamplerStateOGL::MagFilter::MagNearest)
	, m_MipFilter(SamplerStateOGL::MipFilter::MipNearest)
	, m_WrapModeU(WrapMode::Repeat)
	, m_WrapModeV(WrapMode::Repeat)
	, m_WrapModeW(WrapMode::Repeat)
	, m_CompareMode(CompareMode::None)
	, m_CompareFunc(CompareFunc::Always)
	, m_fLODBias(0.0f)
	, m_fMinLOD(0.0f)
	, m_fMaxLOD(Math::MaxFloat)
	, m_bIsAnisotropicFilteringEnabled(false)
	, m_AnisotropicFiltering(1)
	, m_bIsDirty(true)
{
	glGenSamplers(1, &m_GLObj);
}

SamplerStateOGL::~SamplerStateOGL()
{
	if (m_GLObj != 0)
	{
		glDeleteSamplers(1, &m_GLObj);
		m_GLObj = 0;
	}
}

void SamplerStateOGL::SetFilter(MinFilter minFilter, MagFilter magFilter, MipFilter mipFilter)
{
	m_MinFilter = minFilter;
	m_MagFilter = magFilter;
	m_MipFilter = mipFilter;
	m_bIsDirty = true;
}

void SamplerStateOGL::GetFilter(MinFilter& minFilter, MagFilter& magFilter, MipFilter& mipFilter) const
{
	minFilter = m_MinFilter;
	magFilter = m_MagFilter;
	mipFilter = m_MipFilter;
}

void SamplerStateOGL::SetWrapMode(WrapMode u, WrapMode v, WrapMode w)
{
	m_WrapModeU = u;
	m_WrapModeV = v;
	m_WrapModeW = w;
	m_bIsDirty = true;
}

void SamplerStateOGL::GetWrapMode(WrapMode& u, WrapMode& v, WrapMode& w) const
{
	u = m_WrapModeU;
	v = m_WrapModeV;
	w = m_WrapModeW;
}

void SamplerStateOGL::SetCompareFunction(CompareFunc compareFunc)
{
	m_CompareFunc = compareFunc;
	m_bIsDirty = true;
}

SamplerState::CompareFunc SamplerStateOGL::GetCompareFunc() const
{
	return m_CompareFunc;
}

void SamplerStateOGL::SetLODBias(float lodBias)
{
	m_fLODBias = lodBias;
	m_bIsDirty = true;
}

float SamplerStateOGL::GetLODBias() const
{
	return m_fLODBias;
}

void SamplerStateOGL::SetMinLOD(float minLOD)
{
	m_fMinLOD = minLOD;
	m_bIsDirty = true;
}

float SamplerStateOGL::GetMinLOD() const
{
	return m_fMinLOD;
}

void SamplerStateOGL::SetMaxLOD(float maxLOD)
{
	m_fMaxLOD = maxLOD;
	m_bIsDirty = true;
}

float SamplerStateOGL::GetMaxLOD() const
{
	return m_fMaxLOD;
}

void SamplerStateOGL::SetBorderColor(cvec4 borderColor)
{
	m_BorderColor = borderColor;
	m_bIsDirty = true;
}

vec4 SamplerStateOGL::GetBorderColor() const
{
	return m_BorderColor;
}

void SamplerStateOGL::EnableAnisotropicFiltering(bool enabled)
{
	m_bIsAnisotropicFilteringEnabled = enabled;
	m_bIsDirty = true;
}

bool SamplerStateOGL::IsAnisotropicFilteringEnabled() const
{
	return m_bIsAnisotropicFilteringEnabled;
}


void SamplerStateOGL::SetMaxAnisotropy(uint8_t maxAnisotropy)
{
	m_AnisotropicFiltering = glm::clamp<uint8_t>(maxAnisotropy, 1, 16);
	m_bIsDirty = true;
}

uint8_t SamplerStateOGL::GetMaxAnisotropy() const
{
	return m_AnisotropicFiltering;
}

void SamplerStateOGL::Bind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	//if (m_bIsDirty)
	{
		glSamplerParameteri(m_GLObj, GL_TEXTURE_MIN_FILTER, GLTranslateMinFilter(m_MinFilter));
		//glSamplerParameteri(m_GLObj, GL_TEXTURE_MIN_FILTER, GLTranslateMinMipFilter(m_MinFilter, m_MipFilter));
		glSamplerParameteri(m_GLObj, GL_TEXTURE_MAG_FILTER, GLTranslateMagFilter(m_MagFilter));

		glSamplerParameteri(m_GLObj, GL_TEXTURE_MAX_ANISOTROPY_EXT, m_AnisotropicFiltering);

		// Lod
		glSamplerParameteri(m_GLObj, GL_TEXTURE_MIN_LOD, m_fMinLOD);
		glSamplerParameteri(m_GLObj, GL_TEXTURE_MAX_LOD, m_fMaxLOD);
		//glSamplerParameteri(m_GLObj, GL_TEXTURE_LOD_BIAS, m_fLODBias);
		
		// Wrap
		glSamplerParameteri(m_GLObj, GL_TEXTURE_WRAP_S, GLTranslateWrapMode(m_WrapModeU));
		glSamplerParameteri(m_GLObj, GL_TEXTURE_WRAP_T, GLTranslateWrapMode(m_WrapModeV));
		glSamplerParameteri(m_GLObj, GL_TEXTURE_WRAP_R, GLTranslateWrapMode(m_WrapModeW));

		if (m_CompareMode != CompareMode::None)
		{
			glSamplerParameteri(m_GLObj, GL_TEXTURE_COMPARE_MODE, GLTranslateCompareMode(m_CompareMode));
			glSamplerParameteri(m_GLObj, GL_TEXTURE_COMPARE_FUNC, GLTranslateCompareFunction(m_CompareFunc));
		}
		else
		{
			glSamplerParameteri(m_GLObj, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		}

		m_bIsDirty = false;
	}

	glBindSampler(ID, m_GLObj);
}

void SamplerStateOGL::UnBind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	glBindSampler(ID, 0);
}

