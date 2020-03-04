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
// FORWARD END

typedef ZN_API int64 SceneNodeType;

ZN_INTERFACE ZN_API ISceneNode3D
	: public std::enable_shared_from_this<ISceneNode3D>
{
	typedef std::vector<std::shared_ptr<ISceneNode3D>>                Node3DList;
	typedef std::multimap<std::string, std::shared_ptr<ISceneNode3D>> Node3DNameMap;

	virtual ~ISceneNode3D() {}

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;

	virtual void SetType(SceneNodeType Type) = 0;
	virtual SceneNodeType GetType() const = 0;
	virtual bool Is(SceneNodeType SceneNodeType) const = 0;
	virtual void SetName(std::string Name) = 0;
	virtual std::string	GetName() const = 0;

	virtual void AddChild(const std::shared_ptr<ISceneNode3D>& childNode) = 0;
	virtual void RemoveChild(const std::shared_ptr<ISceneNode3D>& childNode) = 0;
	virtual std::weak_ptr<ISceneNode3D> GetParent() const = 0;
	virtual const Node3DList& GetChilds() = 0;
	virtual void RaiseOnParentChanged() = 0;

	template<typename T, typename... Args> inline std::shared_ptr<T> CreateSceneNode(Args &&... _Args)
	{
		return GetScene()->CreateSceneNode<T>(shared_from_this(), std::forward<Args>(_Args)...);
	}

	virtual IActionsGroup* GetActions() const = 0;
	virtual IPropertiesGroup* GetProperties() const = 0;
	virtual IScene* GetScene() const = 0;



	//
	// Transform functional
	//
	virtual void SetTranslate(cvec3 Translate) = 0;
	virtual void AddTranslate(vec3 Translate) = 0;
	virtual cvec3 GetTranslation() const = 0;

	virtual void SetRotation(cvec3 _rotate) = 0;
	virtual cvec3 GetRotation() const = 0;

	virtual void SetRotationQuaternion(cquat _rotate) = 0;
	virtual cquat GetRotationQuaternion() const = 0;

	virtual void SetScale(cvec3 _scale) = 0;
	virtual cvec3 GetScale() const = 0;

	virtual mat4 GetLocalTransform() const = 0;
	virtual mat4 GetInverseLocalTransform() const = 0;
	virtual void SetLocalTransform(cmat4 localTransform) = 0;

	virtual mat4 GetWorldTransfom() const = 0;
	virtual mat4 GetInverseWorldTransform() const = 0;
	virtual mat4 GetParentWorldTransform() const = 0;
	virtual void SetWorldTransform(cmat4 worldTransform) = 0;



	//
	// Components engine
	//
	virtual bool IsComponentExists(GUID ComponentID) const = 0;
	virtual std::shared_ptr<ISceneNodeComponent> GetComponent(GUID ComponentID) const = 0;
	virtual std::shared_ptr<ISceneNodeComponent> AddComponent(GUID ComponentID, const std::shared_ptr<ISceneNodeComponent>& Component) = 0;
	virtual const ComponentsMap& GetComponents() const = 0;
	virtual void RaiseComponentMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) const = 0;
	virtual void RegisterComponents() = 0;

	virtual const std::shared_ptr<IColliderComponent3D>& GetColliderComponent() const = 0;

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
	template<> inline std::shared_ptr<IColliderComponent3D> GetComponent<IColliderComponent3D>() const
	{
		return GetColliderComponent();
	}
	template<typename T> inline std::shared_ptr<T> AddComponent(const std::shared_ptr<T>& Component)
	{
		if (std::shared_ptr<ISceneNodeComponent> component = AddComponent(__uuidof(T), Component))
			return std::dynamic_pointer_cast<T>(component);
		return nullptr;
	}

	// Load & Save
	virtual bool Load(std::shared_ptr<IXMLReader> Reader) = 0;
	virtual bool Save(std::shared_ptr<IXMLWriter> Writer) = 0;

	// Actions
	virtual void Update(const UpdateEventArgs& e) = 0;
	virtual void Accept(IVisitor* visitor) = 0;
};
