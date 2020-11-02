#pragma once

#ifndef ONLY_SIMPLE_UI

// Common UI
#include "SceneFunctional/UI/CUIControl.h"
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

    void SetHeader(std::shared_ptr<CUIControl> Header);
    std::shared_ptr<CUIControl> GetHeader() const;

    void AddChild(std::shared_ptr<CUIControl> Child, bool IsNeedCalculate = true) override;

    void SetActive(bool Active);
    bool IsActive() const;

    // CUIControl
    virtual std::vector<std::shared_ptr<CUIControl>> GetChilds() const override final;

protected:
    virtual std::vector<std::shared_ptr<CUIControl>> GetNodesUsingMaxSize() const override;

private:
    std::shared_ptr<CUIControl>    m_Header;
    bool                            m_IsActive;
};

#endif
