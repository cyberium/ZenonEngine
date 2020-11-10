#include "stdafx.h"

// General
#include "UIControlConsole.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const glm::vec2  cDefaultOffset = glm::vec2(0.0f, 0.0f);
	const glm::vec4  cDefaultColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUIControlConsole::CUIControlConsole(IScene& Scene)
	: CUIControl(Scene)
{}

CUIControlConsole::~CUIControlConsole()
{}



void CUIControlConsole::Initialize()
{
	__super::Initialize();

	m_Font = GetBaseManager().GetManager<IznFontsManager>()->GetMainFont();

	m_TextProperty = MakeShared(CProperty<std::string>);
	m_TextProperty->SetName("Text");
	m_TextProperty->Set(cDefaultText);
	GetProperties()->AddProperty(m_TextProperty);

	m_OffsetProperty = MakeShared(CProperty<glm::vec2>);
	m_OffsetProperty->SetName("Offset");
	m_OffsetProperty->Set(cDefaultOffset);
	GetProperties()->AddProperty(m_OffsetProperty);

	m_ColorProperty = MakeShared(CProperty<glm::vec4>);
	m_ColorProperty->SetName("Color");
	m_ColorProperty->Set(cDefaultColor);
	GetProperties()->AddProperty(m_ColorProperty);
}



//
// CUIControlConsole
//

std::shared_ptr<IznFont> CUIControlConsole::GetFont() const
{
    return m_Font;
}

std::string CUIControlConsole::GetText() const
{
	return m_TextProperty->Get();
}

glm::vec2 CUIControlConsole::GetOffset() const
{
	return m_OffsetProperty->Get();
}

glm::vec4 CUIControlConsole::GetColor() const
{
	return m_ColorProperty->Get();
}



//
// CUIControl
//
glm::vec2 CUIControlConsole::GetSize() const
{
	float width = m_Font->GetWidth(GetText());
	float height = m_Font->GetHeight();

    return glm::vec2(width, height) + 2.0f * GetOffset();
}
