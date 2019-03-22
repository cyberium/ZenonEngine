#pragma once

#include "TextureOGL.h"
#include "OpenGL.h"


struct TextureOGLFormat
{
	GLenum SizedInternalFormat;
	GLenum BaseInternalFormat;
	uint8_t RedBits;
	uint8_t GreenBits;
	uint8_t BlueBits;
	uint8_t AlphaBits;
};

TextureOGLFormat TextureOGLFormatsCommonSNORM[] =
{

	{ GL_R8_SNORM,		GL_RED,	8, 0, 0, 0},
	{ GL_R16_SNORM,		GL_RED,	16, 0, 0, 0},
	{ GL_RG8_SNORM,		GL_RG,	8,	8, 0, 0 },
	{ GL_RG16_SNORM,	GL_RG,	16, 16, 0, 0 },
	{ GL_RGB8_SNORM,	GL_RGB,	8,	8,	8, 0 },
	{ GL_RGB16_SNORM,	GL_RGB,	16,	16,	16, 0 },
	{ GL_RGBA8_SNORM,	GL_RGBA,8,	8,	8,	8 }
};

TextureOGLFormat TextureOGLFormatsCommon[] =
{
	{ GL_R8,			GL_RED, 8, 0, 0, 0},
	{ GL_R16,			GL_RED,	16, 0, 0, 0},
	{ GL_RG8,			GL_RG,	8,	8, 0, 0 },
	{ GL_RG16,			GL_RG,	16,	16, 0, 0 },
	{ GL_RGB8,			GL_RGB,	8,	8,	8, 0 },
	{ GL_RGB10,			GL_RGB,	10,	10,	10, 0 },
	{ GL_RGB12,			GL_RGB,	12,	12,	12, 0 },
	{ GL_RGBA4,			GL_RGBA,4,	4,	4,	4 },
	{ GL_RGB5_A1,		GL_RGBA,5,	5,	5,	1 },
	{ GL_RGBA8,			GL_RGBA,8,	8,	8,	8 },
	{ GL_RGBA12,		GL_RGBA,12,	12,	12,	12 },
	{ GL_RGBA16,		GL_RGBA,16,	16,	16,	16 }
};

TextureOGLFormat TextureOGLFormatsFloat[] =
{
	{ GL_R16F,			GL_RED, 16, 0, 0, 0},
	{ GL_RG16F,			GL_RG,	16,	16, 0, 0 },
	{ GL_RGB16F,		GL_RGB,	16,	16,	16, 0 },
	{ GL_RGBA16F,		GL_RGBA,16,	16,	16,	16 },

	{ GL_R32F,			GL_RED,	32, 0, 0, 0 },
	{ GL_RG32F,			GL_RG,	32,	32, 0, 0 },
	{ GL_RGB32F,		GL_RGB,	32,	32,	32, 0 },
	{ GL_RGBA32F,		GL_RGBA,32,	32,	32,	32 }
};

TextureOGLFormat TextureOGLFormatsInteger[] =
{
	{ GL_R8I,			GL_RED,	8, 0, 0, 0 },
	{ GL_R16I,			GL_RED,	16, 0, 0, 0 },
	{ GL_R32I,			GL_RED,	32, 0, 0, 0 },
	{ GL_RG8I,			GL_RG,	8,	8, 0, 0 },
	{ GL_RG16I,			GL_RG,	16,	16, 0, 0 },
	{ GL_RG32I,			GL_RG,	32,	32, 0, 0 },
	{ GL_RGB8I,			GL_RGB,	8,	8,	8, 0 },
	{ GL_RGB16I,		GL_RGB,	16,	16,	16, 0 },
	{ GL_RGB32I,		GL_RGB,	32,	32,	32, 0 },
	{ GL_RGBA8I,		GL_RGBA,8,	8,	8,	8 },
	{ GL_RGBA16I,		GL_RGBA,16,	16,	16,	16 },
	{ GL_RGBA32I,		GL_RGBA,32,	32,	32,	32 }
};

TextureOGLFormat TextureOGLFormatsUnsignedInteger[] =
{
	{ GL_R8UI,			GL_RED,	8, 0, 0, 0 },
	{ GL_R16UI,			GL_RED,	16, 0, 0, 0 },
	{ GL_R32UI,			GL_RED,	32, 0, 0, 0 },
	{ GL_RG8UI,			GL_RG,	8,	8, 0, 0 },
	{ GL_RG16UI,		GL_RG,	16,	16, 0, 0 },
	{ GL_RG32UI,		GL_RG,	32,	32, 0, 0 },
	{ GL_RGB8UI,		GL_RGB,	8,	8,	8, 0 },
	{ GL_RGB16UI,		GL_RGB,	16,	16,	16, 0 },
	{ GL_RGB32UI,		GL_RGB,	32,	32,	32, 0 },
	{ GL_RGBA8UI,		GL_RGBA,8,	8,	8,	8 },
	{ GL_RGB10_A2UI,	GL_RGBA,10,	10,	10,	2 },
	{ GL_RGBA16UI,		GL_RGBA,16,	16,	16,	16 },
	{ GL_RGBA32UI,		GL_RGBA,32,	32,	32,	32 }
};

static void ReportAndThrowTextureFormatError(const Texture::TextureFormat& format, cstring file, int line, cstring function, cstring message)
{
	std::stringstream ss;
	ss << message << std::endl;
	ss << "Components: ";
	switch (format.Components)
	{
	case Texture::Components::R:
		ss << "R" << std::endl;
		break;
	case Texture::Components::RG:
		ss << "RG" << std::endl;
		break;
	case Texture::Components::RGB:
		ss << "RGB" << std::endl;
		break;
	case Texture::Components::RGBA:
		ss << "RGBA" << std::endl;
		break;
	case Texture::Components::Depth:
		ss << "Depth" << std::endl;
		break;
	case Texture::Components::DepthStencil:
		ss << "DepthStencil" << std::endl;
		break;
	default:
		ss << "Unknown" << std::endl;
		break;
	}

	ss << "Type:";
	switch (format.Type)
	{
	case Texture::Type::Typeless:
		ss << "Typeless" << std::endl;
		break;
	case Texture::Type::UnsignedNormalized:
		ss << "UnsignedNormalized" << std::endl;
		break;
	case Texture::Type::SignedNormalized:
		ss << "SignedNormalized" << std::endl;
		break;
	case Texture::Type::Float:
		ss << "Float" << std::endl;
		break;
	case Texture::Type::UnsignedInteger:
		ss << "UnsignedInteger" << std::endl;
		break;
	case Texture::Type::SignedInteger:
		ss << "SignedInteger" << std::endl;
		break;
	default:
		ss << "Unknown" << std::endl;
		break;
	}

	ss << "RedBits:     " << (int32_t)format.RedBits << std::endl;
	ss << "GreenBits:   " << (int32_t)format.GreenBits << std::endl;
	ss << "BlueBits:    " << (int32_t)format.BlueBits << std::endl;
	ss << "AlphaBits:   " << (int32_t)format.AlphaBits << std::endl;
	ss << "DepthBits:   " << (int32_t)format.DepthBits << std::endl;
	ss << "StencilBits: " << (int32_t)format.StencilBits << std::endl;
	ss << "Num Samples: " << (int32_t)format.NumSamples << std::endl;

	Log::Fatal("%s", ss.str().c_str());
}
#define ReportTextureFormatError( fmt, msg ) ReportAndThrowTextureFormatError( (fmt), __FILE__, __LINE__, __FUNCTION__, (msg) )

GLenum TranslateTextureInternalFormat(const Texture::TextureFormat& format)
{
	GLenum result = 0;

	switch (format.Components)
	{
	case Texture::Components::R:
		switch (format.Type)
		{
		case Texture::Type::Typeless:
			if (format.RedBits == 8)
			{
				result = GL_R8;
			}
			else if (format.RedBits == 16)
			{
				result = GL_R16;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;
		case Texture::Type::UnsignedNormalized:
			if (format.RedBits == 8)
			{
				result = GL_R8;
			}
			else if (format.RedBits == 16)
			{
				result = GL_R16;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
			}
			break;
		case Texture::Type::SignedNormalized:
			if (format.RedBits == 8)
			{
				result = GL_R8_SNORM;
			}
			else if (format.RedBits == 16)
			{
				result = GL_R16_SNORM;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;
		case Texture::Type::Float:
			if (format.RedBits == 16)
			{
				result = GL_R16F;
			}
			else if (format.RedBits == 32)
			{
				result = GL_R32F;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;
		case Texture::Type::UnsignedInteger:
			if (format.RedBits == 8)
			{
				result = GL_R8UI;
			}
			else if (format.RedBits == 16)
			{
				result = GL_R16UI;
			}
			else if (format.RedBits == 32)
			{
				result = GL_R32UI;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;
		case Texture::Type::SignedInteger:
			if (format.RedBits == 8)
			{
				result = GL_R8I;
			}
			else if (format.RedBits == 16)
			{
				result = GL_R16I;
			}
			else if (format.RedBits == 32)
			{
				result = GL_R32I;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;
		default:
			ReportTextureFormatError(format, "Unknown texture format.");
			break;
		}
		break;

	case Texture::Components::RG:
		switch (format.Type)
		{
		case Texture::Type::Typeless:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = GL_RG8;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = GL_RG16;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;
		case Texture::Type::UnsignedNormalized:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = GL_RG8;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = GL_RG16;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
			}
			break;
		case Texture::Type::SignedNormalized:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = GL_RG8_SNORM;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = GL_RG16_SNORM;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
			}
			break;
		case Texture::Type::Float:
			if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = GL_RG16F;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32)
			{
				result = GL_RG32F;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
			}
			break;
		case Texture::Type::UnsignedInteger:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = GL_RG8UI;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = GL_RG16UI;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32)
			{
				result = GL_RG32UI;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
			}
			break;
		case Texture::Type::SignedInteger:
			if (format.RedBits == 8 && format.GreenBits == 8)
			{
				result = GL_RG8I;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16)
			{
				result = GL_RG16I;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32)
			{
				result = GL_RG32I;
			}
			else
			{
				ReportTextureFormatError(format, "Unsupported texture format.");
			}
			break;
		default:
			ReportTextureFormatError(format, "Unknown texture format.");
			break;
		}
		break;

	case Texture::Components::RGB:
		switch (format.Type)
		{
		case Texture::Type::Typeless:
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;
		case Texture::Type::Float:
			if (format.RedBits == 11 && format.GreenBits == 11 && format.BlueBits == 10)
			{
				result = GL_R11F_G11F_B10F;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32)
			{
				result = GL_RGB32F;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		case Texture::Type::UnsignedInteger:
			if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32)
			{
				result = GL_RGB32UI;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		case Texture::Type::SignedInteger:
			if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32)
			{
				result = GL_RGB32I;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		default:
			ReportTextureFormatError(format, "Unsupported texture format.");
		}
		break;
	case Texture::Components::RGBA:
		switch (format.Type)
		{
		case Texture::Type::Typeless:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = GL_RGBA8;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = GL_RGBA16;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		case Texture::Type::UnsignedNormalized:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = GL_RGBA8;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = GL_RGBA16;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		case Texture::Type::SignedNormalized:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = GL_RGBA8_SNORM;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = GL_RGBA16_SNORM;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		case Texture::Type::Float:
			if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits && format.AlphaBits == 32)
			{
				result = GL_RGBA32F;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = GL_RGBA16F;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		case Texture::Type::UnsignedInteger:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = GL_RGBA8UI;
			}
			else if (format.RedBits == 10 && format.GreenBits == 10 && format.BlueBits == 10 && format.AlphaBits == 2)
			{
				result = GL_RGB10_A2UI;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = GL_RGBA16UI;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		case Texture::Type::SignedInteger:
			if (format.RedBits == 8 && format.GreenBits == 8 && format.BlueBits == 8 && format.AlphaBits == 8)
			{
				result = GL_RGBA8I;
			}
			else if (format.RedBits == 16 && format.GreenBits == 16 && format.BlueBits == 16 && format.AlphaBits == 16)
			{
				result = GL_RGBA16I;
			}
			else if (format.RedBits == 32 && format.GreenBits == 32 && format.BlueBits == 32 && format.AlphaBits == 32)
			{
				result = GL_RGBA32I;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		default:
			ReportTextureFormatError(format, "Unknown texture format.");
			break;
		}
		break;
	case Texture::Components::Depth:
		switch (format.Type)
		{
		case Texture::Type::UnsignedNormalized:
			if (format.DepthBits == 32)
			{
				result = GL_DEPTH_COMPONENT32;
			}
			else if (format.DepthBits == 24)
			{
				result = GL_DEPTH_COMPONENT24;
			}
			else if (format.DepthBits == 16)
			{
				result = GL_DEPTH_COMPONENT16;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		case Texture::Type::Float:
			if (format.DepthBits == 32)
			{
				result = GL_DEPTH_COMPONENT32F;
			}
			else
				ReportTextureFormatError(format, "Unsupported texture format.");
			break;

		default:
			ReportTextureFormatError(format, "Unknown texture format.");
			break;
		}
		break;
	case Texture::Components::DepthStencil:
		if (format.DepthBits == 24 && format.StencilBits == 8)
		{
			result = GL_DEPTH24_STENCIL8;
		}
		else if (format.DepthBits == 32 && format.StencilBits == 8)
		{
			result = GL_DEPTH32F_STENCIL8;
		}
		else
			ReportTextureFormatError(format, "Unsupported texture format.");
		break;
	default:
		ReportTextureFormatError(format, "Unknown texture format.");
		break;
	}

	return result;
}

GLenum TranslateTextureInputFormat(const Texture::TextureFormat& format)
{
	GLenum result = 0;

	switch (format.Components)
	{
	case Texture::Components::R:
		result = GL_RED;
		break;

	case Texture::Components::RG:
		result = GL_RG;
		break;

	case Texture::Components::RGB:
		result = GL_RGB;
		break;

	case Texture::Components::RGBA:
		result = GL_RGBA;
		break;

	case Texture::Components::Depth:
		result = GL_DEPTH_COMPONENT;
		break;

	case Texture::Components::DepthStencil:
		result = GL_DEPTH_STENCIL;
		break;
	default:
		ReportTextureFormatError(format, "Unknown texture format.");
		break;
	}

	return result;
}

GLenum TranslateTextureInputType(const Texture::TextureFormat& format)
{
	/* Type
	GL_UNSIGNED_BYTE,
	GL_BYTE,
	GL_UNSIGNED_SHORT,
	GL_SHORT,
	GL_UNSIGNED_INT,
	GL_INT,
	GL_FLOAT,
	GL_UNSIGNED_BYTE_3_3_2,
	GL_UNSIGNED_BYTE_2_3_3_REV,
	GL_UNSIGNED_SHORT_5_6_5,
	GL_UNSIGNED_SHORT_5_6_5_REV,
	GL_UNSIGNED_SHORT_4_4_4_4,
	GL_UNSIGNED_SHORT_4_4_4_4_REV,
	GL_UNSIGNED_SHORT_5_5_5_1,
	GL_UNSIGNED_SHORT_1_5_5_5_REV,
	GL_UNSIGNED_INT_8_8_8_8,
	GL_UNSIGNED_INT_8_8_8_8_REV,
	GL_UNSIGNED_INT_10_10_10_2,
	GL_UNSIGNED_INT_2_10_10_10_REV
	*/

	GLenum result = 0;

	switch (format.Type)
	{
	case Texture::Type::UnsignedNormalized:
		result = GL_UNSIGNED_BYTE;
		break;
	case Texture::Type::SignedNormalized:
		result = GL_BYTE;
		break;
	case Texture::Type::Float:
		result = GL_FLOAT;
		break;
	case Texture::Type::UnsignedInteger:
		result = GL_UNSIGNED_INT;
		break;
	case Texture::Type::SignedInteger:
		result = GL_INT;
		break;
	default:
		ReportTextureFormatError(format, "Unknown texture format.");
		break;
	}

	if (format.DepthBits > 0 && format.StencilBits == 0)
	{
		if (format.DepthBits == 32)
		{
			result = GL_FLOAT;
		}
		else if (format.DepthBits == 24)
		{
			result = GL_UNSIGNED_INT;
		}
		else if (format.DepthBits == 16)
		{
			result = GL_UNSIGNED_SHORT;
		}
	}
	else
	{
		if (format.DepthBits == 24 && format.StencilBits == 8)
		{
			result = GL_UNSIGNED_INT_24_8;
		}
		else if (format.DepthBits == 32 && format.StencilBits == 8)
		{
			result = GL_FLOAT_32_UNSIGNED_INT_24_8_REV;
		}
	}

	return result;
}