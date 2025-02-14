#include "stdafx.h"

// General
#include "TextureDX11.h"

// Additional
#include "TextureDX11Translate.h"

/*bool TextureDX11::LoadTexture2D(const std::string& fileName)
{
	std::shared_ptr<IFile> f = m_RenderDevice.lock()->GetBaseManager().GetManager<IFilesManager>()->Open(fileName);
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
		throw CznRenderException("TextureDX11: Error while loading '%s': Unknown format.", fileName.c_str());
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

	if (FAILED(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport)))
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
	m_NeedGenerateMipmaps = !m_bDynamic && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0;

	// Load the texture data into a GPU texture.
	D3D11_TEXTURE2D_DESC textureDesc = { 0 };
	textureDesc.Width = m_TextureWidth;
	textureDesc.Height = m_TextureHeight;
	textureDesc.MipLevels = m_NeedGenerateMipmaps ? 0 : 1;
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
	textureDesc.MiscFlags = m_NeedGenerateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;

	BYTE* textureData = FreeImage_GetBits(dib);

	D3D11_SUBRESOURCE_DATA subresourceData;
	subresourceData.pSysMem = textureData;
	subresourceData.SysMemPitch = m_Pitch;
	subresourceData.SysMemSlicePitch = 0;

	if (FAILED(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, m_NeedGenerateMipmaps ? nullptr : &subresourceData, &m_DX11Texture2D)))
	{
		_ASSERT_EXPR(false, "Failed to create texture.");
		return false;
	}

	// Create a Shader resource view for the texture.
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;

	resourceViewDesc.Format = m_ShaderResourceViewFormat;
	resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MipLevels = m_NeedGenerateMipmaps ? -1 : 1;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;

	if (FAILED(m_RenderDeviceDX11.GetDeviceD3D11()->CreateShaderResourceView(m_DX11Texture2D, &resourceViewDesc, &m_DX11ShaderResourceView)))
	{
		_ASSERT_EXPR(false, "Failed to create texture resource view.");
		return false;
	}

	// From DirectXTK (28/05/2015) @see https://directxtk.codeplex.com/
	if (m_NeedGenerateMipmaps)
	{
		m_RenderDeviceDX11.GetDeviceContextD3D11()->UpdateSubresource(m_DX11Texture2D, 0, nullptr, textureData, m_Pitch, 0);
		m_RenderDeviceDX11.GetDeviceContextD3D11()->GenerateMips(m_DX11ShaderResourceView);
	}

	m_bIsDirty = false;

	// Unload the texture (it should now be on the GPU anyways).
	FreeImage_Unload(dib);

	return true;
}*/

void TextureDX11::LoadTexture2D(const std::string& fileName)
{
	std::shared_ptr<IFile> f = m_RenderDeviceDX11.GetBaseManager().GetManager<IFilesManager>()->Open(fileName);
	if (f == nullptr)
		throw CException("File '%s' not found.", f->Path_Name());

	std::shared_ptr<IImage> image = m_RenderDeviceDX11.GetBaseManager().GetManager<IImagesFactory>()->CreateImage(f);
	if (image == nullptr)
		throw CException("Unable load image '%s'.", f->Path_Name());

	LoadTexture2DFromImage(image);
}

void TextureDX11::LoadTexture2DFromImage(const std::shared_ptr<IImage>& Image)
{
	m_BPP = Image->GetBitsPerPixel();
	//m_bIsTransparent = Image->IsTransperent(); // TODO: Fixme
	m_TextureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TextureDimension = ITexture::Dimension::Texture2D;
	m_TextureWidth = Image->GetWidth();
	m_TextureHeight = Image->GetHeight();
	m_NumSlices = 1;
	m_Pitch = Image->GetStride();

	m_ShaderResourceViewFormat = m_RenderTargetViewFormat = m_TextureResourceFormat;
	m_SampleDesc = GetSupportedSampleCount(m_TextureResourceFormat, 1);

	CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport));

	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURE2D) == 0)
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for 2D textures.");

	m_ShaderResourceViewFormatSupport = m_RenderTargetViewFormatSupport = m_TextureResourceFormatSupport;

	// Can mipmaps be automatically generated for this texture format?
	m_NeedGenerateMipmaps = (false == m_bDynamic) && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0;


	// Load the texture data into a GPU texture.
	D3D11_TEXTURE2D_DESC textureDesc = { };
	textureDesc.Width = m_TextureWidth;
	textureDesc.Height = m_TextureHeight;
	textureDesc.MipLevels = m_NeedGenerateMipmaps ? 0 : 1;
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
	textureDesc.MiscFlags = m_NeedGenerateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0;



	if (m_NeedGenerateMipmaps)
	{
		m_Buffer[0].resize(Image->GetHeight() * Image->GetStride());
		m_Buffer[0].assign(Image->GetData(), Image->GetData() + (Image->GetHeight() * Image->GetStride()));

		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, NULL, &m_DX11Texture2D), L"Failed to create texture with mipmaps.");
	}
	else
	{
		m_Buffer[0].resize(Image->GetHeight() * Image->GetStride());
		m_Buffer[0].assign(Image->GetData(), Image->GetData() + (Image->GetHeight() * Image->GetStride()));

		/*D3D11_SUBRESOURCE_DATA subresourceData = { };
		subresourceData.pSysMem = Image->GetData();
		subresourceData.SysMemPitch = Image->GetStride();
		subresourceData.SysMemSlicePitch = 0;*/
		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, /*&subresourceData*/ NULL, &m_DX11Texture2D), L"Failed to create texture.");
	}



	/*

	if (m_NeedGenerateMipmaps)
	{
		//D3D11_SUBRESOURCE_DATA subresourceData = { };
		//subresourceData.pSysMem = Image->GetData();
		//subresourceData.SysMemPitch = Image->GetStride();
		//subresourceData.SysMemSlicePitch = 0;

		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, NULL, &m_DX11Texture2D), L"Failed to create texture with mipmaps.");

		m_RenderDeviceDX11.GetDeviceContextD3D11()->UpdateSubresource(m_DX11Texture2D, 0, nullptr, Image->GetData(), m_Pitch, Image->GetDataSize());
	}
	else
	{
		D3D11_SUBRESOURCE_DATA subresourceData = { };
		subresourceData.pSysMem = Image->GetData();
		subresourceData.SysMemPitch = Image->GetStride();
		subresourceData.SysMemSlicePitch = 0;

		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, &subresourceData, &m_DX11Texture2D), L"Failed to create texture.");
	}*/

	// Create a Shader resource view for the texture.
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {};
		resourceViewDesc.Format = m_ShaderResourceViewFormat;
		resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = m_NeedGenerateMipmaps ? -1 : 1;
		resourceViewDesc.Texture2D.MostDetailedMip = 0;

		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateShaderResourceView(m_DX11Texture2D, &resourceViewDesc, &m_DX11ShaderResourceView), L"Failed to create texture resource view.");
	}

	//if (m_NeedGenerateMipmaps)
	//	m_RenderDeviceDX11.GetDeviceContextD3D11()->GenerateMips(m_DX11ShaderResourceView);

	m_bIsDirty = true;
}


//
//          |-------|
//          |   2   |
//          |  +Y   |
//  |-------|-------|-------|-------|
//  |   1   |   4   |   0   |   5   |
//  |  -X   |  +Z   |  +X   |  -Z   |
//  |-------|-------|-------|-------|
//          |   3   |
//          |  -Y   |
//          |-------|
//
//          |-------|
//          |   2   |
//          |  TOP  |
//  |-------|-------|-------|-------|
//  |   1   |   4   |   0   |   5   |
//  |  LEFT | FRONT | RIGHT |  BACK |
//  |-------|-------|-------|-------|
//          |   3   |
//          |BOTTOM |
//          |-------|
//  0 - +X
//  1 - -X
//  2 - +Y
//  3 - -Y
//  4 - +Z
//  5 - -Z


void TextureDX11::LoadTextureCube(const std::string& FileName)
{
	std::vector<std::string> sides;
	sides.push_back("Right");
	sides.push_back("Left");
	sides.push_back("Top");
	sides.push_back("Bottom");
	sides.push_back("Front");
	sides.push_back("Back");

	auto fileNameStruct = Utils::SplitFilename(FileName);
	
	std::vector<std::shared_ptr<IImage>> images;
	for (const auto& side : sides)
	{
		std::string sideImageFilename = fileNameStruct.Path + fileNameStruct.NameWithoutExtension + "_" + side + "." + fileNameStruct.Extension;
		auto sideImage = m_RenderDeviceDX11.GetBaseManager().GetManager<IImagesFactory>()->CreateImage(sideImageFilename);
		images.push_back(sideImage);
	}

	LoadTextureCubeFromImages(images);
}

void TextureDX11::LoadTextureCubeFromImages(const std::vector<std::shared_ptr<IImage>>& Images)
{
	m_BPP = Images[0]->GetBitsPerPixel();
	//m_bIsTransparent = Image->IsTransperent(); // TODO: Fixme
	m_TextureResourceFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_TextureDimension = ITexture::Dimension::TextureCube;
	m_TextureWidth = Images[0]->GetWidth();
	m_TextureHeight = Images[0]->GetHeight();
	m_NumSlices = 6;
	m_Pitch = Images[0]->GetStride();

	m_ShaderResourceViewFormat = m_RenderTargetViewFormat = m_TextureResourceFormat;
	m_SampleDesc = GetSupportedSampleCount(m_TextureResourceFormat, 1);

	CHECK_HR(m_RenderDeviceDX11.GetDeviceD3D11()->CheckFormatSupport(m_TextureResourceFormat, &m_TextureResourceFormatSupport));

	if ((m_TextureResourceFormatSupport & D3D11_FORMAT_SUPPORT_TEXTURE2D) == 0)
		ReportTextureFormatError(m_TextureFormat, "Unsupported texture format for 2D textures.");

	m_ShaderResourceViewFormatSupport = m_RenderTargetViewFormatSupport = m_TextureResourceFormatSupport;

	// Can mipmaps be automatically generated for this texture format?
	m_NeedGenerateMipmaps = false; // !m_bDynamic && (m_ShaderResourceViewFormatSupport & D3D11_FORMAT_SUPPORT_MIP_AUTOGEN) != 0;


	// Load the texture data into a GPU texture.
	D3D11_TEXTURE2D_DESC textureDesc = { };
	textureDesc.Width = m_TextureWidth;
	textureDesc.Height = m_TextureHeight;
	textureDesc.MipLevels = m_NeedGenerateMipmaps ? 0 : 1;
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
	textureDesc.MiscFlags = (m_NeedGenerateMipmaps ? D3D11_RESOURCE_MISC_GENERATE_MIPS : 0) | D3D11_RESOURCE_MISC_TEXTURECUBE;




	if (m_NeedGenerateMipmaps)
	{
		for (size_t i = 0; i < Images.size(); i++)
		{
			const auto& image = Images.at(i);
			size_t imageSize = image->GetHeight() * Images.at(i)->GetStride();

			m_Buffer[i].resize(imageSize);
			m_Buffer[i].assign(image->GetData(), Images.at(i)->GetData() + (Images.at(i)->GetHeight() * Images.at(i)->GetStride()));
		}
		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, NULL, &m_DX11Texture2D), L"Failed to create texture with mipmaps.");
	}
	else
	{
		for (size_t i = 0; i < Images.size(); i++)
		{
			const auto& image = Images.at(i);
			size_t imageSize = image->GetHeight() * image->GetStride();

			m_Buffer[i].resize(imageSize);
			m_Buffer[i].assign(image->GetData(), image->GetData() + imageSize);
		}

		/*D3D11_SUBRESOURCE_DATA subresourceData = { };
		subresourceData.pSysMem = Image->GetData();
		subresourceData.SysMemPitch = Image->GetStride();
		subresourceData.SysMemSlicePitch = 0;*/
		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, NULL/*&subresourceData*/, &m_DX11Texture2D), L"Failed to create texture.");
	}




	/*if (m_NeedGenerateMipmaps)
	{
		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, NULL, &m_DX11Texture2D), L"Failed to create texture with mipmaps.");
	}
	else
	{
		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateTexture2D(&textureDesc, NULL, &m_DX11Texture2D), L"Failed to create texture with mipmaps.");
	}

	size_t index = 0;
	for (const auto& image : Images)
	{
		UINT res = D3D11CalcSubresource(0, index++, 1);
		m_RenderDeviceDX11.GetDeviceContextD3D11()->UpdateSubresource(m_DX11Texture2D, res, nullptr, image->GetData(), m_Pitch, image->GetDataSize());
	}*/

	// Create a Shader resource view for the texture.
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc = {};
		resourceViewDesc.Format = m_ShaderResourceViewFormat;
		resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;
		resourceViewDesc.TextureCube.MipLevels = m_NeedGenerateMipmaps ? -1 : 1;
		resourceViewDesc.TextureCube.MostDetailedMip = 0;

		CHECK_HR_MSG(m_RenderDeviceDX11.GetDeviceD3D11()->CreateShaderResourceView(m_DX11Texture2D, &resourceViewDesc, &m_DX11ShaderResourceView), L"Failed to create texture resource view.");
	}


	//if (m_NeedGenerateMipmaps)
	//	m_RenderDeviceDX11.GetDeviceContextD3D11()->GenerateMips(m_DX11ShaderResourceView);

	//m_Buffer.resize(Images[0]->GetHeight() * Images[0]->GetStride());
	//m_Buffer.assign(Images[0]->GetData(), Images[0]->GetData() + (Images[0]->GetHeight() * Images[0]->GetStride()));

	m_bIsDirty = true;
}