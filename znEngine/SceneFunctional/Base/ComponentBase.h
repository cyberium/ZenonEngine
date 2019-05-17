#pragma once

// FORWARD BEGIN
class SceneNode;
// FORWARD END

class CComponentBase : public ISceneNodeComponent
{
public:
    CComponentBase(std::shared_ptr<SceneNode> OwnerNode);
    virtual ~CComponentBase();

    std::shared_ptr<SceneNode>                      GetOwnerNode() const;

    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              IsComponentExists();
    template<typename T>
    std::shared_ptr<T>                              GetComponent();

    // Accept from SceneNode
    virtual bool                                    Accept(IVisitor& visitor) override;

    // ISceneNodeComponent
    virtual void                                    OnParentChanged() override;
    virtual void                                    OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) override;

protected:
    void                                            RaiseComponentMessage(ComponentMessageType Message);

private:
    std::weak_ptr<SceneNode>                        m_OwnerNode;
};

#include "ComponentBase.inl"
