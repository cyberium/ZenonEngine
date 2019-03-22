#pragma once

// Common UI
#include "UIBaseNode.h"
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

    void SetHeader(std::shared_ptr<CUIBaseNode> Header);
    std::shared_ptr<CUIBaseNode> GetHeader() const;

    void AddChild(std::shared_ptr<CUIBaseNode> Child, bool IsNeedCalculate = true) override;

    void SetActive(bool Active);
    bool IsActive() const;

    // CUIBaseNode
    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override final;

protected:
    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetNodesUsingMaxSize() const override;

private:
    std::shared_ptr<CUIBaseNode>    m_Header;
    bool                            m_IsActive;
};