#pragma once

class ZN_API CComponentBase 
	: public ISceneNodeComponent
{
public:
    CComponentBase(const ISceneNode3D& OwnerNode);
    virtual ~CComponentBase();

	const ISceneNode3D&								GetOwnerNode() const;

    // Components engine template access
    template<typename T>
    std::shared_ptr<T>                              IsComponentExists();
    template<typename T>
    std::shared_ptr<T>                              GetComponent();


	// ISceneNodeComponent
	virtual void                                    OnParentChanged() override;
	virtual void                                    OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) override;
	virtual IPropertiesGroup*                       GetPropertiesGroup() const override;

	// Load & Save
	virtual bool                                    Load(std::shared_ptr<IXMLReader> Reader) override;
	virtual bool                                    Save(std::shared_ptr<IXMLWriter> Writer) override;

	// Called before all others calls
	virtual void                                    DoUpdate(UpdateEventArgs& e) override;

    // Accept from SceneNode
    virtual void                                    Accept(IVisitor* visitor) override;

protected:
    void                                            RaiseComponentMessage(ComponentMessageType Message);

private:
    const ISceneNode3D&								m_OwnerNode;
	std::shared_ptr<IPropertiesGroup>               m_PropertyGroup;
};

#include "ComponentBase.inl"
