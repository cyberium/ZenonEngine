#pragma once

#include <png.h>

class CImagePNG
	: public IImage
	, public IImageInternal
{
public:
	CImagePNG();
	virtual ~CImagePNG();

	// IImage
	uint32 GetWidth() const override;
	uint32 GetHeight() const override;
	uint32 GetBitsPerPixel() const override;
	uint32 GetStride() const override;
	bool IsTransperent() const override;
	const uint8* GetData() const override;

	// IImageInternal
	bool LoadImageData(std::shared_ptr<IFile> File) override;

private:
	png_structp m_PngPtr;
	png_infop m_InfoPtr;

	uint32 m_Width;
	uint32 m_Height;
	uint32 m_BitsPerPixel;
	uint32 m_Stride;
	bool m_IsTransperent;
	png_bytep m_Data;
};

class CImageLoaderPNG
	: public IImageLoader
{
public:
	CImageLoaderPNG();
	virtual ~CImageLoaderPNG();

	// IImageLoader
	std::string GetName() const override;
	std::string GetSupportedExtention() const override;
	bool IsFileSupported(std::shared_ptr<IFile> File) const  override;
	std::shared_ptr<IImage> LoadImage(std::shared_ptr<IFile> File) const  override;
};
