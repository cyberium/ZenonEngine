#include "stdafx.h"

// General
#include "UIContainerGarmoshkaCategoryHeader.h"

// Additional
#include "UIContainerGarmoshka.h"
#include "UIContainerGarmoshkaCategory.h"

#include "UIUtils.h"

namespace
{
    // Background
    const vec2   cDefaultBackgroundSize = vec2(240.0f, 20.0f);
    const vec4   cDefaultBackgroundColor = vec4(0.88f, 0.88f, 0.88f, 1.0f);

    // Text
    const char*  cDefaultText = "Category header";
    const uint32 cDefaultTextHeight = 12;
    const vec2   cDefaultTextOffset = vec2(3.0f, 3.0f);
    const vec4   cDefaultTextColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
}


CUIContainerGarmoshkaCategoryHeader::CUIContainerGarmoshkaCategoryHeader(std::weak_ptr<CUIContainerGarmoshka> ContainerGarmoshka)
    : m_ContainerGarmoshka(ContainerGarmoshka)
{
}

CUIContainerGarmoshkaCategoryHeader::~CUIContainerGarmoshkaCategoryHeader()
{
}



//
// CUIContainerGarmoshkaCategoryHeader
//
void CUIContainerGarmoshkaCategoryHeader::Initialize()
{
    m_Background = std::make_shared<CUIColorNode>(cDefaultBackgroundSize);
    m_Background->SetParentInternal(weak_from_this());
    m_Background->SetColor(cDefaultBackgroundColor);

    m_Text = std::make_shared<CUITextNode>();
    m_Text->SetParentInternal(weak_from_this());
    m_Text->SetText(cDefaultText);
    m_Text->SetTextColor(cDefaultTextColor);
    m_Text->SetOffset(cDefaultTextOffset);
    m_Text->SetFont(GetManager<IFontsManager>()->Add("Fonts\\JustBreatheBd.otf", cDefaultTextHeight));
}

void CUIContainerGarmoshkaCategoryHeader::SetText(cstring Text)
{
    _ASSERT(m_Background != nullptr);
    _ASSERT(m_Text != nullptr);

    m_Text->SetText(Text);
}



//
// CUIBaseNode
//
glm::vec2 CUIContainerGarmoshkaCategoryHeader::GetSize() const
{
    return m_Background->GetSize();
}

std::vector<std::shared_ptr<CUIBaseNode>> CUIContainerGarmoshkaCategoryHeader::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Background)
        childs.push_back(m_Background);

    if (m_Text)
        childs.push_back(m_Text);

    return childs;
}

// Input events

bool CUIContainerGarmoshkaCategoryHeader::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    std::shared_ptr<CUIBaseNode> parent = m_pParentNode.lock();
    _ASSERT(parent != nullptr);

    std::shared_ptr<CUIContainerGarmoshkaCategory> parentAsContainerGarmoshka = std::dynamic_pointer_cast<CUIContainerGarmoshkaCategory>(parent);
    _ASSERT(parentAsContainerGarmoshka != nullptr);

    std::shared_ptr<CUIContainerGarmoshka> containerGarmoshka = m_ContainerGarmoshka.lock();
    _ASSERT(containerGarmoshka != nullptr);

    containerGarmoshka->OnCategoryHeaderClicked(parentAsContainerGarmoshka);

    return true;
}

void CUIContainerGarmoshkaCategoryHeader::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
}
