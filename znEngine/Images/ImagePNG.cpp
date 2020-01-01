#include "stdafx.h"

// General
#include "ImagePNG.h"

// Additional
#pragma comment(lib, "LibPNG.lib")

CImagePNG::CImagePNG()
{}

CImagePNG::~CImagePNG()
{}

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



	// This is the length in bytes, of one row.
	m_Stride = m_Width * bitdepth * channels / 8;


	//Alocate a buffer with enough space.
	//(Don't use the stack, these blocks get big easilly)
	//This pointer was also defined in the error handling section, so we can clean it up on error.
	m_Data = new png_byte[m_Height * m_Stride];

	png_bytep* rowPtrs = new png_bytep[m_Height];
	for (uint32 i = 0; i < m_Height; i++)
		rowPtrs[i] = GetLine(i);

	png_read_image(m_PngPtr, rowPtrs);

	if (rowPtrs != nullptr)
		delete[] rowPtrs;

	png_destroy_read_struct(&m_PngPtr, &m_InfoPtr, NULL);

	return true;
}



//
// Static
//
bool CImagePNG::IsFileSupported(std::shared_ptr<IFile> File)
{
	_ASSERT(File != nullptr);

	if (!File->Extension().empty())
	{
		return Utils::ToLower(File->Extension()) == "png";
	}

	const size_t cPNGSigSize = 8;
	png_byte pngsig[cPNGSigSize];
	File->seek(0);
	File->readBytes(pngsig, cPNGSigSize);
	File->seek(0);
	return png_sig_cmp(pngsig, 0, cPNGSigSize) == 0;
}

std::shared_ptr<IImage> CImagePNG::CreateImage(std::shared_ptr<IFile> File)
{
	_ASSERT(IsFileSupported(File));

	std::shared_ptr<CImagePNG> imagePNG = std::make_shared<CImagePNG>();
	if (!imagePNG->LoadImageData(File))
	{
		Log::Error("CImagePNG: Unable to load PNG file '%s'.", File->Name().c_str());
		return nullptr;
	}

	return imagePNG;
}
