#include "stdafx.h"

// General
#include "Font.h"

CFont::CFont(std::shared_ptr<ITexture> Texture, std::shared_ptr<IGeometry> FontGeometry, std::vector<int32> WidthArray, uint32 Height)
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

glm::ivec2 CFont::GetCharSize(char Char) const
{
	unsigned char uChar = static_cast<unsigned char>(Char);
	if (uChar >= m_WidthArray.size())
		throw CException("Character '%c' out of bounds.", Char);
	return glm::ivec2(m_WidthArray[uChar], m_Height);
}

glm::ivec2 CFont::GetLineSize(const std::string& Line) const
{
	glm::ivec2 lineSize = glm::ivec2(0.0f);

	for (size_t i = 0; i < Line.length(); i++)
	{
		const auto ch = static_cast<uint8>(Line.c_str()[i]);
		if (ch == '\n')
			throw CException("Line must not contains new-line symbols.");

		const auto charSize = GetCharSize(ch);
		lineSize.x += charSize.x;
		lineSize.y = charSize.y;
	}

	return lineSize;
}

glm::ivec2 CFont::GetTextSize(const std::string& Text) const
{
	return GetTextSize(Utils::SplitTextToLines(Text));
}

glm::ivec2 CFont::GetTextSize(const std::vector<std::string>& Lines) const
{
	glm::ivec2 textSize = glm::ivec2(0.0f);
	for (const auto& line : Lines)
	{
		const auto lineSize = GetLineSize(line);
		textSize.x = glm::max(textSize.x, lineSize.x);
		textSize.y += lineSize.y;
	}

	return textSize;
}


