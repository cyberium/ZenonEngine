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

	m_TextProperty = MakeShared(CProperty<std::string>);
	m_TextProperty->SetName("Text");
	m_TextProperty->Set(cDefaultText);
	GetProperties()->AddProperty(m_TextProperty);

	m_OffsetProperty = MakeShared(CProperty<glm::vec2>);
	m_OffsetProperty->SetName("Offset");
	m_OffsetProperty->Set(cDefaultOffset);
	GetProperties()->AddProperty(m_OffsetProperty);

	m_Font = GetBaseManager().GetManager<IznFontsManager>()->GetMainFont();

	m_Material = MakeShared(UI_Font_Material, GetBaseManager().GetApplication().GetRenderDevice());
	m_Material->SetTexture(0, m_Font->GetTexture());
	m_Material->SetColor(cDefaultColor);
}



//
// CUIControlConsole
//

std::shared_ptr<IznFont> CUIControlConsole::GetFont() const
{
    return m_Font;
}

const std::shared_ptr<UI_Font_Material>& CUIControlConsole::GetMaterial() const
{
	return m_Material;
}

glm::vec2 CUIControlConsole::GetTextSize() const
{
    float width = m_Font->GetWidth(m_TextProperty->Get());
    float height = m_Font->GetHeight();

    return glm::vec2(width, height);
}

const std::string CUIControlConsole::GetText() const
{
	return m_TextProperty->Get();
}

const glm::vec2 CUIControlConsole::GetOffset() const
{
	return m_OffsetProperty->Get();
}



//
// CUIControl
//
glm::vec2 CUIControlConsole::GetSize() const
{
    return GetTextSize() + 2.0f * m_OffsetProperty->Get();
}
