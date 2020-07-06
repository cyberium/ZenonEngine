#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE IScene;
ZN_INTERFACE ICameraComponent3D;
class Viewport;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE IManager;
ZN_INTERFACE IColliderComponent3D;
ZN_INTERFACE IColliderComponent3D;
// FORWARD END


/**
  * Добавление чилда:
  * 1) Пересчитать текущий WorldTransform
  * 2) Пересчитать WorldTransform для всех чилдов
  * 3) 
*/

const ObjectClass cSceneNode3D = 527338441;

ZN_INTERFACE ZN_API ISceneNode3D
	: public Object
	, public std::enable_shared_from_this<ISceneNode3D>
{
	typedef std::vector<std::shared_ptr<ISceneNode3D>>                Node3DList;
	typedef std::multimap<std::string, std::shared_ptr<ISceneNode3D>> Node3DNameMap;

	static ObjectType GetType() { return otSceneNode3D; }
	static ObjectClass GetClass() { return cSceneNode3D; }

	virtual ~ISceneNode3D() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Copy(std::shared_ptr<ISceneNode3D> Destination) const = 0;

	virtual void AddChild(std::shared_ptr<ISceneNode3D> childNode) = 0;
	virtual void RemoveChild(std::shared_ptr<ISceneNode3D> childNode) = 0;
	virtual std::weak_ptr<ISceneNode3D> GetParent() const = 0;
	virtual const Node3DList& GetChilds() = 0;
	virtual void ClearChilds() = 0;
	virtual void RaiseOnParentChanged() = 0;

	//template<typename T, typename... Args> inline std::shared_ptr<T> CreateSceneNode(Args &&... _Args)
	//{
	//	if (auto scene = GetScene())
	//		return scene->CreateSceneNode3D<T>(shared_from_this(), std::forward<Args>(_Args)...);
	//	
	//	return nullptr;
	//}

	virtual std::shared_ptr<IPropertiesGroup> GetProperties() const = 0;
	virtual IScene* GetScene() const = 0;



	//
	// Transform functional
	//
	virtual void SetTranslate(const glm::vec3& Translate) = 0;
	virtual void AddTranslate(const glm::vec3& Translate) = 0;
	virtual const glm::vec3& GetTranslation() const = 0;

	virtual void SetRotation(const glm::vec3& _rotate) = 0;
	virtual const glm::vec3& GetRotation() const = 0;

	virtual void SetRotationQuaternion(const glm::quat& _rotate) = 0;
	virtual const glm::quat& GetRotationQuaternion() const = 0;

	virtual void SetScale(const glm::vec3& _scale) = 0;
	virtual const glm::vec3& GetScale() const = 0;

	virtual glm::mat4 GetLocalTransform() const = 0;
	virtual glm::mat4 GetInverseLocalTransform() const = 0;
	virtual void SetLocalTransform(const glm::mat4& localTransform) = 0;

	virtual glm::mat4 GetWorldTransfom() const = 0;
	virtual glm::mat4 GetInverseWorldTransform() const = 0;
	virtual glm::mat4 GetParentWorldTransform() const = 0;
	virtual void SetWorldTransform(const glm::mat4& worldTransform) = 0;


	virtual const std::shared_ptr<IColliderComponent3D>& GetColliderComponent() const = 0;
	virtual const std::shared_ptr<IModelsComponent3D>& GetModelsComponent() const = 0;

	//
	// Components engine
	//
	virtual bool IsComponentExists(GUID ComponentID) const = 0;
	virtual std::shared_ptr<ISceneNodeComponent> GetComponent(GUID ComponentID) const = 0;
	virtual std::shared_ptr<ISceneNodeComponent> AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component) = 0;
	virtual const ComponentsMap& GetComponents() const = 0;
	virtual void RaiseComponentMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) const = 0;
	virtual void RegisterComponents() = 0;

	

	template<typename T> inline bool IsComponentExists()
	{
		return IsComponentExists(__uuidof(T));
	}
	template<typename T> inline std::shared_ptr<T> GetComponent() const
	{
		if (std::shared_ptr<ISceneNodeComponent> component = GetComponent(__uuidof(T)))
			return std::dynamic_pointer_cast<T>(component);
		return nullptr;
	}
	template<typename T> inline std::shared_ptr<T> AddComponent(std::shared_ptr<T> Component)
	{
		if (std::shared_ptr<ISceneNodeComponent> component = AddComponent(__uuidof(T), Component))
			return std::dynamic_pointer_cast<T>(component);
		return nullptr;
	}

	// Actions
	virtual void Update(const UpdateEventArgs& e) = 0;
	virtual void Accept(IVisitor* visitor) = 0;
};


ZN_INTERFACE ZN_API ISceneNode3DInternal
{
	virtual ~ISceneNode3DInternal() {}

	virtual void SetSceneInternal(std::weak_ptr<IScene> Scene) = 0;
	virtual void AddChildInternal(std::shared_ptr<ISceneNode3D> ChildNode) = 0;
	virtual void RemoveChildInternal(std::shared_ptr<ISceneNode3D> ChildNode) = 0;
	virtual void SetParentInternal(std::weak_ptr<ISceneNode3D> parentNode) = 0;
};
