#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE IPropertiesG;
ZN_INTERFACE IScene;
ZN_INTERFACE ICamera;
class Viewport;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE IManager;
// FORWARD END

ZN_INTERFACE ZN_API ISceneNode : public std::enable_shared_from_this<ISceneNode>
{
	virtual ~ISceneNode() {}

	virtual void									Initialize() = 0;
	virtual void									Finalize() = 0;

	virtual std::string								GetType() const = 0;
	virtual void                                    SetType(std::string Type) = 0;
	virtual std::string								GetName() const = 0;
	virtual void                                    SetName(std::string Name) = 0;

	// Transform functional
	virtual mat4									GetLocalTransform() const = 0;
	virtual mat4									GetInverseLocalTransform() const = 0;
	virtual void									SetLocalTransform(cmat4 localTransform) = 0;

	virtual mat4									GetWorldTransfom() const = 0;
	virtual mat4									GetInverseWorldTransform() const = 0;
	virtual mat4									GetParentWorldTransform() const = 0;
	virtual void									SetWorldTransform(cmat4 worldTransform) = 0;

	virtual void									ForceRecalculateLocalTransform() = 0;


	// Components engine
	virtual bool                                    IsComponentExists(GUID ComponentID) = 0;
	virtual std::shared_ptr<ISceneNodeComponent>    GetComponent(GUID ComponentID) = 0;
	virtual std::shared_ptr<ISceneNodeComponent>    AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component) = 0;
	virtual const ComponentsMap&                    GetComponents() const = 0;
	virtual void                                    RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) = 0;
	virtual void                                    RegisterComponents() = 0;


	// Actions & Properties
	virtual std::shared_ptr<IActionsGroup>          GetActions() const = 0;
	virtual std::shared_ptr<IPropertiesGroup>       GetProperties() const = 0;


	// Scene access
	virtual std::shared_ptr<IScene>                 GetScene() const = 0;


	// Childs functional
	virtual void                                    AddChild(std::shared_ptr<ISceneNode> childNode) = 0;
	virtual void                                    RemoveChild(std::shared_ptr<ISceneNode> childNode) = 0;
	virtual void                                    SetParent(std::weak_ptr<ISceneNode> parentNode) = 0;
	virtual std::shared_ptr<ISceneNode>             GetParent() const = 0;
	virtual std::vector<std::shared_ptr<ISceneNode>>GetChilds() = 0;


	// Called before all others calls
	virtual void                                    UpdateCamera(const ICamera* camera) = 0;
	virtual void                                    UpdateViewport(const Viewport* viewport) = 0;


	// Load & Save
	virtual bool                                    Load(std::shared_ptr<IXMLReader> Reader) = 0;
	virtual bool                                    Save(std::shared_ptr<IXMLWriter> Writer) = 0;


	// Visitor functionality
	virtual bool                                    Accept(IVisitor* visitor) = 0;

	virtual void                                    OnUpdate(UpdateEventArgs& e) = 0;

	virtual void                                    RaiseOnParentChanged() = 0;

	//
	// Templates
	//


	// Components engine template access
	template<typename T>
	inline std::shared_ptr<T> IsComponentExists()
	{
		return IsComponentExists(__uuidof(T));
	}
	template<typename T>
	inline std::shared_ptr<T> GetComponent()
	{
		std::shared_ptr<ISceneNodeComponent> component = GetComponent(__uuidof(T));
		if (component == nullptr)
			return std::shared_ptr<T>();

		return std::dynamic_pointer_cast<T, ISceneNodeComponent>(component);
	}
	template<typename T>
	inline std::shared_ptr<T> AddComponent(std::shared_ptr<T> Component)
	{
		std::shared_ptr<ISceneNodeComponent> component = AddComponent(__uuidof(T), Component);
		if (component == nullptr)
			return std::shared_ptr<T>();

		return std::dynamic_pointer_cast<T, ISceneNodeComponent>(component);
	}

	// Creates new SceneNode and initialize it. You !must! call this method instead of creating nodes in code
	template<typename T, typename... Args>
	inline std::shared_ptr<T> CreateSceneNode(Args &&... _Args)
	{
		return GetScene()->CreateSceneNode<T>(shared_from_this(), std::forward<Args>(_Args)...);
	}

	// Creates new SceneNode and initialize it. You !must! call this method instead of creating nodes in code
	template<typename T, typename... Args>
	inline std::shared_ptr<T> CreateWrappedSceneNode(std::string WrapperNodeTypeName, Args &&... _Args)
	{
		return GetScene()->CreateWrappedSceneNode<T>(WrapperNodeTypeName, shared_from_this(), std::forward<Args>(_Args)...);
	}
};
typedef std::vector<std::shared_ptr<ISceneNode>>                NodeList;
typedef std::multimap<std::string, std::shared_ptr<ISceneNode>> NodeNameMap;



ZN_INTERFACE ZN_API ISceneNodeInternal
{
	virtual ~ISceneNodeInternal() {}

	virtual void SetScene(std::shared_ptr<IScene> Scene) = 0;
	virtual void SetParentInternal(std::weak_ptr<ISceneNode> parentNode) = 0;
};


ZN_INTERFACE ZN_API ISceneNode3D
{
	virtual ~ISceneNode3D() {}

	virtual void SetTranslate(cvec3 _translate) = 0;
	virtual cvec3 GetTranslation() const = 0;

	virtual void SetRotation(cvec3 _rotate) = 0;
	virtual cvec3 GetRotation() const = 0;

	virtual void SetRotationQuaternion(cquat _rotate) = 0;
	virtual cquat GetRotationQuaternion() const = 0;

	virtual void SetScale(cvec3 _scale) = 0;
	virtual cvec3 GetScale() const = 0;
};

ZN_INTERFACE ZN_API ISceneNodeUI
{
	virtual ~ISceneNodeUI() {}

	virtual void SetTranslate(cvec2 _translate) = 0;
	virtual cvec2 GetTranslation() const = 0;
	virtual glm::vec2 GetTranslationAbs() const = 0;

	virtual void SetRotation(cvec3 _rotate) = 0;
	virtual cvec3 GetRotation() const = 0;

	virtual void SetScale(cvec2 _scale) = 0;
	virtual cvec2 GetScale() const = 0;
	virtual glm::vec2 GetScaleAbs() const = 0;
};

/*ZN_INTERFACE ZN_API ISceneNodeWrapper
{
	virtual ~ISceneNodeWrapper() {}

	virtual void SetWrappedNode(std::shared_ptr<ISceneNode> ThisNode) = 0;
	virtual std::shared_ptr<ISceneNode> GetWrappedNode() const = 0;
};*/


//
// For plugins
//
ZN_INTERFACE ZN_API ISceneNodeCreator
{
	virtual ~ISceneNodeCreator() {}

	virtual size_t                                  GetSceneNodesCount() const = 0;
	virtual std::string                             GetSceneNodeTypeName(size_t Index) const = 0;
	virtual std::shared_ptr<ISceneNode>             CreateSceneNode(std::weak_ptr<ISceneNode> Parent, size_t Index) const = 0;
};

ZN_INTERFACE ZN_API
	__declspec(uuid("9C3ACF8D-F30D-47AE-BBA1-D71DEA6B14D4"))
	ISceneNodesFactory
	: public IManager
{
	virtual ~ISceneNodesFactory() {}

	virtual void AddSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) = 0;
	virtual void RemoveSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) = 0;

	virtual std::shared_ptr<ISceneNode> CreateSceneNode(std::weak_ptr<ISceneNode> Parent, std::string SceneNodeTypeName) const = 0;
};
