#pragma once

#include "../ObjectsFactories/Object.h"

class ZN_API CSceneNode
	: virtual public ISceneNode
	, public ISceneNodeInternal
	, public Object
{
public:
	CSceneNode(IScene& Scene);
	virtual ~CSceneNode();

	// ISceneNode
	virtual void                                    Initialize() override;
	virtual void                                    Finalize() override;

	virtual void                                    SetEnabled(bool Value) override;
	virtual bool                                    IsEnabled() const override;

	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<ISceneNode> childNode) override final;
	virtual void                                    RemoveChild(std::shared_ptr<ISceneNode> childNode) override final;
	virtual void                                    MakeMeOrphan() override final;
	virtual std::shared_ptr<ISceneNode>             GetParent() const override final;
	virtual const SceneNodesList&                   GetChilds() const override final;
	virtual std::shared_ptr<ISceneNode>             GetChild(std::string Name) const override;
	bool                                            IsPersistance() const override;

	// Actions & Properties
	IScene&											GetScene() const override final;

	// Transform functional
	void                                            SetPosition(glm::vec3 Position) override;
	glm::vec3                                       GetPosition() const override;

	void                                            SetLocalPosition(glm::vec3 LocalPosition) override;
	glm::vec3                                       GetLocalPosition() const override;

	void                                            SetDirection(glm::vec3 Direction) override;
	glm::vec3                                       GetDirection() const override;
	void                                            SetRotationEuler(glm::vec3 Rotation) override;
	glm::vec3                                       GetRotationEuler() const override;
	void                                            SetRotationQuaternion(glm::quat Rotation) override;
	glm::quat                                       GetRotationQuaternion() const override;
	ISceneNode::ERotationKind                       GetRotationKind() const override;

	void											SetScale(glm::vec3 Scale) override;
	glm::vec3								        GetScale() const override;

	void											SetLocalTransform(const glm::mat4& localTransform);
	glm::mat4										GetLocalTransform() const;
	glm::mat4										GetInverseLocalTransform() const;
	
	void											SetWorldTransform(const glm::mat4& worldTransform);
	glm::mat4										GetWorldTransfom() const;
	glm::mat4										GetInverseWorldTransform() const;
	glm::mat4										GetParentWorldTransform() const;

	// Components engine
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
	//void                                            SetName(const std::string& Name) override;

	// IObjectLoadSave
	virtual std::shared_ptr<IObject>                Copy() const override;
	virtual void                                    CopyTo(std::shared_ptr<IObject> Destination) const override;
	virtual void									Load(const std::shared_ptr<IXMLReader>& Reader) override;
	virtual void									Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

	// ISceneNodeInternal
	void                                            AddChildInternal(std::shared_ptr<ISceneNode> ChildNode) override;     // Called from scene
	void                                            RemoveChildInternal(std::shared_ptr<ISceneNode> ChildNode) override;  // Called from scene
	void                                            SetPersistanceInternal(bool Value) override;
	void                                            RaiseOnParentChangedInternal() override;

protected:
	virtual glm::mat4                               CalculateLocalTransform() const;
	void											UpdateLocalTransform();
	void											UpdateWorldTransform();
	void                                            DoLoadProperties(const std::shared_ptr<IXMLReader>& Reader) const;
	void                                            DoSaveProperties(const std::shared_ptr<IXMLWriter>& Writer) const;
	IRenderDevice&                                  GetRenderDevice() const;

private:
	void                                            AddChildPrivate(std::shared_ptr<ISceneNode> ChildNode);
	void                                            RemoveChildPrivate(std::shared_ptr<ISceneNode> ChildNode);
	void                                            SetParentPrivate(std::weak_ptr<ISceneNode> parentNode);

private:
	IScene&                                         m_Scene;

	bool                                            m_IsEnabled;

	glm::vec3										m_LocalPosition;
	std::shared_ptr<CPropertyWrappedVec3>           m_LocalPositionProperty;

	glm::vec3										m_Rotation;
	std::shared_ptr<CPropertyWrappedVec3>           m_RotationProperty;
	glm::quat										m_RotationQuaternion;
	ERotationKind                                   m_RotationKind;
	
	glm::vec3										m_Scale;
	std::shared_ptr<CPropertyWrappedVec3>           m_ScaleProperty;

	glm::mat4										m_LocalTransform;
	glm::mat4										m_InverseLocalTransform;
	glm::mat4										m_WorldTransform;
	glm::mat4										m_InverseWorldTransform;

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
