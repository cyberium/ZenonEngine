#pragma once

class ZN_API CFont
	: public IznFont
{
public:
	CFont(std::shared_ptr<ITexture> Texture, std::shared_ptr<IGeometry> FontGeometry, std::vector<uint32> WidthArray, uint32 Height);
	virtual ~CFont();

	// IznFont
	std::shared_ptr<ITexture>  GetTexture() const override;
	std::shared_ptr<IGeometry> GetGeometry() const override;
	uint32                     GetWidth(char Char) const override;
	uint32                     GetWidth(const std::string& String) const override;
	uint32                     GetHeight() const override;
	 
public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 256;

private:
	std::shared_ptr<ITexture>	m_Texture;
	std::shared_ptr<IGeometry>	m_Geometry;
	std::vector<uint32>	        m_WidthArray;
	uint32			            m_Height;
};

