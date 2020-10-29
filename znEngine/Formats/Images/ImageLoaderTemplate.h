#pragma once

class ZN_API CImageBase
	: public IImage
{
public:
	CImageBase();
	CImageBase(uint32 Width, uint32 Height, uint32 BitsPerPixel, bool IsTransperent);
	virtual ~CImageBase();

	template <typename T>
	T& GetPixel(uint32 x, uint32 y)
	{
		return *((T*)((uint8*)m_Data + y * m_Stride + x * (m_BitsPerPixel / 8)));
	}

	uint8*                  GetDataEx();
	std::shared_ptr<IImage> ConvertAnyTo32Bit();

	// IImage
	uint32 GetWidth() const override;
	uint32 GetHeight() const override;
	uint32 GetBitsPerPixel() const override;
	uint32 GetStride() const override;
	bool IsTransperent() const override;
	const uint8* GetData() const override;
	void Resize(uint32 NewWidth, uint32 NewHeight) override;

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
class ZN_API CImageLoaderT
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
		return image->ConvertAnyTo32Bit();
	}
};
