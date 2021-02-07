#include "stdafx.h"

// General
#include "Font.h"

CFont::CFont(std::shared_ptr<ITexture> Texture, std::shared_ptr<IGeometry> FontGeometry, std::vector<uint32> WidthArray, uint32 Height)
	: m_Texture(Texture)
	, m_Geometry(FontGeometry)
	, m_WidthArray(WidthArray)
	, m_Height(Height)
{
}

CFont::~CFont()
{

}



//
// IznFont
//
std::shared_ptr<ITexture> CFont::GetTexture() const
{
	return m_Texture;
}

std::shared_ptr<IGeometry> CFont::GetGeometry() const
{
	return m_Geometry;
}

glm::vec2 CFont::GetSize(const std::string& String) const
{
	return glm::vec2(GetWidth(String), GetHeight());
}

uint32 CFont::GetWidth(char Char) const
{
	unsigned char uChar = static_cast<unsigned char>(Char);
	if (uChar >= m_WidthArray.size())
		throw CException("Character '%c' out of bounds.", Char);
	return m_WidthArray[uChar];
}

uint32 CFont::GetWidth(const std::string& String) const
{
	uint32 width = 0;
	std::for_each(String.begin(), String.end(), [this, &width](const char& Ch) {
		width += m_WidthArray[static_cast<uint8>(Ch)];
	});
	return width;
}

uint32 CFont::GetHeight() const
{
	return m_Height;
}
