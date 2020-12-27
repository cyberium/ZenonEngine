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

	// IImage
	uint32 GetWidth() const override;
	uint32 GetHeight() const override;
	uint32 GetBitsPerPixel() const override;
	uint32 GetStride() const override;
	bool IsTransperent() const override;

	const uint8* GetData() const override;
	uint8* GetDataEx() override;

	const uint8* GetLine(uint32 Line) const override;
	uint8* GetLineEx(uint32 Line) override;

	size_t GetDataSize() const override;
	void Resize(uint32 NewWidth, uint32 NewHeight) override;
	
	std::shared_ptr<IImage> ConvertAnyTo32Bit() override;
	std::shared_ptr<IByteBuffer> SaveToMemory() override;


protected:
	uint32 m_Width;
	uint32 m_Height;
	uint32 m_BitsPerPixel;
	uint32 m_Stride;
	bool m_IsTransperent;
	std::vector<uint8> m_Data;
};
