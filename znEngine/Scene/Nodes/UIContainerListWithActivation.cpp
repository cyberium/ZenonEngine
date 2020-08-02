#include "stdafx.h"

#ifndef ONLY_SIMPLE_UI

// General
#include "UIContainerListWithActivation.h"

#include "UIUtils.h"
#include "UIText.h"

CUIContainerListWithActivation::CUIContainerListWithActivation()
    : m_IsActive(false)
{
}

CUIContainerListWithActivation::~CUIContainerListWithActivation()
{
}



//
// CUIContainerListWithActivation
//
void CUIContainerListWithActivation::Initialize(glm::vec2 MaxSize)
{
    base::Initialize(MaxSize);

}

void CUIContainerListWithActivation::SetHeader(std::shared_ptr<SceneNodeUI> Header)
{
    Header->SetParentInternal(weak_from_this());
    m_Header = Header;
}

std::shared_ptr<SceneNodeUI> CUIContainerListWithActivation::GetHeader() const
{
    return m_Header;
}

void CUIContainerListWithActivation::AddChild(std::shared_ptr<SceneNodeUI> Child, bool /*IsNeedCalculate*/)
{
    base::AddChild(Child, false);

    glm::vec2 startPosition = glm::vec2(0.0f, 0.0f);
    if (m_Header)
        startPosition += m_Header->GetTranslation() + vec2(0.0, m_Header->GetSize().y);

    CalculateChildsTranslate(startPosition);
}

void CUIContainerListWithActivation::SetActive(bool Active)
{
    m_IsActive = Active;
}

bool CUIContainerListWithActivation::IsActive() const
{
    return m_IsActive;
}



//
// SceneNodeUI
//
std::vector<std::shared_ptr<SceneNodeUI>> CUIContainerListWithActivation::GetChilds() const
{
    std::vector<std::shared_ptr<SceneNodeUI>> childs;

    if (m_Header)
        childs.push_back(m_Header);

    if (m_IsActive)
        for (auto child : base::GetNodesUsingMaxSize())
            childs.push_back(child);

    return childs;
}



//
// Protected
//
std::vector<std::shared_ptr<SceneNodeUI>> CUIContainerListWithActivation::GetNodesUsingMaxSize() const
{
    if (m_IsActive)
        return base::GetNodesUsingMaxSize();

    return std::vector<std::shared_ptr<SceneNodeUI>>();
}

#endif
