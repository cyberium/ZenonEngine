#pragma once

template <typename TIMAGE>
class ZN_API CImageLoaderT
	: public IImageLoader
{
public:
	CImageLoaderT()
	{}
	virtual ~CImageLoaderT()
	{}

	// IImageLoader
	inline bool IsFilenameSupported(const std::string& Filename) const override
	{
		return TIMAGE::IsFilenameSupported(Filename);
	}
	inline bool IsFileSupported(std::shared_ptr<IFile> File) const override
	{
		return TIMAGE::IsFileSupported(File);
	}
	inline std::shared_ptr<IImage> CreateEmptyImage(uint32 Width, uint32 Height, uint32 BitsPerPixel) const override
	{
		return TIMAGE::CreateEmptyImage(Width, Height, BitsPerPixel);
	}
	inline std::shared_ptr<IImage> CreateImage(std::shared_ptr<IFile> File) const override
	{
		std::shared_ptr<TIMAGE> image = TIMAGE::CreateImage(File);
		return image->ConvertAnyTo32Bit();
	}
};
