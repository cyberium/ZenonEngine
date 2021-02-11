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

	SetFont(GetBaseManager().GetManager<IznFontsManager>()->GetMainFont());

	/*m_TextProperty = MakeShared(CProperty<std::string>, "Text", "descr", cDefaultText);
	m_TextProperty->SetValueChangedCallback([this](const std::string& NewValue) {
		SetSize(GetFont()->GetSize(NewValue));
	});
	GetProperties()->AddProperty(m_TextProperty);

	m_TextAlignHorizontalProperty = MakeShared(CProperty<ETextAlignHorizontal>, "TextAlignHorizontal", "Text align horizontal.", ETextAlignHorizontal::Left);
	GetProperties()->AddProperty(m_TextAlignHorizontalProperty);

	m_TextAlignVerticalProperty = MakeShared(CProperty<ETextAlignVertical>, "TextAlignVertical", "Text align vertical.", ETextAlignVertical::Top);
	GetProperties()->AddProperty(m_TextAlignVerticalProperty);

	m_ColorProperty = MakeShared(CProperty<ColorRGBA>, "Color", "Color of text", ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	GetProperties()->AddProperty(m_ColorProperty);*/
}



//
// IUIControlText
//
void CUIControlText::SetFont(std::shared_ptr<IznFont> Font)
{
	m_Font = Font;
	m_FontModel = MakeShared(CFontModel, GetRenderDevice(), m_Font);

	for (const auto& prop : m_FontModel->GetProperties()->GetProperties())
		GetProperties()->AddProperty(prop.second);
}

std::shared_ptr<IznFont> CUIControlText::GetFont() const
{
    return m_Font;
}

void CUIControlText::SetText(std::string Text)
{
	m_FontModel->SetText(Text);
}

std::string CUIControlText::GetText() const
{
	return m_FontModel->GetText();
}

void CUIControlText::SetTextAlignHorizontal(ETextAlignHorizontal TextAlignHorizontal)
{
	m_FontModel->SetTextAlignHorizontal(TextAlignHorizontal);
}

ETextAlignHorizontal CUIControlText::GetTextAlignHorizontal() const
{
	return m_FontModel->GetTextAlignHorizontal();
}

void CUIControlText::SetTextAlignVertical(ETextAlignVertical TextAlignVertical)
{
	m_FontModel->SetTextAlignVertical(TextAlignVertical);
}

ETextAlignVertical CUIControlText::GetTextAlignVertical() const
{
	return m_FontModel->GetTextAlignVertical();
}

void CUIControlText::SetColor(ColorRGBA Color)
{
	m_FontModel->SetColor(Color);
}

ColorRGBA CUIControlText::GetColor() const
{
	return m_FontModel->GetColor();
}



//
// Protected
//
EVisitResult CUIControlText::AcceptContent(IVisitor * visitor)
{
	return visitor->Visit(m_FontModel);
}
