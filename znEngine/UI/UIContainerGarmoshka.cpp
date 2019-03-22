#include "stdafx.h"

// General
#include "UIContainerGarmoshka.h"

CUIContainerGarmoshka::CUIContainerGarmoshka()
    : m_ActiveCategory(nullptr)
{
}

CUIContainerGarmoshka::~CUIContainerGarmoshka()
{
}

//
// CUIContainerGarmoshka
//
void CUIContainerGarmoshka::Initialize(vec2 Size)
{
    m_Background = std::make_shared<CUIColorNode>(Size);
    m_Background->SetParentInternal(weak_from_this());
    m_Background->SetColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void CUIContainerGarmoshka::CreateDefault()
{
    CreateCategory("First catogory")->CreateDefault();
    CreateCategory("Second catogory")->CreateDefault();
    CreateCategory("Third catogory")->CreateDefault();
    CreateCategory("Fourth catogory")->CreateDefault();
    CreateCategory("Fifth catogory")->CreateDefault();
}

std::shared_ptr<CUIContainerGarmoshkaCategory> CUIContainerGarmoshka::CreateCategory(const std::string& CategoryName)
{
    std::shared_ptr<CUIContainerGarmoshkaCategory> category = std::make_shared<CUIContainerGarmoshkaCategory>(std::dynamic_pointer_cast<CUIContainerGarmoshka>(shared_from_this()));
    category->SetParentInternal(weak_from_this());
    category->Initialize(m_Background->GetSize(), CategoryName);
    m_Categories.push_back(category);

    CalculateActiveCategory();

    return category;
}

void CUIContainerGarmoshka::OnCategoryHeaderClicked(std::shared_ptr<CUIContainerGarmoshkaCategory> Category)
{
    if (m_ActiveCategory == Category)
        return;

    m_ActiveCategory = Category;
    CalculateActiveCategory();
}



glm::vec2 CUIContainerGarmoshka::GetSize() const
{
    _ASSERT(m_Background != nullptr);
    return m_Background->GetSize();
}

//
// CUIBaseNode
//
std::vector<std::shared_ptr<CUIBaseNode>> CUIContainerGarmoshka::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Background)
        childs.push_back(m_Background);

    for (auto category : m_Categories)
        if (category)
            childs.push_back(category);

    return childs;
}



//
// Protected
//
void CUIContainerGarmoshka::CalculateActiveCategory()
{
    if (m_ActiveCategory == nullptr)
    {
        if (!m_Categories.empty())
        {
            std::shared_ptr<CUIContainerGarmoshkaCategory> category = *(m_Categories.begin());
            _ASSERT(category != nullptr);

            m_ActiveCategory = category;
        }
    }

    for (auto category : m_Categories)
    {
        category->SetActive(category == m_ActiveCategory);
    }

    CalculateCategoriesTranslate();
}

void CUIContainerGarmoshka::CalculateCategoriesTranslate()
{
    glm::vec2 increment = glm::vec2(0.0f, 0.0f);
    glm::vec2 eachOffset = vec2(0.0f, 2.0f);

    for (auto category : m_Categories)
    {
        category->SetTranslate(eachOffset + increment);
        increment += vec2(0.0f, eachOffset.y);
        increment += vec2(0.0f, category->GetSize().y);
    }
}
