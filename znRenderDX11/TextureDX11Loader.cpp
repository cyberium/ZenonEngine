#include "stdafx.h"

// General
#include "TextureDX11.h"

// Additional
#include "TextureDX11Translate.h"

bool TextureDX11::LoadTextureCustom(uint16_t width, uint16_t height, void * pixels)
{
	m_TextureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TextureDimension = ITexture::Dimension::Texture2D;
	m_TextureWidth = width;
	m_TextureHeight = height;
	m_BPP = 4;
	m_bIsTransparent = FALSE;
	m_NumSlices = 1;
	m_Pitch = (m_TextureWidth) * 4;

	m_ShaderResourceViewFormat = m_RenderTargetViewFormat = m_TextureResourceFormat;
	m_SampleDesc = GetSupportedSampleCount(m_TextureResourceFormat, 1);

	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport)))
	{
		Log::Error("Failed to query format support.");
	}

	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURE2D) == 0)
	{
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for 2D textures.");
		return false;
	}

	m_ShaderResourceViewFormatSupport = m_RenderTargetViewFormatSupport = m_TextureResourceFormatSupport;

	// Can mipmaps be automatically generated for this texture format?
	m_bGenerateMipmaps = !m_bDynamic && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0;

	// Load the texture data into a GPU texture.
	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = m_TextureWidth;
	textureDesc.Height = m_TextureHeight;
	textureDesc.MipLevels = m_bGenerateMipmaps ? 0 : 1;
	textureDesc.ArraySize = m_NumSlices;
	textureDesc.Format = m_TextureResourceFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	if ((m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}
	if ((m_RenderTargetViewFormatSupport & D3D11_FORMAT_SUPPORT_RENDER_TARGET) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = m_bGenerateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

	// Subresource
	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = pixels;
	subresourceData.SysMemPitch = m_Pitch;
	subresourceData.SysMemSlicePitch = 0;
	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateTexture2D(&textureDesc, m_bGenerateMipmaps ? nullptr : &subresourceData, &m_pTexture2D)))
	{
		Log::Error("Failed to create texture.");
		return false;
	}

	// Create a Shader resource view for the texture.
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	resourceViewDesc.Format = m_ShaderResourceViewFormat;
	resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = m_bGenerateMipmaps ? -1 : 1;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateShaderResourceView(m_pTexture2D, &resourceViewDesc, &m_pShaderResourceView)))
	{
		Log::Error("Failed to create texture resource view.");
		return false;
	}

	// From DirectXTK (28/05/2015) @see https://directxtk.codeplex.com/
	if (m_bGenerateMipmaps)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->UpdateSubresource(m_pTexture2D, 0, nullptr, pixels, m_Pitch, 0);
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->GenerateMips(m_pShaderResourceView);
	}

	m_bIsDirty = false;

	return true;
}

#if 0

bool TextureDX11::LoadTexture2D(const std::string& fileName)
{
	std::shared_ptr<IFile> f = m_RenderDevice.lock()->GetBaseManager()->GetManager<IFilesManager>()->Open(fileName);
	if (f == nullptr)
		return false;

	FIMEMORY *hmem = FreeImage_OpenMemory(const_cast<BYTE*>(f->getData()), f->getSize());

	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem, f->getSize());
	if (fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading(fif))
	{
		_ASSERT_EXPR(false, "Unknow file format.");
		return false;
	}

	FIBITMAP* dib = FreeImage_LoadFromMemory(fif, hmem, f->getSize());
	if (dib == nullptr || FreeImage_HasPixels(dib) == FALSE)
	{
		//_ASSERT_EXPR(false, "Failed to load image.");
		Log::Error("TextureDX11: Error while loading '%s': Unknown format.", fileName.c_str());
		return false;
	}

	//// Check to see if we need to flip the image
	//for ( int model = 0; model < FIMD_EXIF_RAW + 1; model++ )
	//{
	//    PrintMetaData( (FREE_IMAGE_MDMODEL)model, dib );
	//}

	m_BPP = FreeImage_GetBPP(dib);
	FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(dib);

	// Check to see if the texture has an alpha channel.
	m_bIsTransparent = (FreeImage_IsTransparent(dib) == TRUE);

	switch (m_BPP)
	{
	case 8:
	{
		switch (imageType)
		{
		case FIT_BITMAP:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R8_UNORM;
		}
		break;
		default:
		{
			_ASSERT_EXPR(false, "Unknown image format.");
		}
		break;
		}
	}
	break;
	case 16:
	{
		switch (imageType)
		{
		case FIT_BITMAP:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R8G8_UNORM;
		}
		break;
		case FIT_UINT16:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R16_UINT;
		}
		break;
		case FIT_INT16:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R16_SINT;
		}
		break;
		default:
		{
			_ASSERT_EXPR(false, "Unknown image format.");
		}
		break;
		}
	}
	break;
	case 32:
	{
		switch (imageType)
		{
		case FIT_BITMAP:
		{
#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
			m_TextureResourceFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
#else
			m_TextureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
		}
		break;
		case FIT_FLOAT:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R32_FLOAT;
		}
		break;
		case FIT_INT32:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R32_SINT;
		}
		break;
		case FIT_UINT32:
		{
			m_TextureResourceFormat = DXGI_FORMAT_R32_UINT;
		}
		break;
		default:
		{
			_ASSERT_EXPR(false, "Unknown image format.");
		}
		break;
		}
	}
	break;
	default:
	{
		FIBITMAP* dib32 = FreeImage_ConvertTo32Bits(dib);

		// Unload the original image.
		FreeImage_Unload(dib);

		dib = dib32;

		// Update pixel bit depth (should be 32 now if it wasn't before).
		m_BPP = FreeImage_GetBPP(dib);

#if FREEIMAGE_COLORORDER == FREEIMAGE_COLORORDER_BGR
		m_TextureResourceFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
#else
		m_TextureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
	}
	break;
	}

	m_TextureDimension = ITexture::Dimension::Texture2D;
	m_TextureWidth = FreeImage_GetWidth(dib);
	m_TextureHeight = FreeImage_GetHeight(dib);
	m_NumSlices = 1;
	m_Pitch = FreeImage_GetPitch(dib);

	m_ShaderResourceViewFormat = m_RenderTargetViewFormat = m_TextureResourceFormat;
	m_SampleDesc = GetSupportedSampleCount(m_TextureResourceFormat, 1);

	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport)))
	{
		_ASSERT_EXPR(false, "Failed to query format support.");
	}

	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURE2D) == 0)
	{
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for 2D textures.");
		return false;
	}

	m_ShaderResourceViewFormatSupport = m_RenderTargetViewFormatSupport = m_TextureResourceFormatSupport;

	// Can mipmaps be automatically generated for this texture format?
	m_bGenerateMipmaps = !m_bDynamic && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0;

	// Load the texture data into a GPU texture.
	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = m_TextureWidth;
	textureDesc.Height = m_TextureHeight;
	textureDesc.MipLevels = m_bGenerateMipmaps ? 0 : 1;
	textureDesc.ArraySize = m_NumSlices;
	textureDesc.Format = m_TextureResourceFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	if ((m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}
	if ((m_RenderTargetViewFormatSupport & D3D11_FORMAT_SUPPORT_RENDER_TARGET) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = m_bGenerateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

	BYTE* textureData = FreeImage_GetBits(dib);

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = textureData;
	subresourceData.SysMemPitch = m_Pitch;
	subresourceData.SysMemSlicePitch = 0;

	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateTexture2D(&textureDesc, m_bGenerateMipmaps ? nullptr : &subresourceData, &m_pTexture2D)))
	{
		_ASSERT_EXPR(false, "Failed to create texture.");
		return false;
	}

	// Create a Shader resource view for the texture.
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;

	resourceViewDesc.Format = m_ShaderResourceViewFormat;
	resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = m_bGenerateMipmaps ? -1 : 1;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateShaderResourceView(m_pTexture2D, &resourceViewDesc, &m_pShaderResourceView)))
	{
		_ASSERT_EXPR(false, "Failed to create texture resource view.");
		return false;
	}

	// From DirectXTK (28/05/2015) @see https://directxtk.codeplex.com/
	if (m_bGenerateMipmaps)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->UpdateSubresource(m_pTexture2D, 0, nullptr, textureData, m_Pitch, 0);
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->GenerateMips(m_pShaderResourceView);
	}

	m_bIsDirty = false;

	// Unload the texture (it should now be on the GPU anyways).
	FreeImage_Unload(dib);

	return true;
}

#else

bool TextureDX11::LoadTexture2D(const std::string& fileName)
{
	std::shared_ptr<IFile> f = m_RenderDeviceD3D11->GetDevice()->GetBaseManager()->GetManager<IFilesManager>()->Open(fileName);
	if (f == nullptr)
	{
		return false;
	}

	std::shared_ptr<IImage> image = m_RenderDeviceD3D11->GetDevice()->GetBaseManager()->GetManager<IImagesFactory>()->CreateImage(f);
	if (image == nullptr)
	{
		return false;
	}

	//
	// DirectX
	//

	m_BPP = image->GetBitsPerPixel();
	m_bIsTransparent = image->IsTransperent();
	m_TextureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TextureDimension = ITexture::Dimension::Texture2D;
	m_TextureWidth = image->GetWidth();
	m_TextureHeight = image->GetHeight();
	m_NumSlices = 1;
	m_Pitch = image->GetStride();

	m_ShaderResourceViewFormat = m_RenderTargetViewFormat = m_TextureResourceFormat;
	m_SampleDesc = GetSupportedSampleCount(m_TextureResourceFormat, 1);

	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport)))
	{
		_ASSERT_EXPR(false, "Failed to query format support.");
	}

	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURE2D) == 0)
	{
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for 2D textures.");
		return false;
	}

	m_ShaderResourceViewFormatSupport = m_RenderTargetViewFormatSupport = m_TextureResourceFormatSupport;

	// Can mipmaps be automatically generated for this texture format?
	m_bGenerateMipmaps = !m_bDynamic && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0;

	// Load the texture data into a GPU texture.
	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = m_TextureWidth;
	textureDesc.Height = m_TextureHeight;
	textureDesc.MipLevels = m_bGenerateMipmaps ? 0 : 1;
	textureDesc.ArraySize = m_NumSlices;
	textureDesc.Format = m_TextureResourceFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	if ((m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_SHADER_SAMPLE) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
	}
	if ((m_RenderTargetViewFormatSupport & D3D11_FORMAT_SUPPORT_RENDER_TARGET) != 0)
	{
		textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	}
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = m_bGenerateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;


	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = image->GetData();
	subresourceData.SysMemPitch = image->GetStride();
	subresourceData.SysMemSlicePitch = 0;

	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateTexture2D(&textureDesc, m_bGenerateMipmaps ? nullptr : &subresourceData, &m_pTexture2D)))
	{
		_ASSERT_EXPR(false, "Failed to create texture.");
		return false;
	}

	// Create a Shader resource view for the texture.
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;

	resourceViewDesc.Format = m_ShaderResourceViewFormat;
	resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = m_bGenerateMipmaps ? -1 : 1;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateShaderResourceView(m_pTexture2D, &resourceViewDesc, &m_pShaderResourceView)))
	{
		_ASSERT_EXPR(false, "Failed to create texture resource view.");
		return false;
	}

	// From DirectXTK (28/05/2015) @see https://directxtk.codeplex.com/
	if (m_bGenerateMipmaps)
	{
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->UpdateSubresource(m_pTexture2D, 0, nullptr, image->GetData(), image->GetStride(), 0);
		m_RenderDeviceD3D11->GetDeviceContextD3D11()->GenerateMips(m_pShaderResourceView);
	}

	m_bIsDirty = false;

	return true;
}

#endif

bool TextureDX11::LoadTextureCube(const std::string& fileName)
{
	/*fs::path filePath( fileName );
	if ( !fs::exists( filePath ) || !fs::is_regular_file( filePath ) )
	{
		Log::Error( "Could not load texture: " + filePath.std::string() );
		return false;
	}

	m_TextureFileName = fileName;
	m_DependencyTracker = DependencyTracker( fileName );
	// Try to load the dependency file for the texture asset.
	if ( !m_DependencyTracker.Load() )
	{
		// If loading failed, likely, the dependency tracker file
		// does not exist. Save the default dependency tracker.
		m_DependencyTracker.Save();
	}

	m_DependencyTracker.SetLastLoadTime();

	// Try to determine the file type from the image file.
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeU( filePath.c_str() );
	if ( fif == FIF_UNKNOWN )
	{
		fif = FreeImage_GetFIFFromFilenameU( filePath.c_str() );
	}

	if ( fif == FIF_UNKNOWN || !FreeImage_FIFSupportsReading( fif ) )
	{
		Log::Error( "Unknow file format: " + filePath.std::string() );
		return false;
	}

	FIMULTIBITMAP* dib = FreeImage_OpenMultiBitmap( fif, filePath.std::string().c_str(), FALSE, TRUE, TRUE );
	if ( dib == nullptr || FreeImage_GetPageCount( dib ) == 0 )
	{
		Log::Error( "Failed to load image: " + filePath.std::string() );
		return false;
	}

	int pageCount = FreeImage_GetPageCount( dib );*/

	// TODO: DDS cubemap loading with FreeImage?

	return false;
}