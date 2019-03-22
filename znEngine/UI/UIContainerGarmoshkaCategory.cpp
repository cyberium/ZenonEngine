#include "stdafx.h"

// General
#include "UIContainerGarmoshkaCategory.h"

// Additional
#include "UIContainerGarmoshka.h"

#include "UIUtils.h"
#include "UIText.h"


CUIContainerGarmoshkaCategory::CUIContainerGarmoshkaCategory(std::weak_ptr<CUIContainerGarmoshka> ContainerGarmoshka)
    : m_ContainerGarmoshka(ContainerGarmoshka)
{
}

CUIContainerGarmoshkaCategory::~CUIContainerGarmoshkaCategory()
{
}



//
// CUIContainerGarmoshkaCategory
//
void CUIContainerGarmoshkaCategory::Initialize(glm::vec2 MaxSize, const std::string& CategoryName)
{
    base::Initialize(MaxSize);

    std::shared_ptr<CUIContainerGarmoshkaCategoryHeader> header = std::make_shared<CUIContainerGarmoshkaCategoryHeader>(m_ContainerGarmoshka);
    header->Initialize();
    header->SetText(CategoryName);

    SetHeader(header);
}

void CUIContainerGarmoshkaCategory::SetCategoryName(cstring Name)
{
    std::dynamic_pointer_cast<CUIContainerGarmoshkaCategoryHeader>(GetHeader())->SetText(Name);
}


