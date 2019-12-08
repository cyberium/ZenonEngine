#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE IScene;
ZN_INTERFACE ICamera;
class Viewport;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE IManager;
ZN_INTERFACE ISettingGroup;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API ISceneNode : public std::enable_shared_from_this<ISceneNode>
{
	virtual ~ISceneNode() {}

	virtual void									Initialize() = 0;
	virtual void									Finalize() = 0;

	virtual std::string								GetName() const = 0;
	virtual void                                    SetName(const std::string& name) = 0;

	// Components engine
	virtual bool                                    IsComponentExists(GUID ComponentID) = 0;
	virtual std::shared_ptr<ISceneNodeComponent>    GetComponent(GUID ComponentID) = 0;
	virtual std::shared_ptr<ISceneNodeComponent>    AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component) = 0;
	virtual const ComponentsMap&                    GetComponents() const = 0;
	virtual void                                    RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) = 0;
	virtual void                                    RegisterComponents() = 0;

	// Scene access
	virtual void                                    SetScene(std::shared_ptr<IScene> Scene) = 0;
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

	virtual bool                                    Accept(IVisitor* visitor) = 0;

	virtual void                                    OnUpdate(UpdateEventArgs& e) = 0;

	virtual std::shared_ptr<ISettingGroup>          GetProperties() const = 0;

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



ZN_INTERFACE OW_ENGINE_API ISceneNodeInternal
{
	virtual ~ISceneNodeInternal() {}

	virtual void SetParentInternal(std::weak_ptr<ISceneNode> parentNode) = 0;
};


ZN_INTERFACE OW_ENGINE_API ISceneNode3D
{
	virtual ~ISceneNode3D() {}
};

ZN_INTERFACE OW_ENGINE_API ISceneNodeUI
{
	virtual ~ISceneNodeUI() {}
};

ZN_INTERFACE OW_ENGINE_API ISceneNodeWrapper
{
	virtual ~ISceneNodeWrapper() {}

	virtual void SetWrappedNode(std::shared_ptr<ISceneNode> ThisNode) = 0;
	virtual std::shared_ptr<ISceneNode> GetWrappedNode() const = 0;
};


//
// For plugins
//
ZN_INTERFACE OW_ENGINE_API ISceneNodeCreator
{
	virtual ~ISceneNodeCreator() {}

	virtual size_t                                  GetSceneNodesCount() const = 0;
	virtual std::string                             GetSceneNodeTypeName(size_t Index) const = 0;
	virtual std::shared_ptr<ISceneNode>             CreateSceneNode(std::weak_ptr<ISceneNode> Parent, size_t Index) const = 0;
};

ZN_INTERFACE OW_ENGINE_API
	__declspec(uuid("9C3ACF8D-F30D-47AE-BBA1-D71DEA6B14D4"))
	ISceneNodesFactory
	: public IManager
{
	virtual ~ISceneNodesFactory() {}

	virtual void AddSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) = 0;
	virtual void RemoveSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) = 0;

	virtual std::shared_ptr<ISceneNode> CreateSceneNode(std::weak_ptr<ISceneNode> Parent, std::string SceneNodeTypeName) const = 0;
};
