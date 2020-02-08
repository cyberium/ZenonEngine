#pragma once

#include "ImageLoaderTemplate.h"
#include "DDSFormat.h"

class CImageDDS
	: public CImageBase
{
public:
	CImageDDS();
	virtual ~CImageDDS();

protected:
	bool LoadImageData(std::shared_ptr<IFile> File);
	bool LoadRGB(const DDSURFACEDESC2& desc, std::shared_ptr<IFile> io);
	bool LoadDXT(int type, const DDSURFACEDESC2& desc, std::shared_ptr<IFile> io);

	template <class DECODER>
	bool LoadDXT_Helper(std::shared_ptr<IFile> io);

public: // Static
	static bool IsFileSupported(std::shared_ptr<IFile> File);
	static std::shared_ptr<CImageDDS> CreateImage(std::shared_ptr<IFile> File);
};
