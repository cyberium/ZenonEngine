#include "stdafx.h"

// General
#include "ImageLoaderPNG.h"

// Additional
#pragma comment(lib, "LibPNG.lib")

CImagePNG::CImagePNG()
	: m_Width(0)
	, m_Height(0)
	, m_BitsPerPixel(0)
	, m_Stride(0)
	, m_IsTransperent(false)
	, m_Data(nullptr)
{
}

CImagePNG::~CImagePNG()
{
	if (m_Data != nullptr)
	{
		delete[] m_Data;
	}
}

uint32 CImagePNG::GetWidth() const
{
	return m_Width;
}

uint32 CImagePNG::GetHeight() const
{
	return m_Height;
}

uint32 CImagePNG::GetBitsPerPixel() const
{
	return m_BitsPerPixel;
}

uint32 CImagePNG::GetStride() const
{
	return m_Stride;
}

bool CImagePNG::IsTransperent() const
{
	return m_IsTransperent;
}

const uint8* CImagePNG::GetData() const
{
	return m_Data;
}

void userReadData(png_structp pngPtr, png_bytep data, png_size_t length)
{
	std::shared_ptr<IFile>* file = ((std::shared_ptr<IFile>*)png_get_io_ptr(pngPtr));
	_ASSERT((*file) != nullptr);

	if (!(*file)->readBytes(data, length))
		_ASSERT_EXPR(false, L"CImagePNG: Error while reading PNG file.");
}

bool CImagePNG::LoadImageData(std::shared_ptr<IFile> File)
{
	m_PngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (m_PngPtr == NULL)
	{
		Log::Error("CImagePNG: png_create_read_struct error.");
		return false;
	}

	m_InfoPtr = png_create_info_struct(m_PngPtr);
	if (m_InfoPtr == NULL)
	{
		Log::Error("CImagePNG: png_create_info_struct error.");
		png_destroy_read_struct(&m_PngPtr, NULL, NULL);
		return false;
	}

	// Custom reader
	File->seek(0);
	png_set_read_fn(m_PngPtr, (png_voidp)&File, userReadData);

	// Receive the file info.
	png_read_info(m_PngPtr, m_InfoPtr);

	m_Width = png_get_image_width(m_PngPtr, m_InfoPtr);
	m_Height = png_get_image_height(m_PngPtr, m_InfoPtr);

	// bits per CHANNEL! note: not per pixel!
	png_uint_32 bitdepth = png_get_bit_depth(m_PngPtr, m_InfoPtr);

	// Number of channels
	png_uint_32 channels = png_get_channels(m_PngPtr, m_InfoPtr);

	// Color type. (RGB, RGBA, Luminance, luminance alpha... palette... etc)
	png_uint_32 color_type = png_get_color_type(m_PngPtr, m_InfoPtr);

	switch (color_type)
	{
		case PNG_COLOR_TYPE_PALETTE:
		{
			png_set_palette_to_rgb(m_PngPtr);
			channels = 3;
		}
		break;
		case PNG_COLOR_TYPE_GRAY:
		{
			if (bitdepth < 8)
			{
				png_set_expand_gray_1_2_4_to_8(m_PngPtr);
				bitdepth = 8;
			}
		}
		break;
	}

	// if the image has a transperancy set.. convert it to a full Alpha channel
	if (png_get_valid(m_PngPtr, m_InfoPtr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(m_PngPtr);
		m_IsTransperent = true;
		channels += 1;
	}

	// We don't support 16 bit precision.. so if the image Has 16 bits per channel
	if (bitdepth == 16)
	{
		png_set_strip_16(m_PngPtr);
	}

	png_read_update_info(m_PngPtr, m_InfoPtr);

	m_BitsPerPixel = channels * bitdepth;

	//Here's one of the pointers we've defined in the error handler section:
	//Array of row pointers. One for every row.
	png_bytep* rowPtrs = new png_bytep[m_Height];

	//Alocate a buffer with enough space.
	//(Don't use the stack, these blocks get big easilly)
	//This pointer was also defined in the error handling section, so we can clean it up on error.
	m_Data = new png_byte[m_Width * m_Height * bitdepth * channels / 8];

	// This is the length in bytes, of one row.
	m_Stride = m_Width * bitdepth * channels / 8;

	// A little for-loop here to set all the row pointers to the starting
	// Adresses for every row in the buffer

	for (uint32 i = 0; i < m_Height; i++)
	{
#if 0 // DirectX
		png_uint_32 q = (i) * m_Stride;
#else // OpenGL
		png_uint_32 q = (m_Height - i - 1) * m_Stride;
#endif
		rowPtrs[i] = (png_bytep)m_Data + q;
	}

	png_read_image(m_PngPtr, rowPtrs);

	if (rowPtrs != nullptr)
	{
		delete[] rowPtrs;
	}

	png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, NULL);

	return true;
}


// -------------------------------------------


CImageLoaderPNG::CImageLoaderPNG()
{}

CImageLoaderPNG::~CImageLoaderPNG()
{}

std::string CImageLoaderPNG::GetName() const
{
	return "PNG Loader";
}

std::string CImageLoaderPNG::GetSupportedExtention() const
{
	return "png";
}

#define PNGSIGSIZE 8
bool CImageLoaderPNG::IsFileSupported(std::shared_ptr<IFile> File) const
{
	_ASSERT(File != nullptr);

	png_byte pngsig[PNGSIGSIZE];
	File->seek(0);
	File->readBytes(pngsig, PNGSIGSIZE);
	File->seek(0);
	return png_sig_cmp(pngsig, 0, PNGSIGSIZE) == 0;
}

std::shared_ptr<IImage> CImageLoaderPNG::LoadImage(std::shared_ptr<IFile> File) const
{
	_ASSERT(File != nullptr);
	_ASSERT(IsFileSupported(File));

	std::shared_ptr<CImagePNG> imagePNG = std::make_shared<CImagePNG>();
	if (!imagePNG->LoadImageData(File))
	{
		Log::Error("CImageLoaderPNG: Unable to load PNG file '%s'.", File->Name().c_str());
		return nullptr;
	}

	return imagePNG;
}
