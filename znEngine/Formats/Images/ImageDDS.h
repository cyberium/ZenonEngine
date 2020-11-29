#pragma once

#include "ImageBase.h"
#include "DDSFormat.h"

class CImageDDS
	: public CImageBase
{
public:
	CImageDDS();
	CImageDDS(uint32 Width, uint32 Height, uint32 BitsPerPixel);
	virtual ~CImageDDS();

protected:
	bool LoadImageData(std::shared_ptr<IFile> File);
	bool LoadRGB(const DDSFormat::DDSURFACEDESC2& desc, std::shared_ptr<IFile> io);
	bool LoadDXT(int type, const DDSFormat::DDSURFACEDESC2& desc, std::shared_ptr<IFile> io);

	template <class DECODER>
	bool LoadDXT_Helper(std::shared_ptr<IFile> io);

public: // Static
	static bool IsFilenameSupported(const std::string& Filename);
	static bool IsFileSupported(std::shared_ptr<IFile> File);
	static std::shared_ptr<CImageDDS> CreateEmptyImage(uint32 Width, uint32 Height, uint32 BitsPerPixel);
	static std::shared_ptr<CImageDDS> CreateImage(std::shared_ptr<IFile> File);
};
