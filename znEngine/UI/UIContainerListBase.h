#pragma once

// Common UI
#include "UIBaseNode.h"

class CUIContainerListBase : public CUIBaseNode
{
public:
    CUIContainerListBase();
    virtual ~CUIContainerListBase();

    // CUIContainerListBase
    void Initialize(glm::vec2 MaxSize);
    void CreateDefault();

    virtual void AddChild(std::shared_ptr<CUIBaseNode> Child, bool IsNeedCalculate = true);
    void IncStartNodeIndex();
    void DecStartNodeIndex();

    // CUIBaseNode
    virtual glm::vec2 GetSize() const override;
    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override;

protected:
    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetNodesUsingMaxSize() const;
    void CalculateChildsTranslate(glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f));

private:
    size_t                                    m_StartNodeIndex; // used for MaxSize
    glm::vec2                                 m_MaxSize;

    // Const params override
    glm::vec2                                 m_c_ChildsOffset;
    float                                     m_c_ChildsVerticalPadding;

    std::vector<std::shared_ptr<CUIBaseNode>> m_Nodes;
};