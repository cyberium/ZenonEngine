#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE IScene;
ZN_INTERFACE ICameraComponent3D;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE IColliderComponent;
ZN_INTERFACE IColliderComponent;
// FORWARD END


/**
  * Добавление чилда:
  * 1) Пересчитать текущий WorldTransform
  * 2) Пересчитать WorldTransform для всех чилдов
  * 3) 
*/


ZN_INTERFACE ZN_API ISceneNode
	: public virtual IObject
	, public std::enable_shared_from_this<ISceneNode>
{
	typedef std::vector<std::shared_ptr<ISceneNode>> SceneNodesList;

	enum ZN_API ERotationKind : size_t
	{
		Euler = 0,
		Quaternion,
		Direction
	};

	ZN_OBJECTCLASS(cSceneNode3D)
	virtual ~ISceneNode() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;

	virtual void SetEnabled(bool Value) = 0;
	virtual bool IsEnabled() const = 0;

	virtual void AddChild(std::shared_ptr<ISceneNode> childNode) = 0;
	virtual void RemoveChild(std::shared_ptr<ISceneNode> childNode) = 0;
	virtual void MakeMeOrphan() = 0;
	virtual std::shared_ptr<ISceneNode> GetParent() const = 0;
	virtual const SceneNodesList& GetChilds() const = 0;
	virtual std::shared_ptr<ISceneNode> GetChild(std::string Name) const = 0;
	virtual bool IsPersistance() const = 0; // Means this node can't be deleted from parent and any parent may contains only one instance of this node (by Name)

	virtual IScene& GetScene() const = 0;

	// Transform functional
	virtual void SetPosition(glm::vec3 Position) = 0;
	virtual glm::vec3 GetPosition() const = 0;

	virtual void SetLocalPosition(glm::vec3 LocalPosition) = 0;
	virtual glm::vec3 GetLocalPosition() const = 0;

	virtual void SetDirection(glm::vec3 Direction) = 0;
	virtual glm::vec3 GetDirection() const = 0;

	virtual void SetRotationEuler(glm::vec3 Rotation) = 0;
	virtual glm::vec3 GetRotationEuler() const = 0;
	
	virtual void SetRotationQuaternion(glm::quat Rotation) = 0;
	virtual glm::quat GetRotationQuaternion() const = 0;

	virtual ISceneNode::ERotationKind GetRotationKind() const = 0;

	virtual void SetScale(glm::vec3 Scale) = 0;
	virtual glm::vec3 GetScale() const = 0;

	virtual void SetLocalTransform(const glm::mat4& localTransform) = 0;
	virtual glm::mat4 GetLocalTransform() const = 0;
	virtual glm::mat4 GetInverseLocalTransform() const = 0;
	
	virtual void SetWorldTransform(const glm::mat4& worldTransform) = 0;
	virtual glm::mat4 GetWorldTransfom() const = 0;
	virtual glm::mat4 GetInverseWorldTransform() const = 0;
	virtual glm::mat4 GetParentWorldTransform() const = 0;
	
	// Components engine
	virtual bool IsComponentExists(ObjectClass ComponentID) const = 0;
	virtual std::shared_ptr<ISceneNodeComponent> GetComponent(ObjectClass ComponentID) const = 0;
	virtual std::shared_ptr<ISceneNodeComponent> AddComponent(ObjectClass ComponentID, std::shared_ptr<ISceneNodeComponent> Component) = 0;
	virtual const ComponentsMap& GetComponents() const = 0;
	virtual void RaiseComponentMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) const = 0;
	virtual void RegisterComponents() = 0;

	template<typename T> inline bool IsComponentExistsT() const
	{
		return IsComponentExists(T::GetClassT());
	}
	template<typename T> inline std::shared_ptr<T> GetComponentT() const
	{
		if (std::shared_ptr<ISceneNodeComponent> component = GetComponent(T::GetClassT()))
			return std::dynamic_pointer_cast<T>(component);
		return nullptr;
	}
	template<typename T> inline std::shared_ptr<T> AddComponentT(std::shared_ptr<T> Component)
	{
		if (std::shared_ptr<ISceneNodeComponent> component = AddComponent(T::GetClassT(), std::dynamic_pointer_cast<ISceneNodeComponent>(Component)))
			return std::dynamic_pointer_cast<T>(component);
		return nullptr;
	}

	// Actions
	virtual void Update(const UpdateEventArgs& e) = 0;
	virtual void Accept(IVisitor* visitor) = 0;
};


ZN_INTERFACE ZN_API ISceneNodeInternal
{
	virtual ~ISceneNodeInternal() {}

	virtual void AddChildInternal(std::shared_ptr<ISceneNode> ChildNode) = 0;     // Called from scene
	virtual void RemoveChildInternal(std::shared_ptr<ISceneNode> ChildNode) = 0;  // Called from scene
	virtual void SetPersistanceInternal(bool Value) = 0;
	virtual void RaiseOnParentChangedInternal() = 0;
};
