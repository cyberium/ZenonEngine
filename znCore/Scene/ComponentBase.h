#pragma once

class ZN_API CComponentBase 
	: public ISceneNodeComponent
{
public:
    CComponentBase(const ISceneNode3D& OwnerNode);
    virtual ~CComponentBase();

    // Components engine template access
    template<class T>
    std::shared_ptr<T>                              IsComponentExists() const;
    template<class T>
    std::shared_ptr<T>                              GetComponent() const;

	void											Copy(std::shared_ptr<ISceneNodeComponent> Destination) const override;

	// ISceneNodeComponent
	virtual void                                    OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) override;
	virtual std::shared_ptr<IPropertiesGroup>       GetProperties() const override;

    // Accept from SceneNode
	virtual void                                    Update(const UpdateEventArgs& e) override;
    virtual void                                    Accept(IVisitor* visitor) override;

protected:
	const ISceneNode3D&								GetOwnerNode() const;
    void                                            RaiseComponentMessage(ComponentMessageType Message);

private:
    const ISceneNode3D&								m_OwnerNode;
	std::shared_ptr<IPropertiesGroup>               m_Properties;
};

#include "ComponentBase.inl"
