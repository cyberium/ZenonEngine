#pragma once

#include "../ObjectsFactories/Object.h"

class ZN_API SceneNode3D
	: public ISceneNode3D
	, public ISceneNode3DInternal
	, public Object
{
    friend IScene;
public:
	SceneNode3D(IScene& Scene);
	virtual ~SceneNode3D();

	// ISceneNode3D
	virtual void                                    Initialize() override;
	virtual void                                    Finalize() override;
	virtual void                                    Copy(std::shared_ptr<ISceneNode3D> Destination) const override;

	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<ISceneNode3D> childNode) override final;
	virtual void                                    RemoveChild(std::shared_ptr<ISceneNode3D> childNode) override final;
	virtual std::weak_ptr<ISceneNode3D>             GetParent() const override final;
	virtual const Node3DList&                       GetChilds() const override final;
	void											ClearChilds() override;
	void                                            RaiseOnParentChanged() override final;

	// Actions & Properties
	std::shared_ptr<IPropertiesGroup>				GetProperties() const override final;
	IScene*											GetScene() const override final;

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
	const std::shared_ptr<IColliderComponent3D>&    GetColliderComponent() const;
	const std::shared_ptr<IModelsComponent3D>&      GetModelsComponent() const;

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

private:
	// ISceneNode3DInternal
	void                                            AddChildInternal(std::shared_ptr<ISceneNode3D> ChildNode) override;
	void                                            RemoveChildInternal(std::shared_ptr<ISceneNode3D> ChildNode) override;
	void                                            SetParentInternal(std::weak_ptr<ISceneNode3D> parentNode) override;

protected:
	virtual glm::mat4                               CalculateLocalTransform() const;
	void											UpdateLocalTransform();
	void											UpdateWorldTransform();
	void                                            DoLoadProperties(const std::shared_ptr<IXMLReader>& Reader) const;
	void                                            DoSaveProperties(const std::shared_ptr<IXMLWriter>& Writer) const;

	IBaseManager&                                   GetBaseManager() const;
	IRenderDevice&                                  GetRenderDevice() const;

	std::shared_ptr<IColliderComponent3D>			m_Components_Collider;
	std::shared_ptr<IModelsComponent3D>				m_Components_Models;

private:
	Node3DList                                      m_Children;
	std::weak_ptr<ISceneNode3D>                     m_ParentNode;

	std::shared_ptr<IPropertiesGroup>               m_PropertiesGroup;
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

	ComponentsMap                                   m_Components;
};

inline bool IsChildOf(const std::shared_ptr<ISceneNode3D>& Parent, const std::shared_ptr<ISceneNode3D>& Child)
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
