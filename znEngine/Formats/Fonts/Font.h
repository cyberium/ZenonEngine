#pragma once

class ZN_API CFont
	: public IznFont
{
public:
	CFont(std::shared_ptr<ITexture> Texture, std::shared_ptr<IGeometry> FontGeometry, std::vector<int32> WidthArray, uint32 Height);
	virtual ~CFont();

	// IznFont
	std::shared_ptr<ITexture>  GetTexture() const override;
	std::shared_ptr<IGeometry> GetGeometry() const override;
	glm::ivec2                 GetCharSize(char Char) const override;
	glm::ivec2                 GetLineSize(const std::string& Line) const override;
	glm::ivec2                 GetTextSize(const std::string& Text) const override;
	glm::ivec2				   GetTextSize(const std::vector<std::string>& Lines) const override;
	 
public:
	static const uint32 SPACE = 32;
	static const uint32 NUM_CHARS = 256;

private:
	std::shared_ptr<ITexture>	m_Texture;
	std::shared_ptr<IGeometry>	m_Geometry;
	std::vector<int32>	        m_WidthArray;
	uint32			            m_Height;
};

