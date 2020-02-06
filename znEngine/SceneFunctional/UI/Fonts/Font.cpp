#include "stdafx.h"

// General
#include "Font.h"

CFont::CFont(std::shared_ptr<ITexture> _texture, std::shared_ptr<IGeometry> _fontGeometry, std::vector<uint32> _widthArray, uint32 _height)
	: m_Texture(_texture)
	, m_Geometry(_fontGeometry)
	, m_WidthArray(_widthArray)
	, m_Height(_height)
{
}

CFont::~CFont()
{
	Log::Info("Font deleted!!!");
}


std::shared_ptr<ITexture> CFont::GetTexture() const
{
	return m_Texture;
}

std::shared_ptr<IGeometry> CFont::GetGeometry() const
{
	return m_Geometry;
}

uint32 CFont::GetCharWidth(char _char) const
{
	_ASSERT(_char < m_WidthArray.size());
	return m_WidthArray[_char];
}

uint32 CFont::GetStringWidth(const std::string& _string) const
{
	uint32 width = 0;

	for (uint32 i = 0; i < _string.length(); i++)
	{
		uint8 ch = _string.c_str()[i];
		width += m_WidthArray[ch];
	}

	return width;
}

uint32 CFont::GetHeight() const
{
	return m_Height;
}
