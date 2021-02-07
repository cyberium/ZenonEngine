#include "stdafx.h"

// General
#include "UIControlText.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const glm::vec4 cDefaultColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUIControlText::CUIControlText(IScene& Scene)
	: CUIControl(Scene)
{}

CUIControlText::~CUIControlText()
{}



void CUIControlText::Initialize()
{
	__super::Initialize();

	m_Font = GetBaseManager().GetManager<IznFontsManager>()->GetMainFont();

	m_TextProperty = MakeShared(CProperty<std::string>, "Text", "descr", cDefaultText);
	m_TextProperty->SetValueChangedCallback([this](const std::string& NewValue) {
		SetSize(GetFont()->GetSize(NewValue));
	});
	GetProperties()->AddProperty(m_TextProperty);

	m_TextAlignHorizontalProperty = MakeShared(CProperty<ETextAlignHorizontal>, "TextAlignHorizontal", "Text align horizontal.", ETextAlignHorizontal::Left);
	GetProperties()->AddProperty(m_TextAlignHorizontalProperty);

	m_TextAlignVerticalProperty = MakeShared(CProperty<ETextAlignVertical>, "TextAlignVertical", "Text align vertical.", ETextAlignVertical::Top);
	GetProperties()->AddProperty(m_TextAlignVerticalProperty);

	//m_OffsetProperty = MakeShared(CProperty<glm::vec2>, "Offset", "Offset of first string character relatieve to local transform.", cDefaultOffset);
	//GetProperties()->AddProperty(m_OffsetProperty);

	m_ColorProperty = MakeShared(CProperty<ColorRGBA>, "Color", "Color of text", ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	GetProperties()->AddProperty(m_ColorProperty);
}



//
// IUIControlText
//
void CUIControlText::SetFont(std::shared_ptr<IznFont> Font)
{
	m_Font = Font;
}

std::shared_ptr<IznFont> CUIControlText::GetFont() const
{
    return m_Font;
}

void CUIControlText::SetText(std::string Text)
{
	m_TextProperty->Set(Text);
}

std::string CUIControlText::GetText() const
{
	return m_TextProperty->Get();
}

void CUIControlText::SetTextAlignHorizontal(ETextAlignHorizontal TextAlignHorizontal)
{
	m_TextAlignHorizontalProperty->Set(TextAlignHorizontal);
}

ETextAlignHorizontal CUIControlText::GetTextAlignHorizontal() const
{
	return m_TextAlignHorizontalProperty->Get();
}

void CUIControlText::SetTextAlignVertical(ETextAlignVertical TextAlignVertical)
{
	m_TextAlignVerticalProperty->Set(TextAlignVertical);
}

ETextAlignVertical CUIControlText::GetTextAlignVertical() const
{
	return m_TextAlignVerticalProperty->Get();
}


/*void CUIControlText::SetOffset(glm::vec2 Offset)
{
	m_OffsetProperty->Set(Offset);
}

glm::vec2 CUIControlText::GetOffset() const
{
	return m_OffsetProperty->Get();
}*/

void CUIControlText::SetColor(ColorRGBA Color)
{
	m_ColorProperty->Set(Color);
}

ColorRGBA CUIControlText::GetColor() const
{
	return m_ColorProperty->Get();
}
