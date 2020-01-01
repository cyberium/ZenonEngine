#pragma once

#include "ImageLoaderTemplate.h"
#include <png.h>

class CImagePNG
	: public CImageBase
{
public:
	CImagePNG();
	virtual ~CImagePNG();

protected:
	bool LoadImageData(std::shared_ptr<IFile> File);

public: // Static
	static bool IsFileSupported(std::shared_ptr<IFile> File);
	static std::shared_ptr<IImage> CreateImage(std::shared_ptr<IFile> File);

private:
	png_structp m_PngPtr;
	png_infop m_InfoPtr;
};
