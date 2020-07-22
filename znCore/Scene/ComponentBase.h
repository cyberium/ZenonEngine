#pragma once

#include "../ObjectsFactories/Object.h"

class ZN_API CComponentBase 
	: public ISceneNodeComponent
	, public Object
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

	// IObject
	Guid                                            GetGUID() const override final { return Object::GetGUID(); };
	std::string                                     GetName() const override final { return Object::GetName(); };
	void                                            SetName(const std::string& Name) override final { Object::SetName(Name); };
	std::string                                     GetTypeName() const override final { return Object::GetTypeName(); };
	std::string                                     GetClassNameW() const override final { return Object::GetClassNameW(); };

	// IObjectLoadSave
	virtual void									Load(const std::shared_ptr<IByteBuffer>& Buffer) override;
	virtual void									Save(const std::shared_ptr<IByteBuffer>& Buffer) const override;
	virtual void									Load(const std::shared_ptr<IXMLReader>& Reader) override;
	virtual void									Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	const ISceneNode3D&								GetOwnerNode() const;
	IBaseManager&									GetBaseManager() const;
    void                                            RaiseComponentMessage(ComponentMessageType Message);

private:
    const ISceneNode3D&								m_OwnerNode;
	std::shared_ptr<IPropertiesGroup>               m_Properties;
};

#include "ComponentBase.inl"
