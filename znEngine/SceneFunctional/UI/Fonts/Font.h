#pragma once

class ZN_API CFont
{
public:
	CFont(std::shared_ptr<ITexture> _texture, std::shared_ptr<IGeometry> _fontGeometry, std::vector<uint32> _widthArray, uint32 _height);
	virtual ~CFont();

	// Getters
	std::shared_ptr<ITexture> GetTexture() const;
	std::shared_ptr<IGeometry>GetGeometry() const;
	uint32                    GetCharWidth(char _char) const;
	uint32                    GetStringWidth(const std::string& _string) const;
	uint32                    GetHeight() const;
	 
public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 256;

private:
	std::shared_ptr<ITexture>	m_Texture;
	std::shared_ptr<IGeometry>	m_Geometry;
	std::vector<uint32>	        m_WidthArray;
	uint32			            m_Height;
};

