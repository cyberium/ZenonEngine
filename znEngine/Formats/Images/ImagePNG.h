#pragma once

#include "ImageBase.h"

class CImagePNG
	: public CImageBase
{
public:
	CImagePNG();
	CImagePNG(uint32 Width, uint32 Height, uint32 BitsPerPixel);
	virtual ~CImagePNG();

	std::shared_ptr<IByteBuffer> SaveToMemory() override;

protected:
	bool LoadImageData(std::shared_ptr<IFile> File);
	
public: // Static
	static bool IsFilenameSupported(const std::string& Filename);
	static bool IsFileSupported(std::shared_ptr<IFile> File);
	static std::shared_ptr<CImagePNG> CreateEmptyImage(uint32 Width, uint32 Height, uint32 BitsPerPixel);
	static std::shared_ptr<CImagePNG> CreateImage(std::shared_ptr<IFile> File);
};
