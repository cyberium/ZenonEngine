#pragma once

#ifndef ONLY_SIMPLE_UI

#include "SceneFunctional/UI/SceneNodeUI.h"

class CUIContainerListBase : public SceneNodeUI
{
public:
    CUIContainerListBase();
    virtual ~CUIContainerListBase();

    // CUIContainerListBase
    void Initialize(glm::vec2 MaxSize);
    void CreateDefault();

    virtual void AddChild(std::shared_ptr<SceneNodeUI> Child, bool IsNeedCalculate = true);
    void IncStartNodeIndex();
    void DecStartNodeIndex();

    // SceneNodeUI
    virtual glm::vec2 GetSize() override;
    virtual std::vector<std::shared_ptr<SceneNodeUI>> GetChilds() override;

protected:
    virtual std::vector<std::shared_ptr<SceneNodeUI>> GetNodesUsingMaxSize() const;
    void CalculateChildsTranslate(glm::vec2 StartPoint = glm::vec2(0.0f, 0.0f));

private:
    size_t                                    m_StartNodeIndex; // used for MaxSize
    glm::vec2                                 m_MaxSize;

    // Const params override
    glm::vec2                                 m_c_ChildsOffset;
    float                                     m_c_ChildsVerticalPadding;

    std::vector<std::shared_ptr<SceneNodeUI>> m_Nodes;
};

#endif
