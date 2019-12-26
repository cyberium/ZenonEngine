#pragma once

class ZN_API CComponentBase 
	: public ISceneNodeComponent
{
public:
    CComponentBase(std::shared_ptr<ISceneNode> OwnerNode);
    virtual ~CComponentBase();

    std::shared_ptr<ISceneNode>                     GetOwnerNode() const;

    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              IsComponentExists();
    template<typename T>
    std::shared_ptr<T>                              GetComponent();

	// Load & Save
	virtual bool                                    Load(std::shared_ptr<IXMLReader> Reader) override;
	virtual bool                                    Save(std::shared_ptr<IXMLWriter> Writer) override;

    // Accept from SceneNode
    virtual bool                                    Accept(IVisitor* visitor) override;

    // ISceneNodeComponent
    virtual void                                    OnParentChanged() override;
    virtual void                                    OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) override;
	virtual std::shared_ptr<IPropertiesGroup>       GetPropertiesGroup() const override;

protected:
    void                                            RaiseComponentMessage(ComponentMessageType Message);

private:
    std::weak_ptr<ISceneNode>                       m_OwnerNode;
	std::shared_ptr<IPropertiesGroup>               m_PropertyGroup;
};

#include "ComponentBase.inl"
