#include "stdafx.h"

// General
#include "TextureDX11.h"

// Additional
#include "TextureDX11Translate.h"

TextureDX11::TextureDX11(IRenderDeviceDX11& RenderDeviceDX11)
	: m_RenderDeviceDX11(RenderDeviceDX11)
	, m_TextureWidth(0)
	, m_TextureHeight(0)
	, m_NumSlices(0)
	, m_TextureResourceFormatSupport(0)
	, m_DepthStencilViewFormatSupport(0)
	, m_ShaderResourceViewFormatSupport(0)
	, m_RenderTargetViewFormatSupport(0)
	, m_Access(EAccess::None)
	, m_bDynamic(false)
	, m_TextureResourceFormat(DXGI_FORMAT_UNKNOWN)
	, m_DepthStencilViewFormat(DXGI_FORMAT_UNKNOWN)
	, m_ShaderResourceViewFormat(DXGI_FORMAT_UNKNOWN)
	, m_RenderTargetViewFormat(DXGI_FORMAT_UNKNOWN)
	, m_NeedGenerateMipmaps(false)
	, m_BPP(0)
	, m_Pitch(0)
	, m_bIsTransparent(false)
	, m_bIsDirty(false)
{}

// 2D Texture
TextureDX11::TextureDX11(IRenderDeviceDX11& RenderDeviceDX11, uint16 width, uint16 height, uint16 slices, const ITexture::TextureFormat& format, EAccess Access)
	: m_RenderDeviceDX11(RenderDeviceDX11)
	, m_DX11Texture2D(nullptr)
	, m_DX11ShaderResourceView(nullptr)
	, m_DX11RenderTargetView(nullptr)
	, m_TextureWidth(0)
	, m_TextureHeight(0)
	, m_BPP(0)
	, m_TextureFormat(format)
	, m_Access(Access)
	, m_NeedGenerateMipmaps(false)
	, m_bIsTransparent(true)
	, m_bIsDirty(false)
{
	m_NumSlices = glm::max<uint16>(slices, 1);

	m_TextureDimension = ITexture::Dimension::Texture2D;
	if (m_NumSlices > 1)
		m_TextureDimension = ITexture::Dimension::Texture2DArray;

	// Translate to DXGI format.
	DXGI_FORMAT dxgiFormat = DX11TranslateFormat(format);
	m_SampleDesc = GetSupportedSampleCount(dxgiFormat, format.NumSamples);

	// Translate back to original format (for best match format).
	m_TextureFormat = DX11TranslateFormat(dxgiFormat, format.NumSamples);

	// Convert Depth/Stencil formats to typeless texture resource formats.
	m_TextureResourceFormat = DX11GetTextureFormat(dxgiFormat);

	// Convert typeless formats to Depth/Stencil view formats.
	m_DepthStencilViewFormat = DX11GetDSVFormat(dxgiFormat);

	// Convert depth/stencil and typeless formats to Shader Resource View formats.
	m_ShaderResourceViewFormat = DX11GetSRVFormat(dxgiFormat);

	// Convert typeless formats to Render Target View formats.
	m_RenderTargetViewFormat = DX11GetRTVFormat(dxgiFormat);

	// Convert typeless format to Unordered Access View formats.
	m_UnorderedAccessViewFormat = DX11GetUAVFormat(dxgiFormat);

	m_BPP = DX11GetBPP(m_TextureResourceFormat);

	// Query for texture format support.
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport), L"Failed to query texture resource format support.");
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_DepthStencilViewFormat, &m_DepthStencilViewFormatSupport), L"Failed to query depth/stencil format support.");
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_ShaderResourceViewFormat, &m_ShaderResourceViewFormatSupport), L"Failed to query shader resource format support.");
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_RenderTargetViewFormat, &m_RenderTargetViewFormatSupport), L"Failed to query RT format support.");
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_UnorderedAccessViewFormat, &m_UnorderedAccessViewFormatSupport), L"Failed to query UAV format support.");

	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURE2D) == 0)
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for 2D textures.");

	// Can the texture be dynamically modified on the CPU?
	m_bDynamic = ((int)m_Access & (int)EAccess::CPUWrite) != 0 && (m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_CPU_LOCKABLE) != 0;
	
	// Can mipmaps be automatically generated for this texture format?
	m_NeedGenerateMipmaps = !m_bDynamic && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0;
	
	if ((((uint32)m_Access & (uint32)EAccess::GPUWrite) != 0))
		if ((m_UnorderedAccessViewFormatSupport & D3D11_FORMAT_SUPPORT_SHADER_LOAD) == 0)
			throw CznRenderException("Format fon't support Unordered access view.");

	Resize(width, height);
}

// CUBE Texture
TextureDX11::TextureDX11(IRenderDeviceDX11& RenderDeviceDX11, uint16 size, const TextureFormat& format, EAccess cpuAccess)
	: m_RenderDeviceDX11(RenderDeviceDX11)
{
	m_TextureDimension = ITexture::Dimension::TextureCube;

	m_TextureWidth = m_TextureHeight = size;

	// Translate to DXGI format.
	DXGI_FORMAT dxgiFormat = DX11TranslateFormat(format);
	m_SampleDesc = GetSupportedSampleCount(dxgiFormat, format.NumSamples);

	// Translate back to original format (for best match format)
	m_TextureFormat = DX11TranslateFormat(dxgiFormat, format.NumSamples);

	// Convert Depth/Stencil formats to typeless texture resource formats
	m_TextureResourceFormat = DX11GetTextureFormat(dxgiFormat);
	// Convert typeless formats to Depth/Stencil view formats
	m_DepthStencilViewFormat = DX11GetDSVFormat(dxgiFormat);
	// Convert depth/stencil and typeless formats to Shader Resource View formats
	m_ShaderResourceViewFormat = DX11GetSRVFormat(dxgiFormat);
	// Convert typeless formats to Render Target View formats
	m_RenderTargetViewFormat = DX11GetRTVFormat(dxgiFormat);
	// Convert typeless format to Unordered Access View formats.
	m_UnorderedAccessViewFormat = DX11GetUAVFormat(dxgiFormat);

	// Query for texture format support.
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport), L"Failed to query texture resource format support.");
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_DepthStencilViewFormat, &m_DepthStencilViewFormatSupport), L"Failed to query depth/stencil format support.");
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_ShaderResourceViewFormat, &m_ShaderResourceViewFormatSupport), L"Failed to query shader resource format support.");
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_RenderTargetViewFormat, &m_RenderTargetViewFormatSupport), L"Failed to query render target format support.");
	CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_UnorderedAccessViewFormat, &m_UnorderedAccessViewFormatSupport), L"Failed to query render target format support.");

	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURECUBE) == 0)
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for cube textures.");

	// Can the texture be dynamically modified on the CPU?
	m_bDynamic = ((int)m_Access & (int)EAccess::CPUWrite) != 0 && (m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_CPU_LOCKABLE) != 0;
	
	// Can mipmaps be automatically generated for this texture format?
	m_NeedGenerateMipmaps = !m_bDynamic && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0; // && ( m_RenderTargetViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN ) != 0;

	if ((((uint32)m_Access & (uint32)EAccess::GPUWrite) != 0))
		if ((m_UnorderedAccessViewFormatSupport & D3D11_FORMAT_SUPPORT_SHADER_LOAD) == 0)
			throw CznRenderException("Format fon't support Unordered access view.");

	Resize(size, size);
}

TextureDX11::~TextureDX11()
{
	// TODO: Change m_Buffer!!!
	//_ASSERT(m_Buffer.empty());
}



//
// ITexture
//
const std::string& TextureDX11::GetFilename() const
{
	return m_FileName;
}

void TextureDX11::GenerateMipMaps()
{
	if (m_NeedGenerateMipmaps && m_DX11ShaderResourceView)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->GenerateMips(m_DX11ShaderResourceView);
	}
}

/*ITexture* TextureDX11::GetFace(CubeFace face) const
{
	return nullptr;
}

ITexture* TextureDX11::GetSlice(uint32 slice) const
{
	return nullptr;
}*/

uint16 TextureDX11::GetWidth() const
{
	return m_TextureWidth;
}

uint16 TextureDX11::GetHeight() const
{
	return m_TextureHeight;
}

glm::ivec2 TextureDX11::GetSize() const
{
	return glm::ivec2(m_TextureWidth, m_TextureHeight);
}

uint16 TextureDX11::GetDepth() const
{
	return m_NumSlices;
}

uint8 TextureDX11::GetBPP() const
{
	return m_BPP;
}

uint8 TextureDX11::GetSamplesCount() const
{
	return m_SampleDesc.Count;
}

bool TextureDX11::IsTransparent() const
{
	return m_bIsTransparent;
}

void TextureDX11::Resize2D(uint16 width, uint16 height)
{
	if (m_TextureWidth == width && m_TextureHeight == height)
		return;

	// Release resource before resizing
	m_DX11Texture2D.Release();
	m_DX11RenderTargetView.Release();
	m_DX11DepthStencilView.Release();
	m_DX11ShaderResourceView.Release();
	m_DX11UnorderedAccessView.Release();

	m_TextureWidth = glm::max<uint16>(width, 1);
	m_TextureHeight = glm::max<uint16>(height, 1);

	// Create texture with the dimensions specified.
	D3D11_TEXTURE2D_DESC textureDesc = { };
	textureDesc.ArraySize = m_NumSlices;
	textureDesc.Format = m_TextureResourceFormat;
	textureDesc.SampleDesc = m_SampleDesc;
	textureDesc.Width = m_TextureWidth;
	textureDesc.Height = m_TextureHeight;
	textureDesc.MipLevels = 1;
	textureDesc.MiscFlags = m_NeedGenerateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

	if (((int)m_Access & (int)EAccess::CPURead) != 0)
	{
		textureDesc.Usage = D3D11_USAGE_STAGING;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	}
	else if (((int)m_Access & (int)EAccess::CPUWrite) != 0)
	{
		textureDesc.Usage = D3D11_USAGE_DYNAMIC;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.CPUAccessFlags = 0;
	}

	if ((((uint32)m_Access & (uint32)EAccess::CPURead) == 0) && !m_bDynamic && (m_DepthStencilViewFormatSupport & D3D11_FORMAT_SUPPORT_DEPTH_STENCIL) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
	}
	if (!m_bDynamic && (m_RenderTargetViewFormatSupport & D3D11_FORMAT_SUPPORT_RENDER_TARGET) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	if (((int)m_Access & (int)EAccess::CPURead) == 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}
	if ((((uint32)m_Access & (uint32)EAccess::CPURead) != 0) && !m_bDynamic)
	{
		textureDesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
	}

	CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, nullptr, &m_DX11Texture2D));

	if ((textureDesc.BindFlags & D3D11_BIND_DEPTH_STENCIL) != 0)
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
		depthStencilViewDesc.Format = m_DepthStencilViewFormat;
		depthStencilViewDesc.Flags = 0;

		if (m_NumSlices > 1)
		{
			if (m_SampleDesc.Count > 1)
			{
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMSARRAY;
				depthStencilViewDesc.Texture2DMSArray.FirstArraySlice = 0;
				depthStencilViewDesc.Texture2DMSArray.ArraySize = m_NumSlices;
			}
			else
			{
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				depthStencilViewDesc.Texture2DArray.MipSlice = 0;
				depthStencilViewDesc.Texture2DArray.FirstArraySlice = 0;
				depthStencilViewDesc.Texture2DArray.ArraySize = m_NumSlices;
			}
		}
		else
		{
			if (m_SampleDesc.Count > 1)
			{
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDesc.Texture2D.MipSlice = 0;
			}
		}

		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateDepthStencilView(m_DX11Texture2D, &depthStencilViewDesc, &m_DX11DepthStencilView));
	}



	if ((textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE) != 0)
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {};
		resourceViewDesc.Format = m_ShaderResourceViewFormat;

		if (m_NumSlices > 1)
		{
			if (m_SampleDesc.Count > 1)
			{
				resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMSARRAY;
				resourceViewDesc.Texture2DMSArray.FirstArraySlice = 0;
				resourceViewDesc.Texture2DMSArray.ArraySize = m_NumSlices;
			}
			else
			{
				resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				resourceViewDesc.Texture2DArray.FirstArraySlice = 0;
				resourceViewDesc.Texture2DArray.ArraySize = m_NumSlices;
				resourceViewDesc.Texture2DArray.MipLevels = m_NeedGenerateMipmaps ? -1 : 1;
				resourceViewDesc.Texture2DArray.MostDetailedMip = 0;
			}
		}
		else
		{
			if (m_SampleDesc.Count > 1)
			{
				resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
				resourceViewDesc.Texture2D.MipLevels = m_NeedGenerateMipmaps ? -1 : 1;
				resourceViewDesc.Texture2D.MostDetailedMip = 0;
			}
		}

		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateShaderResourceView(m_DX11Texture2D, &resourceViewDesc, &m_DX11ShaderResourceView));



		if (m_NeedGenerateMipmaps)
		{
			m_RenderDeviceDX11.GetDeviceContextD3D11()->GenerateMips(m_DX11ShaderResourceView);
		}
	}

	if ((textureDesc.BindFlags & D3D11_BIND_RENDER_TARGET) != 0)
	{
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = {};
		renderTargetViewDesc.Format = m_RenderTargetViewFormat;

		if (m_NumSlices > 1)
		{
			if (m_SampleDesc.Count > 1)
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMSARRAY;
				renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
				renderTargetViewDesc.Texture2DArray.ArraySize = m_NumSlices;

			}
			else
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				renderTargetViewDesc.Texture2DArray.MipSlice = 0;
				renderTargetViewDesc.Texture2DArray.FirstArraySlice = 0;
				renderTargetViewDesc.Texture2DArray.ArraySize = m_NumSlices;
			}
		}
		else
		{
			if (m_SampleDesc.Count > 1)
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
			}
			else
			{
				renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				renderTargetViewDesc.Texture2D.MipSlice = 0;
			}
		}

		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateRenderTargetView(m_DX11Texture2D, &renderTargetViewDesc, &m_DX11RenderTargetView));
	}

	if ((textureDesc.BindFlags & D3D11_BIND_UNORDERED_ACCESS) != 0)
	{
		// UAVs cannot be multi sampled.
		_ASSERT(m_SampleDesc.Count == 1);

		// Create a Shader resource view for the texture.
		D3D11_UNORDERED_ACCESS_VIEW_DESC unorderedAccessViewDesc = {};
		unorderedAccessViewDesc.Format = m_UnorderedAccessViewFormat;

		if (m_NumSlices > 1)
		{
			unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
			unorderedAccessViewDesc.Texture2DArray.MipSlice = 0;
			unorderedAccessViewDesc.Texture2DArray.FirstArraySlice = 0;
			unorderedAccessViewDesc.Texture2DArray.ArraySize = m_NumSlices;
		}
		else
		{
			unorderedAccessViewDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			unorderedAccessViewDesc.Texture2D.MipSlice = 0;
		}

		CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CreateUnorderedAccessView(m_DX11Texture2D, &unorderedAccessViewDesc, &m_DX11UnorderedAccessView));
	}
}

void TextureDX11::ResizeCube(uint16 size)
{
	_ASSERT(FALSE);
}

void TextureDX11::Resize(uint16 width, uint16 height, uint16 depth)
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
			throw CznRenderException("Unknown texture dimension.");
	}
}

void TextureDX11::Plot(glm::ivec2 coord, const uint8* pixel, size_t size)
{
	_ASSERT(m_BPP > 0 && m_BPP % 8 == 0);
	_ASSERT(coord.s < m_TextureWidth && coord.t < m_TextureHeight && size == (m_BPP / 8));

	uint8 bytesPerPixel = (m_BPP / 8);
	uint32_t stride = m_TextureWidth * bytesPerPixel;
	uint32_t index = (coord.s * bytesPerPixel) + (coord.t * stride);

	for (uint32 i = 0; i < size; ++i)
	{
		m_Buffer[index + i] = *(pixel + i);
	}

	m_bIsDirty = true;
}

void TextureDX11::FetchPixel(glm::ivec2 coord, uint8*& pixel, size_t size)
{
	_ASSERT(m_BPP > 0 && m_BPP % 8 == 0);
	_ASSERT(coord.s < m_TextureWidth && coord.t < m_TextureHeight && size == (m_BPP / 8));

	uint8 bytesPerPixel = (m_BPP / 8);
	uint32_t stride = m_TextureWidth * bytesPerPixel;
	uint32_t index = (coord.s * bytesPerPixel) + (coord.t * stride);
	pixel = &m_Buffer[index];
}

void TextureDX11::Copy(const std::shared_ptr<ITexture>& other)
{
	const auto& srcTexture = std::dynamic_pointer_cast<TextureDX11>(other);

	if (srcTexture && srcTexture.get() != this)
	{
		if (m_TextureDimension == srcTexture->m_TextureDimension &&
			m_TextureWidth == srcTexture->m_TextureWidth &&
			m_TextureHeight == srcTexture->m_TextureHeight)
		{
			switch (m_TextureDimension)
			{
			case ITexture::Dimension::Texture2D:
			case ITexture::Dimension::Texture2DArray:
				m_RenderDeviceDX11.GetDeviceContextD3D11()->CopyResource(m_DX11Texture2D, srcTexture->m_DX11Texture2D);
				break;
			case ITexture::Dimension::TextureCube:
				m_RenderDeviceDX11.GetDeviceContextD3D11()->CopyResource(m_DX11Texture3D, srcTexture->m_DX11Texture3D);
				break;
			}
		}
		else
		{
			throw CznRenderException("Incompatible source texture.");
		}
	}

	if (((int)m_Access & (int)EAccess::CPURead) != 0 && m_DX11Texture2D)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource = { };
		CHECK_HR(m_RenderDeviceDX11.GetDeviceContextD3D11()->Map(m_DX11Texture2D, 0, D3D11_MAP_READ, 0, &mappedResource));

		memcpy_s(m_Buffer.data(), m_Buffer.size(), mappedResource.pData, m_Buffer.size());

		m_RenderDeviceDX11.GetDeviceContextD3D11()->Unmap(m_DX11Texture2D, 0);
	}
}

void TextureDX11::Clear(ClearFlags clearFlags, const glm::vec4& color, float depth, uint8 stencil)
{
	if (m_DX11RenderTargetView)
	{
		if (((int)clearFlags & (int)ClearFlags::Color) != 0)
			m_RenderDeviceDX11.GetDeviceContextD3D11()->ClearRenderTargetView(m_DX11RenderTargetView, glm::value_ptr(color));
	}

	if (m_DX11DepthStencilView)
	{
		UINT flags = 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Depth) != 0 ? D3D11_CLEAR_DEPTH : 0;
		flags |= ((int)clearFlags & (int)ClearFlags::Stencil) != 0 ? D3D11_CLEAR_STENCIL : 0;
		if (flags > 0)
			m_RenderDeviceDX11.GetDeviceContextD3D11()->ClearDepthStencilView(m_DX11DepthStencilView, flags, depth, stencil);
	}
}

void TextureDX11::Bind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const
{
	Bind(ID, shader->GetShaderType(), parameterType);
}

void TextureDX11::Bind(uint32_t ID, EShaderType _shaderType, IShaderParameter::Type parameterType) const
{
	if (m_bIsDirty)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->UpdateSubresource(m_DX11Texture2D, 0, nullptr, m_Buffer.data(), m_Pitch, 0);
		m_RenderDeviceDX11.GetDeviceContextD3D11()->GenerateMips(m_DX11ShaderResourceView);

		const_cast<TextureDX11*>(this)->m_Buffer.clear();

		/*if (m_bDynamic && m_DX11Texture2D)
		{
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			// Copy the texture data to the texture resource
			HRESULT hr = m_RenderDeviceDX11.GetDeviceContextD3D11()->Map(m_DX11Texture2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			if (FAILED(hr))
			{
				throw CznRenderException("Failed to map texture resource for writing.");
			}

			memcpy_s(mappedResource.pData, m_Buffer.size(), m_Buffer.data(), m_Buffer.size());

			m_RenderDeviceDX11.GetDeviceContextD3D11()->Unmap(m_DX11Texture2D, 0);

			if (m_NeedGenerateMipmaps)
			{
				m_RenderDeviceDX11.GetDeviceContextD3D11()->UpdateSubresource(m_DX11Texture2D, 0, nullptr, m_Buffer.data(), m_Pitch, 0);
				m_RenderDeviceDX11.GetDeviceContextD3D11()->GenerateMips(m_DX11ShaderResourceView);
			}
		}*/
		m_bIsDirty = false;
	}

	ID3D11ShaderResourceView* srv[] = { m_DX11ShaderResourceView };
	ID3D11UnorderedAccessView* uav[] = { m_DX11UnorderedAccessView };

	if (parameterType == IShaderParameter::Type::Texture && m_DX11ShaderResourceView)
	{
		switch (_shaderType)
		{
		case EShaderType::VertexShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::TessellationControlShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::TessellationEvaluationShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::GeometryShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::PixelShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::ComputeShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetShaderResources(ID, 1, srv);
			break;
		}
	}
	else if (parameterType == IShaderParameter::Type::RWTexture && m_DX11UnorderedAccessView)
	{
		switch (_shaderType)
		{
		case EShaderType::ComputeShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetUnorderedAccessViews(ID, 1, uav, nullptr);
			break;
		}
	}
}


void TextureDX11::UnBind(uint32_t ID, const IShader* shader, IShaderParameter::Type parameterType) const
{
	UnBind(ID, shader->GetShaderType(), parameterType);
}

void TextureDX11::UnBind(uint32_t ID, EShaderType _shaderType, IShaderParameter::Type parameterType) const
{
	ID3D11ShaderResourceView* srv[] = { nullptr };
	ID3D11UnorderedAccessView* uav[] = { nullptr };

	if (parameterType == IShaderParameter::Type::Texture)
	{
		switch (_shaderType)
		{
		case EShaderType::VertexShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->VSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::TessellationControlShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->HSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::TessellationEvaluationShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->DSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::GeometryShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->GSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::PixelShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->PSSetShaderResources(ID, 1, srv);
			break;
		case EShaderType::ComputeShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetShaderResources(ID, 1, srv);
			break;
		}
	}
	else if (parameterType == IShaderParameter::Type::RWTexture)
	{
		switch (_shaderType)
		{
		case EShaderType::ComputeShader:
			m_RenderDeviceDX11.GetDeviceContextD3D11()->CSSetUnorderedAccessViews(ID, 1, uav, nullptr);
			break;
		}
	}
}

DXGI_SAMPLE_DESC TextureDX11::GetSupportedSampleCount(DXGI_FORMAT format, uint8 numSamples)
{
	DXGI_SAMPLE_DESC sampleDesc = {};

	UINT sampleCount = 1;
	UINT qualityLevels = 0;

	while (sampleCount <= numSamples && SUCCEEDED(m_RenderDeviceDX11.GetDeviceD3D11()->CheckMultisampleQualityLevels(format, sampleCount, &qualityLevels)) && qualityLevels > 0)
	{
		sampleDesc.Count = sampleCount;
		sampleDesc.Quality = qualityLevels - 1;

		sampleCount = sampleCount * 2;
	}

	return sampleDesc;
}

const std::vector<uint8>& TextureDX11::GetBuffer()
{
	/*if (((int)m_Access & (int)EAccess::Read) != 0 && m_DX11Texture2D)
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;

		// Copy the texture data from the texture resource
		if (FAILED(m_RenderDeviceDX11.GetDeviceContextD3D11()->Map(m_DX11Texture2D, 0, D3D11_MAP_READ, 0, &mappedResource)))
		{
			throw CznRenderException("Failed to map texture resource for reading.");
		}

		memcpy_s(m_Buffer.data(), m_Buffer.size(), mappedResource.pData, m_Buffer.size());

		m_RenderDeviceDX11.GetDeviceContextD3D11()->Unmap(m_DX11Texture2D, 0);
	}*/

	return m_Buffer;
}



//
// ITextureInternal
//
void TextureDX11::SetFileName(const std::string & FileName)
{
	m_FileName = FileName;
}




ID3D11Resource* TextureDX11::GetTextureResource() const
{
	ID3D11Resource* resource = nullptr;
	switch (m_TextureDimension)
	{
	case ITexture::Dimension::Texture2D:
	case ITexture::Dimension::Texture2DArray:
		resource = m_DX11Texture2D;
		break;
	case ITexture::Dimension::TextureCube:
		resource = m_DX11Texture3D;
		break;
	}

	return resource;
}

ID3D11ShaderResourceView* TextureDX11::GetShaderResourceView() const
{
	return m_DX11ShaderResourceView;
}

ID3D11DepthStencilView* TextureDX11::GetDepthStencilView() const
{
	return m_DX11DepthStencilView;
}

ID3D11RenderTargetView* TextureDX11::GetRenderTargetView() const
{
	return m_DX11RenderTargetView;
}

ID3D11UnorderedAccessView* TextureDX11::GetUnorderedAccessView() const
{
	return m_DX11UnorderedAccessView;
}
