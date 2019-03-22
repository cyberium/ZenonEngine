#include <stdafx.h>

// Include
#include "RenderDeviceOGL.h"

// General
#include "TextureOGL.h"

// Additional
#include "ShaderOGL.h"
#include "TextureOGLTranslate.h"

TextureOGL::TextureOGL(RenderDeviceOGL* _device)
	: m_TextureWidth(0)
	, m_TextureHeight(0)
	, m_TextureDepth(0)
	, m_CPUAccess(CPUAccess::None)
	, m_bDynamic(false)
	, m_bGenerateMipmaps(false)
	, m_BPP(0)
	, m_Pitch(0)
	, m_bIsTransparent(false)
	, m_bIsDirty(false)
	, m_TextureType(0)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &m_GLObj);
}

// 2D Texture
TextureOGL::TextureOGL(RenderDeviceOGL* _device, uint16_t width, uint16_t height, uint16_t slices, const TextureFormat& format, CPUAccess cpuAccess)
	: m_TextureWidth(width)
	, m_TextureHeight(height)
	, m_BPP(0)
	, m_TextureFormat(format)
	, m_CPUAccess(cpuAccess)
	, m_bGenerateMipmaps(false)
	, m_bIsTransparent(true)
	, m_bIsDirty(false)
	, m_TextureType(GL_TEXTURE_2D)
{
	m_TextureDepth = glm::max<uint16_t>(slices, 1);

	m_TextureDimension = Dimension::Texture2D;
	if (m_TextureDepth > 1)
	{
		m_TextureDimension = Dimension::Texture2DArray;
	}

	glGenTextures(1, &m_GLObj);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, m_GLObj);
	{
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		/*float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);*/

		GLenum internalFormat = TranslateTextureInternalFormat(format);
		GLenum inputFormat = TranslateTextureInputFormat(format);
		GLenum inputType = TranslateTextureInputType(format);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_TextureWidth, m_TextureHeight, 0, inputFormat, inputType, NULL);
		OGLCheckError();

		// Sampler state
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

// CUBE Texture
TextureOGL::TextureOGL(RenderDeviceOGL* _device, uint16_t size, uint16_t count, const TextureFormat& format, CPUAccess cpuAccess)
{
	m_TextureDimension = Texture::Dimension::TextureCube;
	m_TextureWidth = m_TextureHeight = size;

	glGenTextures(1, &m_GLObj);

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLObj);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Sampler state

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

TextureOGL::~TextureOGL()
{
	if (m_GLObj != 0)
	{
		glDeleteTextures(1, &m_GLObj);
		m_GLObj = 0;
	}
}

bool TextureOGL::LoadTextureCustom(uint16_t width, uint16_t height, void * pixels)
{
	m_TextureType = GL_TEXTURE_2D;
	m_TextureWidth = width;
	m_TextureHeight = height;
	m_TextureDepth = 1;
	m_bGenerateMipmaps = false;

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(m_TextureType, m_GLObj);

	glTexImage2D(m_TextureType, 0, GL_RGBA8, m_TextureWidth, m_TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	OGLCheckError();

	//glGenerateMipmap(m_TextureType);

	glBindTexture(m_TextureType, 0);

	return true;
}

bool TextureOGL::LoadTexture2D(cstring fileName)
{
	/*std::shared_ptr<IFile> f = GetManager<IFilesManager>()->Open(fileName);

	LIBBLP_PixelView blpView;
	LIBBLP_Load(f->getData(), f->getSize(), &blpView);

	m_TextureType = GL_TEXTURE_2D;
	m_TextureWidth = blpView.MipWidth[0];
	m_TextureHeight = blpView.MipHeight[0];
	m_TextureDepth = 1;
	m_bGenerateMipmaps = false;

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(m_TextureType, m_GLObj);

	for (uint8 i = 0; i < blpView.MipCount; i++)
	{
		glTexImage2D(m_TextureType, i, GL_RGBA8, blpView.MipWidth[i], blpView.MipHeight[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, blpView.MipData[i]);
		OGLCheckError();
	}

	//glGenerateMipmap(m_TextureType);

	glBindTexture(m_TextureType, 0);*/

	return true;
}

bool TextureOGL::LoadTextureCube(cstring fileName)
{
	m_TextureType = GL_TEXTURE_CUBE_MAP;

	return false;
}

void TextureOGL::GenerateMipMaps()
{
	if (m_bGenerateMipmaps && (m_TextureType != GL_TEXTURE_CUBE_MAP))
	{
		glGenerateMipmap(m_TextureType);
	}
}

std::shared_ptr<Texture> TextureOGL::GetFace(CubeFace face) const
{
	return std::static_pointer_cast<Texture>(std::const_pointer_cast<TextureOGL>(shared_from_this()));
}

std::shared_ptr<Texture> TextureOGL::GetSlice(uint32 slice) const
{
	return std::static_pointer_cast<Texture>(std::const_pointer_cast<TextureOGL>(shared_from_this()));
}

uint16_t TextureOGL::GetWidth() const
{
	return m_TextureWidth;
}

uint16_t TextureOGL::GetHeight() const
{
	return m_TextureHeight;
}

uint16_t TextureOGL::GetDepth() const
{
	return m_TextureDepth;
}

uint8_t TextureOGL::GetBPP() const
{
	return m_BPP;
}

bool TextureOGL::IsTransparent() const
{
	return m_bIsTransparent;
}

// Resize

void TextureOGL::Resize2D(uint16_t width, uint16_t height)
{
	Log::Error("TextureOGL::Resize2D Not implemented!");
}

void TextureOGL::ResizeCube(uint16_t size)
{
	Log::Error("TextureOGL::ResizeCube Not implemented!");
}

void TextureOGL::Resize(uint16_t width, uint16_t height, uint16_t depth)
{
	switch (m_TextureDimension)
	{
	case Dimension::Texture2D:
	case Dimension::Texture2DArray:
		Resize2D(width, height);
		break;
	case Texture::Dimension::TextureCube:
		ResizeCube(width);
		break;
	default:
		std::exception("Unknown texture dimension.");
	}
}

void TextureOGL::Plot(glm::ivec2 coord, const uint8_t* pixel, size_t size)
{
	assert(m_BPP > 0 && m_BPP % 8 == 0);
	assert(coord.s < m_TextureWidth && coord.t < m_TextureHeight && size == (m_BPP / 8));

	uint8_t bytesPerPixel = (m_BPP / 8);
	uint32_t stride = m_TextureWidth * bytesPerPixel;
	uint32_t index = (coord.s * bytesPerPixel) + (coord.t * stride);

	for (uint32 i = 0; i < size; ++i)
	{
		m_Buffer[index + i] = *(pixel + i);
	}

	m_bIsDirty = true;
}

void TextureOGL::FetchPixel(glm::ivec2 coord, uint8_t*& pixel, size_t size)
{
	assert(m_BPP > 0 && m_BPP % 8 == 0);
	assert(coord.s < m_TextureWidth && coord.t < m_TextureHeight && size == (m_BPP / 8));

	uint8_t bytesPerPixel = (m_BPP / 8);
	uint32_t stride = m_TextureWidth * bytesPerPixel;
	uint32_t index = (coord.s * bytesPerPixel) + (coord.t * stride);
	pixel = &m_Buffer[index];
}

void TextureOGL::Copy(std::shared_ptr<Texture> other)
{
	std::shared_ptr<TextureOGL> srcTexture = std::dynamic_pointer_cast<TextureOGL>(other);

	if (srcTexture && srcTexture.get() != this)
	{
		if (m_TextureDimension == srcTexture->m_TextureDimension &&
			m_TextureWidth == srcTexture->m_TextureWidth &&
			m_TextureHeight == srcTexture->m_TextureHeight
		)
		{
			switch (m_TextureDimension)
			{
			case Texture::Dimension::Texture2D:
			case Texture::Dimension::Texture2DArray:
				throw std::exception("Not implemented!");
				//m_pDeviceContext->CopyResource(m_pTexture2D, srcTexture->m_pTexture2D);
				break;
			case Texture::Dimension::TextureCube:
				throw std::exception("Not implemented!");
				//m_pDeviceContext->CopyResource(m_pTexture3D, srcTexture->m_pTexture3D);
				break;
			}
		}
		else
		{
			Log::Error("Incompatible source texture.");
		}
	}

	/*if (((int)m_CPUAccess & (int)CPUAccess::Read) != 0 && m_pTexture2D)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// Copy the texture data from the texture resource
		if (FAILED(m_pDeviceContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &mappedResource)))
		{
			Log::Error("Failed to map texture resource for reading.");
		}

		memcpy_s(m_Buffer.data(), m_Buffer.size(), mappedResource.pData, m_Buffer.size());

		m_pDeviceContext->Unmap(m_pTexture2D, 0);
	}*/
}

void TextureOGL::Clear(ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	/*if (m_pRenderTargetView && ((int)clearFlags & (int)ClearFlags::Color) != 0)
	{
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, glm::value_ptr(color));
	}

	{
		UINT flags = 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Depth) != 0 ? D3D11_CLEAR_DEPTH : 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Stencil) != 0 ? D3D11_CLEAR_STENCIL : 0;
		if (m_pDepthStencilView && flags > 0)
		{
			m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, flags, depth, stencil);
		}
	}*/
}

void TextureOGL::Bind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	if (m_bIsDirty)
	{
		/*if (m_bDynamic && m_pTexture2D)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			// Copy the texture data to the texture resource
			HRESULT hr = m_pDeviceContext->Map(m_pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				Log::Error("Failed to map texture resource for writing.");
			}

			memcpy_s(mappedResource.pData, m_Buffer.size(), m_Buffer.data(), m_Buffer.size());

			m_pDeviceContext->Unmap(m_pTexture2D, 0);

			if (m_bGenerateMipmaps)
			{
				m_pDeviceContext->GenerateMips(m_pShaderResourceView);
			}
		}*/
		m_bIsDirty = false;
	}

	std::shared_ptr<ShaderOGL> pShader = std::dynamic_pointer_cast<ShaderOGL>(shader.lock());
	assert1(pShader != NULL);

	if (pShader->GetType() != Shader::ShaderType::PixelShader)
	{
		return;
	}

	glProgramUniform1i(pShader->GetGLObject(), ID, ID);

	glActiveTexture(GL_TEXTURE0 + ID);
	glBindTexture(m_TextureType, m_GLObj);

}

void TextureOGL::Bind(uint32_t ID, Shader::ShaderType _shaderType, ShaderParameter::Type parameterType)
{
	fail1();
}

void TextureOGL::UnBind(uint32_t ID, std::weak_ptr<Shader> shader, ShaderParameter::Type parameterType)
{
	std::shared_ptr<ShaderOGL> pShader = std::dynamic_pointer_cast<ShaderOGL>(shader.lock());
	assert1(pShader != NULL);

	glProgramUniform1i(pShader->GetGLObject(), ID, 0);

	glActiveTexture(GL_TEXTURE0 + ID);
	glBindTexture(m_TextureType, 0);
}

void TextureOGL::UnBind(uint32_t ID, Shader::ShaderType _shaderType, ShaderParameter::Type parameterType)
{
	fail1();
}

const std::vector<uint8>& TextureOGL::GetBuffer()
{
	return m_Buffer;
}

uint32 TextureOGL::GetGLObject()
{
	return m_GLObj;
}