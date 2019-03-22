#pragma once

// Common UI
#include "UIBaseNode.h"

// ContainerGarmoshka
#include "UIContainerGarmoshkaCategory.h"

class CUIContainerGarmoshka : public CUIBaseNode
{
public:
    CUIContainerGarmoshka();
    virtual ~CUIContainerGarmoshka();

    //
    // CUIContainerGarmoshka
    //
    void Initialize(vec2 Size);
    void CreateDefault();

    std::shared_ptr<CUIContainerGarmoshkaCategory> CreateCategory(const std::string& CategoryName);

    // Open & close some categories
    void OnCategoryHeaderClicked(std::shared_ptr<CUIContainerGarmoshkaCategory> Category);


    //
    // CUIBaseNode
    //
    glm::vec2 GetSize() const override final;
    std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override;

protected:
    void CalculateActiveCategory();
    void CalculateCategoriesTranslate();

private:
    std::shared_ptr<CUIColorNode>                               m_Background;


    std::vector<std::shared_ptr<CUIContainerGarmoshkaCategory>> m_Categories;
    std::shared_ptr<CUIContainerGarmoshkaCategory>              m_ActiveCategory;
};