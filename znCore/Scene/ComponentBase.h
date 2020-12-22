#pragma once

#include "../ObjectsFactories/Object.h"

class ZN_API CComponentBase 
	: virtual public ISceneNodeComponent
	, public Object
{
public:
    CComponentBase(const ISceneNode& OwnerNode);
    virtual ~CComponentBase();

	// ISceneNodeComponent
	virtual void                                    OnMessage(const ISceneNodeComponent* FromComponent, ComponentMessageType Message) override;
    // Accept from SceneNode
	virtual void                                    Update(const UpdateEventArgs& e) override;
    virtual void                                    Accept(IVisitor* visitor) override;

	// IObjectLoadSave
	std::shared_ptr<IObject>                        Copy() const override;
	void											CopyTo(std::shared_ptr<IObject> Destination) const override;
	virtual void									Load(const std::shared_ptr<IXMLReader>& Reader) override;
	virtual void									Save(const std::shared_ptr<IXMLWriter>& Writer) const override;



	//
	// Templates
	//

	template<class T>
	inline std::shared_ptr<T> IsComponentExistsT() const
	{
		return GetOwnerNode().IsComponentExists(T::GetClassT());
	}

	template<class T>
	inline std::shared_ptr<T> GetComponentT() const
	{
		if (auto component = GetOwnerNode().GetComponent(T::GetClassT()))
			return std::dynamic_pointer_cast<T, ISceneNodeComponent>(component);
		return nullptr;
	}

protected:
	const ISceneNode&								GetOwnerNode() const;
    void                                            RaiseComponentMessage(ComponentMessageType Message);

private:
    const ISceneNode&								m_OwnerNode;
};
