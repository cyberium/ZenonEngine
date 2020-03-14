#pragma once

#ifndef ONLY_SIMPLE_UI

// Common UI
#include "SceneFunctional/UI/SceneNodeUI.h"
#include "UIContainerListBase.h"

// ContainerGarmoshka
#include "UIContainerGarmoshkaCategoryHeader.h"

// FORWARD BEGIN
class CUIContainerGarmoshka;
// FORWARD END

class CUIContainerListWithActivation : public CUIContainerListBase
{
    typedef CUIContainerListBase base;
public:
    CUIContainerListWithActivation();
    virtual ~CUIContainerListWithActivation();

    // CUIContainerListWithActivation
    void Initialize(glm::vec2 MaxSize);

    void SetHeader(std::shared_ptr<SceneNodeUI> Header);
    std::shared_ptr<SceneNodeUI> GetHeader() const;

    void AddChild(std::shared_ptr<SceneNodeUI> Child, bool IsNeedCalculate = true) override;

    void SetActive(bool Active);
    bool IsActive() const;

    // SceneNodeUI
    virtual std::vector<std::shared_ptr<SceneNodeUI>> GetChilds() const override final;

protected:
    virtual std::vector<std::shared_ptr<SceneNodeUI>> GetNodesUsingMaxSize() const override;

private:
    std::shared_ptr<SceneNodeUI>    m_Header;
    bool                            m_IsActive;
};

#endif
