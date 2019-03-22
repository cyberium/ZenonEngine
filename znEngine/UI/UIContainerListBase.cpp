#include "stdafx.h"

// General
#include "UIContainerListBase.h"

// Additional
#include "UIUtils.h"
#include "UIText.h"

namespace
{
    // Icon
    const vec2  cChildsOffset = vec2(5.0f, 5.0f);
    const float cChildsVerticalPadding = 2.0f;
}

CUIContainerListBase::CUIContainerListBase()
    : m_StartNodeIndex(0)
    // consts override
    , m_c_ChildsOffset(cChildsOffset)
    , m_c_ChildsVerticalPadding(cChildsVerticalPadding)
{
}

CUIContainerListBase::~CUIContainerListBase()
{
}



//
// CUIContainerListBase
//
void CUIContainerListBase::Initialize(glm::vec2 MaxSize)
{
    m_MaxSize = MaxSize;
}

void CUIContainerListBase::CreateDefault()
{
    const glm::vec2 textOffset = glm::vec2(5.0f, 5.0f);

    std::shared_ptr<CUITextNode> ch1 = std::make_shared<CUITextNode>();
    ch1->SetText("Some child #1");
    ch1->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch1->SetOffset(textOffset);
    AddChild(ch1);

    std::shared_ptr<CUITextNode> ch2 = std::make_shared<CUITextNode>();
    ch2->SetText("Some child #2");
    ch2->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch2->SetOffset(textOffset);
    AddChild(ch2);

    std::shared_ptr<CUITextNode> ch3 = std::make_shared<CUITextNode>();
    ch3->SetText("Some child #3");
    ch3->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch3->SetOffset(textOffset);
    AddChild(ch3);

    std::shared_ptr<CUITextNode> ch4 = std::make_shared<CUITextNode>();
    ch4->SetText("Some child #4");
    ch4->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch4->SetOffset(textOffset);
    AddChild(ch4);

    std::shared_ptr<CUITextNode> ch5 = std::make_shared<CUITextNode>();
    ch5->SetText("Some child #5");
    ch5->SetTextColor(vec4(0.0f, 0.0f, 0.0f, 1.0f));
    ch5->SetOffset(textOffset);
    AddChild(ch5);
}

void CUIContainerListBase::AddChild(std::shared_ptr<CUIBaseNode> Child, bool IsNeedCalculate)
{
    _ASSERT(Child != nullptr);

    Child->SetParentInternal(weak_from_this());
    m_Nodes.push_back(Child);

    if (IsNeedCalculate)
        CalculateChildsTranslate();
}

void CUIContainerListBase::IncStartNodeIndex()
{
    m_StartNodeIndex++;

    if (m_StartNodeIndex > m_Nodes.size())
        m_StartNodeIndex = m_Nodes.size();
}

void CUIContainerListBase::DecStartNodeIndex()
{
    m_StartNodeIndex--;

    if (m_StartNodeIndex < 0)
        m_StartNodeIndex = 0;
}



//
// CUIBaseNode
//
glm::vec2 CUIContainerListBase::GetSize() const
{
    return owUIUtils::CalculateChildsVerticalSize(GetChilds(), m_c_ChildsVerticalPadding);
}

std::vector<std::shared_ptr<CUIBaseNode>> CUIContainerListBase::GetChilds() const
{
    return GetNodesUsingMaxSize();
}



//
// Protected
//
std::vector<std::shared_ptr<CUIBaseNode>> CUIContainerListBase::GetNodesUsingMaxSize() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;
    float currentYSize = 0.0f;

    for (size_t i = m_StartNodeIndex; i < m_Nodes.size(); i++)
    {
        if (i >= m_Nodes.size())
            continue;

        glm::vec2 childSize = m_Nodes[i]->GetSize();

        if (currentYSize + childSize.y <= m_MaxSize.y)
        {
            childs.push_back(m_Nodes[i]);
            currentYSize += childSize.y;
        }
    }

    return childs;
}

void CUIContainerListBase::CalculateChildsTranslate(glm::vec2 StartPoint)
{
    glm::vec2 currentTranslate = StartPoint;

    for (auto node : m_Nodes)
    {
        node->SetTranslate(m_c_ChildsOffset + currentTranslate);

        currentTranslate += vec2(0.0f, node->GetSize().y);
        currentTranslate += vec2(0.0f, m_c_ChildsVerticalPadding);
    }
}
