#pragma once

// Common UI
#include "UIBaseNode.h"
#include "UIContainerListWithActivation.h"

// ContainerGarmoshka
#include "UIContainerGarmoshkaCategoryHeader.h"

// FORWARD BEGIN
class CUIContainerGarmoshka;
// FORWARD END

class CUIContainerGarmoshkaCategory : public CUIContainerListWithActivation
{
    typedef CUIContainerListWithActivation base;
public:
    CUIContainerGarmoshkaCategory(std::weak_ptr<CUIContainerGarmoshka> ContainerGarmoshka);
    virtual ~CUIContainerGarmoshkaCategory();

    // CUIContainerGarmoshkaCategory
    void Initialize(glm::vec2 MaxSize, const std::string& CategoryName);

    void SetCategoryName(cstring Name);

private:
    // Weak refs
    std::weak_ptr<CUIContainerGarmoshka>                    m_ContainerGarmoshka;
};