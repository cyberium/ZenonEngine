#include "stdafx.h"

// General
#include "ImagePNG.h"

// Additional
#include <png.h>
#pragma comment(lib, "LibPNG.lib")


namespace
{
	void my_png_read_data(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		std::shared_ptr<IFile>* file = ((std::shared_ptr<IFile>*)png_get_io_ptr(pngPtr));
		_ASSERT((*file) != nullptr);

		if (!(*file)->readBytes(data, length))
			throw CException("Error while reading PNG file.");
	}

	void my_png_write_data(png_structp pngPtr, png_bytep data, png_size_t length)
	{
		std::shared_ptr<IByteBuffer>* buffer = ((std::shared_ptr<IByteBuffer>*)png_get_io_ptr(pngPtr));
		_ASSERT((*buffer) != nullptr);

		(*buffer)->writeBytes(data, length);
	}

	void my_png_flush(png_structp png_ptr)
	{
	}
}


CImagePNG::CImagePNG()
{}

CImagePNG::CImagePNG(uint32 Width, uint32 Height, uint32 BitsPerPixel)
	: CImageBase(Width, Height, BitsPerPixel, false)
{}

CImagePNG::~CImagePNG()
{}

bool CImagePNG::LoadImageData(std::shared_ptr<IFile> File)
{
	png_structp pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (pngPtr == NULL)
	{
		throw CException("ImagePNG: png_create_read_struct error.");
	}

	png_infop pngInfoPtr = png_create_info_struct(pngPtr);
	if (pngInfoPtr == NULL)
	{
		png_destroy_read_struct(&pngPtr, NULL, NULL);
		throw CException("ImagePNG: png_create_info_struct error.");
	}

	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &pngInfoPtr, NULL);
		throw CException("ImagePNG: Error while reading PNG file '%s'.", File->Path_Name().c_str());
	}

	// Custom reader
	File->seek(0);
	png_set_read_fn(pngPtr, (png_voidp)&File, my_png_read_data);

	// Receive the file info.
	png_read_info(pngPtr, pngInfoPtr);

	m_Width = png_get_image_width(pngPtr, pngInfoPtr);
	m_Height = png_get_image_height(pngPtr, pngInfoPtr);

	// bits per CHANNEL! note: not per pixel!
	png_uint_32 bitdepth = png_get_bit_depth(pngPtr, pngInfoPtr);

	// Number of channels
	png_uint_32 channels = png_get_channels(pngPtr, pngInfoPtr);

	// Color type. (RGB, RGBA, Luminance, luminance alpha... palette... etc)
	png_uint_32 color_type = png_get_color_type(pngPtr, pngInfoPtr);

	switch (color_type)
	{
		case PNG_COLOR_TYPE_PALETTE:
		{
			png_set_palette_to_rgb(pngPtr);
			channels = 3;
		}
		break;
		case PNG_COLOR_TYPE_GRAY:
		{
			if (bitdepth < 8)
			{
				png_set_expand_gray_1_2_4_to_8(pngPtr);
				bitdepth = 8;
			}
		}
		break;
	}

	// if the image has a transperancy set.. convert it to a full Alpha channel
	if (png_get_valid(pngPtr, pngInfoPtr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(pngPtr);
		m_IsTransperent = true;
		channels += 1;
	}

	// We don't support 16 bit precision.. so if the image Has 16 bits per channel
	if (bitdepth == 16)
	{
		png_set_strip_16(pngPtr);
	}

	png_read_update_info(pngPtr, pngInfoPtr);

	m_BitsPerPixel = channels * bitdepth;

	// This is the length in bytes, of one row.
	m_Stride = m_Width * bitdepth * channels / 8;


	//Alocate a buffer with enough space.
	//(Don't use the stack, these blocks get big easilly)
	//This pointer was also defined in the error handling section, so we can clean it up on error.
	m_Data = ZN_NEW png_byte[m_Height * m_Stride];

	png_bytep* rowPtrs = ZN_NEW png_bytep[m_Height];
	for (uint32 i = 0; i < m_Height; i++)
		rowPtrs[i] = GetLine(i);

	png_read_image(pngPtr, rowPtrs);

	if (rowPtrs != nullptr)
		delete[] rowPtrs;

	png_destroy_read_struct(&pngPtr, &pngInfoPtr, NULL);

	return true;
}



//
// Static
//
bool CImagePNG::IsFilenameSupported(const std::string & Filename)
{
	auto lastPointPos = Filename.find_last_of('.');
	if (lastPointPos != std::string::npos)
	{
		std::string extension = Filename.substr(lastPointPos + 1);
		extension = Utils::ToLower(extension);
		return extension == "png";
	}

	return false;
}

bool CImagePNG::IsFileSupported(std::shared_ptr<IFile> File)
{
	_ASSERT(File != nullptr);

	if (!File->Extension().empty())
	{
		return strcmp(Utils::ToLower(File->Extension()).c_str(), "png") == 0;
	}

	const size_t cPNGSigSize = 8;
	png_byte pngsig[cPNGSigSize];
	File->seek(0);
	File->readBytes(pngsig, cPNGSigSize);
	File->seek(0);
	return png_sig_cmp(pngsig, 0, cPNGSigSize) == 0;
}

std::shared_ptr<CImagePNG> CImagePNG::CreateEmptyImage(uint32 Width, uint32 Height, uint32 BitsPerPixel)
{
	return MakeShared(CImagePNG, Width, Height, BitsPerPixel);
}

std::shared_ptr<CImagePNG> CImagePNG::CreateImage(std::shared_ptr<IFile> File)
{
	_ASSERT(IsFileSupported(File));

	std::shared_ptr<CImagePNG> imagePNG = MakeShared(CImagePNG);
	if (false == imagePNG->LoadImageData(File))
	{
		Log::Error("ImagePNG: Unable to load PNG file '%s'.", File->Name().c_str());
		return nullptr;
	}

	return imagePNG;
}

std::shared_ptr<IByteBuffer> CImagePNG::SaveToMemory()
{
	std::shared_ptr<IByteBuffer> buffer = MakeShared(CByteBuffer);

	png_structp pngPtr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (pngPtr == NULL)
	{
		throw CException("ImagePNG: png_create_write_struct error.");
	}

	png_infop pngInfoPtr = png_create_info_struct(pngPtr);
	if (pngInfoPtr == NULL)
	{
		png_destroy_write_struct(&pngPtr, NULL);
		throw CException("ImagePNG: png_create_info_struct error.");
	}

	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_write_struct(&pngPtr, &pngInfoPtr);
		throw CException("ImagePNG: Error while saving PNG file.");
	}

	png_set_write_fn(pngPtr, (png_voidp)&buffer, my_png_write_data, my_png_flush);

	std::shared_ptr<IImage> image = ConvertAnyTo32Bit();
	uint32 bitsPP = image->GetBitsPerPixel();

	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
		pngPtr,
		pngInfoPtr,
		image->GetWidth(),
		image->GetHeight(),
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);
	png_write_info(pngPtr, pngInfoPtr);

	// To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
	// Use png_set_filler().
	//png_set_filler(pngPtr, 0, PNG_FILLER_AFTER);

	png_bytep* rowPtrs = ZN_NEW png_bytep[image->GetHeight()];
	for (uint32 i = 0; i < image->GetHeight(); i++)
		rowPtrs[image->GetHeight() - i - 1] = std::dynamic_pointer_cast<CImagePNG>(image)->GetLine(i);

	png_write_image(pngPtr, rowPtrs);

	if (rowPtrs != nullptr)
		delete[] rowPtrs;

	png_write_end(pngPtr, NULL);

	png_destroy_write_struct(&pngPtr, &pngInfoPtr);

	return buffer;
}
