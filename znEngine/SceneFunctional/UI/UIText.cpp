#include "stdafx.h"

// General
#include "UIText.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const vec2  cDefaultOffset = vec2(0.0f, 0.0f);
	const vec4  cDefaultColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

CUITextNode::CUITextNode()
{}

CUITextNode::~CUITextNode()
{}



void CUITextNode::Initialize()
{
	m_TextProperty = std::make_shared<CProperty<std::string>>();
	m_TextProperty->SetName("Text");
	m_TextProperty->Set(cDefaultText);
	GetProperties()->AddProperty(m_TextProperty);

	m_OffsetProperty = std::make_shared<CProperty<glm::vec2>>();
	m_OffsetProperty->SetName("Offset");
	m_OffsetProperty->Set(cDefaultOffset);
	GetProperties()->AddProperty(m_OffsetProperty);

	m_Font = GetBaseManager()->GetManager<IFontsManager>()->GetMainFont();

	m_Material = std::make_shared<UI_Font_Material>(GetBaseManager()->GetApplication().GetRenderDevice());
	m_Material->SetWrapper(m_Material.get());
	m_Material->SetTexture(0, m_Font->GetTexture());
	m_Material->SetColor(cDefaultColor);
}



//
// CUITextNode
//
void CUITextNode::SetFont(std::shared_ptr<CFontMesh> _font)
{
    _ASSERT(_font != nullptr);

    m_Font = _font;
    m_Material->SetTexture(0, m_Font->GetTexture());
}

std::shared_ptr<CFontMesh> CUITextNode::GetFont() const
{
    return m_Font;
}

void CUITextNode::SetTextColor(cvec4 _color)
{
    m_Material->SetColor(_color);
}

glm::vec2 CUITextNode::GetTextSize() const
{
    float width = m_Font->GetStringWidth(m_TextProperty->Get());
    float height = m_Font->GetHeight();

    return glm::vec2(width, height);
}



//
// CUIBaseNode
//
glm::vec2 CUITextNode::GetSize()
{
    return GetTextSize() + 2.0f * m_OffsetProperty->Get();
}

bool CUITextNode::AcceptMesh(IVisitor* visitor)
{
	const std::string& _text = m_TextProperty->Get();
	vec2 _offset = m_OffsetProperty->Get();

	m_Font->SetMaterial(m_Material);

	for (uint32 i = 0; i < _text.length(); i++)
	{
		uint8 ch = _text.c_str()[i];
		m_Material->SetOffset(_offset);
		_offset.x += static_cast<float>(m_Font->GetCharWidth(ch)) + 0.01f;

		SGeometryPartParams geometryPartParams;
		geometryPartParams.VertexStartLocation = (ch) * 6;
		geometryPartParams.VertexCnt = 6;
		m_Font->Accept(visitor, geometryPartParams);
	}

	return true;
}
