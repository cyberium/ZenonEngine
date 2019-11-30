#include <stdafx.h>

// Include
#include "MeshWrapper.h"

// General
#include "Font.h"

CFontMesh::CFontMesh(std::shared_ptr<ITexture> _texture, std::shared_ptr<IMesh> _fontGeometry, std::vector<uint32> _widthArray, uint32 _height) 
	: MeshWrapper(_fontGeometry)
	, m_Texture(_texture)
	, m_WidthArray(_widthArray)
	, m_Height(_height)
{
}

CFontMesh::~CFontMesh()
{
	Log::Info("Fonts deleted!!!");
}


std::shared_ptr<ITexture> CFontMesh::GetTexture() const
{
	return m_Texture;
}

std::shared_ptr<IMesh> CFontMesh::GetMesh() const
{
	return m_Geometry;
}

uint32 CFontMesh::GetCharWidth(char _char) const
{
	_ASSERT(_char < m_WidthArray.size());
	return m_WidthArray[_char];
}

uint32 CFontMesh::GetStringWidth(const std::string& _string) const
{
	uint32 width = 0;

	for (uint32 i = 0; i < _string.length(); i++)
	{
		uint8 ch = _string.c_str()[i];
		width += m_WidthArray[ch];
	}

	return width;
}

uint32 CFontMesh::GetHeight() const
{
	return m_Height;
}
