#pragma once

#include "../ObjectsFactories/Object.h"

class ZN_API CSceneNode
	: public ISceneNode
	, public ISceneNodeInternal
	, public Object
{
    friend IScene;
public:
	CSceneNode(IScene& Scene);
	virtual ~CSceneNode();

	// ISceneNode
	virtual void                                    Initialize() override;
	virtual void                                    Finalize() override;
	virtual void                                    CopyTo(std::shared_ptr<ISceneNode> Destination) const override;

	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<ISceneNode> childNode) override final;
	virtual void                                    RemoveChild(std::shared_ptr<ISceneNode> childNode) override final;
	virtual std::shared_ptr<ISceneNode>             GetParent() const override final;
	virtual const SceneNodesList&                   GetChilds() const override final;
	virtual std::shared_ptr<ISceneNode>             GetChild(std::string Name) const override;
	bool                                            IsPersistance() const override;

	// Actions & Properties
	std::shared_ptr<IPropertiesGroup>				GetProperties() const override final;
	IScene&											GetScene() const override final;

	//
	// Transform functional
	//
	void											SetTranslate(const glm::vec3& Translate) override;
	void                                            AddTranslate(const glm::vec3& Translate) override;
	const glm::vec3&								GetTranslation() const override;
	void											SetRotation(const glm::vec3& _rotate) override;
	const glm::vec3&                                GetRotation() const override;
	void											SetRotationQuaternion(const glm::quat& _rotate) override;
	const glm::quat&								GetRotationQuaternion() const override;
	void											SetScale(const glm::vec3& _scale) override;
	const glm::vec3&								GetScale() const override;

	virtual glm::mat4								GetLocalTransform() const;
	virtual glm::mat4								GetInverseLocalTransform() const;
	virtual void									SetLocalTransform(const glm::mat4& localTransform);

	virtual glm::mat4								GetWorldTransfom() const;
	virtual glm::mat4								GetInverseWorldTransform() const;
	virtual glm::mat4								GetParentWorldTransform() const;
	virtual void									SetWorldTransform(const glm::mat4& worldTransform);


	//
	// Components engine
	//
	bool                                            IsComponentExists(ObjectClass ComponentID) const override;
	std::shared_ptr<ISceneNodeComponent>            GetComponent(ObjectClass ComponentID) const override;
	std::shared_ptr<ISceneNodeComponent>            AddComponent(ObjectClass ComponentID, std::shared_ptr<ISceneNodeComponent> Component) override;
	const ComponentsMap&                            GetComponents() const override;
	void                                            RaiseComponentMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) const override;
	virtual void                                    RegisterComponents() override;

	// Others
	virtual void                                    Update(const UpdateEventArgs& e) override;
	virtual void                                    Accept(IVisitor* visitor) override;

	// IObject
	Guid                                            GetGUID() const override final { return Object::GetGUID(); };
	std::string                                     GetName() const override { return Object::GetName(); };
	void                                            SetName(const std::string& Name) override;

	// IObjectLoadSave
	virtual void									Load(const std::shared_ptr<IXMLReader>& Reader) override;
	virtual void									Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

protected:
	// ISceneNodeInternal
	void                                            AddChildInternal(std::shared_ptr<ISceneNode> ChildNode) override;
	void                                            RemoveChildInternal(std::shared_ptr<ISceneNode> ChildNode) override;
	void                                            SetParentInternal(std::weak_ptr<ISceneNode> parentNode) override;
	void                                            SetPersistanceInternal(bool Value) override;
	void                                            RaiseOnParentChangedInternal() override;

protected:
	virtual glm::mat4                               CalculateLocalTransform() const;
	void											UpdateLocalTransform();
	void											UpdateWorldTransform();
	void                                            DoLoadProperties(const std::shared_ptr<IXMLReader>& Reader) const;
	void                                            DoSaveProperties(const std::shared_ptr<IXMLWriter>& Writer) const;

	IBaseManager&                                   GetBaseManager() const;
	IRenderDevice&                                  GetRenderDevice() const;

private:
	IScene&                                         m_Scene;

	glm::vec3										m_Translate;
	glm::vec3										m_Rotate;
	glm::quat										m_RotateQuat;
	bool											m_IsRotateQuat;
	glm::vec3										m_Scale;
	glm::mat4										m_LocalTransform;
	glm::mat4										m_InverseLocalTransform;
	glm::mat4										m_WorldTransform;
	glm::mat4										m_InverseWorldTransform;

	std::shared_ptr<IPropertiesGroup>               m_PropertiesGroup;

	ComponentsMap                                   m_Components;

	std::weak_ptr<ISceneNode>                       m_ParentNode;
	SceneNodesList                                  m_Children;
	bool                                            m_IsPersistance;
};

inline bool IsChildOf(const std::shared_ptr<ISceneNode>& Parent, const std::shared_ptr<ISceneNode>& Child)
{
	if (Parent == nullptr || Child == nullptr)
		return false;

	if (Parent->GetGUID() == Child->GetGUID())
		return true;

	for (const auto& ch : Parent->GetChilds())
		if (IsChildOf(ch, Child))
			return true;

	return false;
}
