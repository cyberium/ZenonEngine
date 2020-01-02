#pragma once

class CImageBase
	: public IImage
{
public:
	CImageBase();
	virtual ~CImageBase();

	std::shared_ptr<IImage> Convert24To32Bit();

	// IImage
	uint32 GetWidth() const override;
	uint32 GetHeight() const override;
	uint32 GetBitsPerPixel() const override;
	uint32 GetStride() const override;
	bool IsTransperent() const override;
	const uint8* GetData() const override;

protected:
	uint8* GetLine(uint32 Line) const;

protected:
	uint32 m_Width;
	uint32 m_Height;
	uint32 m_BitsPerPixel;
	uint32 m_Stride;
	bool m_IsTransperent;
	uint8* m_Data;
};

template <typename TIMAGE>
class CImageLoaderT
	: public IImageLoader
{
public:
	CImageLoaderT()
	{}
	virtual ~CImageLoaderT()
	{}

	// IImageLoader
	inline bool IsFileSupported(std::shared_ptr<IFile> File) const override
	{
		return TIMAGE::IsFileSupported(File);
	}
	inline std::shared_ptr<IImage> CreateImage(std::shared_ptr<IFile> File) const override
	{
		std::shared_ptr<TIMAGE> image = TIMAGE::CreateImage(File);
		if (image != nullptr && image->GetBitsPerPixel() == 24)
			return image->Convert24To32Bit();

		return image;
	}
};