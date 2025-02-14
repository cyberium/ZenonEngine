#include "stdafx.h"

// Include
#include "RenderDeviceOGL.h"

// General
#include "TextureOGL.h"

// Additional
#include "ShaderOGL.h"
#include "TextureOGLTranslate.h"

TextureOGL::TextureOGL(IRenderDevice* RenderDevice)
	: m_RenderDevice(RenderDevice)
	, m_TextureWidth(0)
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
TextureOGL::TextureOGL(IRenderDevice* RenderDevice, uint16_t width, uint16_t height, uint16_t slices, const TextureFormat& format, CPUAccess cpuAccess)
	: m_RenderDevice(RenderDevice)
	, m_TextureWidth(width)
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
TextureOGL::TextureOGL(IRenderDevice* RenderDevice, uint16_t size, uint16_t count, const TextureFormat& format, CPUAccess cpuAccess)
	: m_RenderDevice(RenderDevice)
{
	m_TextureDimension = ITexture::Dimension::TextureCube;
	m_TextureWidth = m_TextureHeight = size;

	glGenTextures(1, &m_GLObj);

	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_GLObj);

	//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// Sampler state

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

TextureOGL::~TextureOGL()
{
	glDeleteTextures(1, &m_GLObj);
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

bool TextureOGL::LoadTexture2D(const std::string& fileName)
{
    std::shared_ptr<IFile> f = m_RenderDevice.lock()->GetBaseManager()->GetManager<IFilesManager>()->Open(fileName);
    if (f == nullptr)
        return false;

	std::shared_ptr<IImage> image = m_RenderDevice.lock()->GetBaseManager()->GetManager<IImagesFactory>()->CreateImage(f);
	if (image == nullptr)
	{
		return false;
	}

    m_BPP = image->GetBitsPerPixel();
    m_bIsTransparent = image->IsTransperent();
    m_TextureDimension = ITexture::Dimension::Texture2D;
    m_TextureWidth = image->GetWidth();
    m_TextureHeight = image->GetHeight();
    m_Pitch = image->GetStride();

    m_TextureType = GL_TEXTURE_2D;
    m_TextureDepth = 1;
    m_bGenerateMipmaps = false;


    glActiveTexture(GL_TEXTURE15);
    glBindTexture(m_TextureType, m_GLObj);
    {
        //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(m_TextureType, 0, GL_RGBA8, m_TextureWidth, m_TextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->GetData());
        OGLCheckError();

		/*uint32 lastW = m_TextureWidth;
		uint32 lastH = m_TextureHeight;
		for (uint32 i = 1; i < 16; i++)
		{
			lastW /= 2;
			lastH /= 2;
			FIBITMAP* dib2 = FreeImage_Rescale(dib, lastW, lastH, FREE_IMAGE_FILTER::FILTER_BICUBIC);

			BYTE* textureData2 = FreeImage_GetBits(dib2);
			glTexImage2D(m_TextureType, i, GL_RGBA8, lastW, lastH, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData2);
			OGLCheckError();

			FreeImage_Unload(dib2);
		}*/

        glGenerateMipmap(m_TextureType);
		OGLCheckError();
    }
    glBindTexture(m_TextureType, 0);

    m_bIsDirty = false;

    return true;
}

bool TextureOGL::LoadTextureCube(const std::string& fileName)
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

ITexture* TextureOGL::GetFace(CubeFace face) const
{
	return std::static_pointer_cast<ITexture>(std::const_pointer_cast<TextureOGL>(shared_from_this()));
}

ITexture* TextureOGL::GetSlice(uint32 slice) const
{
	return std::static_pointer_cast<ITexture>(std::const_pointer_cast<TextureOGL>(shared_from_this()));
}

uint16_t TextureOGL::GetWidth() const
{
	return m_TextureWidth;
}

uint16_t TextureOGL::GetHeight() const
{
	return m_TextureHeight;
}

glm::ivec2 TextureOGL::GetSize() const
{
    return glm::ivec2(m_TextureWidth, m_TextureHeight);
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
	glActiveTexture(GL_TEXTURE15);
	glBindTexture(GL_TEXTURE_2D, m_GLObj);
	{
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		/*float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);*/

		GLenum internalFormat = TranslateTextureInternalFormat(m_TextureFormat);
		GLenum inputFormat = TranslateTextureInputFormat(m_TextureFormat);
		GLenum inputType = TranslateTextureInputType(m_TextureFormat);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, inputFormat, inputType, NULL);
		OGLCheckError();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
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
	case ITexture::Dimension::TextureCube:
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

void TextureOGL::Copy(ITexture* other)
{
	std::shared_ptr<TextureOGL> srcTexture = std::dynamic_pointer_cast<TextureOGL>(other);

	if (srcTexture && srcTexture != this)
	{
		if (m_TextureDimension == srcTexture->m_TextureDimension &&
			m_TextureWidth == srcTexture->m_TextureWidth &&
			m_TextureHeight == srcTexture->m_TextureHeight
		)
		{
			switch (m_TextureDimension)
			{
			case ITexture::Dimension::Texture2D:
			case ITexture::Dimension::Texture2DArray:
				throw std::exception("Not implemented!");
				//m_DeviceImmediateContext->CopyResource(m_pTexture2D, srcTexture->m_pTexture2D);
				break;
			case ITexture::Dimension::TextureCube:
				throw std::exception("Not implemented!");
				//m_DeviceImmediateContext->CopyResource(m_pTexture3D, srcTexture->m_pTexture3D);
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
		if (FAILED(m_DeviceImmediateContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &mappedResource)))
		{
			Log::Error("Failed to map texture resource for reading.");
		}

		memcpy_s(m_Buffer.data(), m_Buffer.size(), mappedResource.pData, m_Buffer.size());

		m_DeviceImmediateContext->Unmap(m_pTexture2D, 0);
	}*/
}

void TextureOGL::Clear(ClearFlags clearFlags, cvec4 color, float depth, uint8_t stencil)
{
	/*if (m_pRenderTargetView && ((int)clearFlags & (int)ClearFlags::Color) != 0)
	{
		m_DeviceImmediateContext->ClearRenderTargetView(m_pRenderTargetView, glm::value_ptr(color));
	}

	{
		UINT flags = 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Depth) != 0 ? D3D11_CLEAR_DEPTH : 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Stencil) != 0 ? D3D11_CLEAR_STENCIL : 0;
		if (m_pDepthStencilView && flags > 0)
		{
			m_DeviceImmediateContext->ClearDepthStencilView(m_pDepthStencilView, flags, depth, stencil);
		}
	}*/
}

void TextureOGL::Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const
{
	if (m_bIsDirty)
	{
		/*if (m_bDynamic && m_pTexture2D)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			// Copy the texture data to the texture resource
			HRESULT hr = m_DeviceImmediateContext->Map(m_pTexture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				Log::Error("Failed to map texture resource for writing.");
			}

			memcpy_s(mappedResource.pData, m_Buffer.size(), m_Buffer.data(), m_Buffer.size());

			m_DeviceImmediateContext->Unmap(m_pTexture2D, 0);

			if (m_bGenerateMipmaps)
			{
				m_DeviceImmediateContext->GenerateMips(m_pShaderResourceView);
			}
		}*/
		m_bIsDirty = false;
	}

	const ShaderOGL* pShader = dynamic_cast<const ShaderOGL*>(shader);
	_ASSERT(pShader != NULL);

	if (pShader->GetType() != EShaderType::PixelShader)
	{
		return;
	}

	glProgramUniform1i(pShader->GetGLObject(), ID, ID);

	glActiveTexture(GL_TEXTURE0 + ID);
	glBindTexture(m_TextureType, m_GLObj);

}

void TextureOGL::Bind(uint32_t ID, EShaderType _shaderType, IShaderParameter::EType parameterType) const
{
	_ASSERT(false);
}

void TextureOGL::UnBind(uint32_t ID, const IShader* shader, IShaderParameter::EType parameterType) const
{
	const ShaderOGL* pShader = dynamic_cast<const ShaderOGL*>(shader);
	_ASSERT(pShader != NULL);

	glProgramUniform1i(pShader->GetGLObject(), ID, 0);

	glActiveTexture(GL_TEXTURE0 + ID);
	glBindTexture(m_TextureType, 0);
}

void TextureOGL::UnBind(uint32_t ID, EShaderType _shaderType, IShaderParameter::Type parameterType) const
{
	_ASSERT(false);
}

const std::vector<uint8>& TextureOGL::GetBuffer()
{
	return m_Buffer;
}

uint32 TextureOGL::GetGLObject()
{
	return m_GLObj;
}