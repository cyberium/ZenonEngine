#include "stdafx.h"

// General
#include "SceneNodeFont3D.h"

CSceneNodeFont3D::CSceneNodeFont3D()
{
	m_Font = GetManager<IFontsManager>()->GetMainFont();
	m_Text = "<empty>";

	m_Material = std::make_shared<UI_Font_Material>();
	m_Material->SetWrapper(m_Material);
	m_Material->SetTexture(0, m_Font->GetTexture());

}

CSceneNodeFont3D::~CSceneNodeFont3D()
{
}



//
// CUITextNode
//
void CSceneNodeFont3D::SetFont(std::shared_ptr<CFontMesh> _font)
{
	m_Font = _font;
}

void CSceneNodeFont3D::SetText(const std::string& _text)
{
	m_Text = _text;
}

void CSceneNodeFont3D::SetColor(cvec4 _color)
{
	m_Material->SetColor(_color);
}



bool CSceneNodeFont3D::Accept(IVisitor& visitor)
{
	bool visitResult = visitor.Visit(*this);
	if (!visitResult)
		return false;

	// Mesh
	{
		m_Font->SetMaterial(m_Material);

		vec2 _offset = vec2(0.0f, 0.0f);

		for (uint32 i = 0; i < m_Text.length(); i++)
		{
			uint8 ch = m_Text.c_str()[i];
			m_Material->SetOffset(_offset);
			_offset.x += static_cast<float>(m_Font->GetCharWidth(ch));

			m_Font->Accept(visitor, 0, 0, (ch) * 6, 6);
		}
	}

	return visitResult;
}
