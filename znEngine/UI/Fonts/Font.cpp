#include <stdafx.h>

// Include
#include "MeshWrapper.h"

// General
#include "Font.h"

CFontMesh::CFontMesh(std::shared_ptr<Texture> _texture, std::shared_ptr<IMesh> _fontGeometry, std::vector<uint32> _widthArray, uint32 _height) 
	: MeshWrapper(SceneNodeTypes::SN_TYPE_FONT, _fontGeometry)
	, m_Texture(_texture)
	, m_WidthArray(_widthArray)
	, m_Height(_height)
{
}

CFontMesh::~CFontMesh()
{
	Log::Info("Fonts deleted!!!");
}


std::shared_ptr<Texture> CFontMesh::GetTexture() const
{
	return m_Texture;
}

std::shared_ptr<IMesh> CFontMesh::GetMesh() const
{
	return m_Geometry;
}

uint32 CFontMesh::GetCharWidth(char _char) const
{
	assert1(_char < m_WidthArray.size());
	return m_WidthArray[_char];
}

uint32 CFontMesh::GetStringWidth(cstring _string) const
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
