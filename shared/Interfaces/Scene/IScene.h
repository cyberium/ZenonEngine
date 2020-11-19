#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderWindow;
ZN_INTERFACE ISceneNode;
ZN_INTERFACE IUIControl;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE IObjectsFactory;
ZN_INTERFACE ISceneNodeFactory;
ZN_INTERFACE ISceneFinder;
// FORWARD END


ZN_INTERFACE ZN_API ISceneEventsListener
{
	virtual ~ISceneEventsListener() {};

	virtual void OnSceneNodeAdded(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode) = 0;
	virtual void OnSceneNodeRemoved(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode) = 0;
};


ZN_INTERFACE ZN_API ISceneInternal
{
	virtual ~ISceneInternal() {};

	virtual void AddChildInternal(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode) = 0;
	virtual void RemoveChildInternal(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode) = 0;
};

ZN_INTERFACE ZN_API ISceneLoadSave
{
	virtual ~ISceneLoadSave() {};

	virtual void LoadFromFile(const std::string& FileName) = 0;
	virtual void SaveToFile(const std::string& FileName) = 0;
	virtual void ResetScene() = 0;
};


ZN_INTERFACE ZN_API IScene
{
	virtual ~IScene() {}

	virtual IBaseManager& GetBaseManager() const = 0;
	virtual IRenderDevice& GetRenderDevice() const = 0;
	virtual IRenderWindow& GetRenderWindow() const = 0;
	virtual const ISceneFinder& GetFinder() const = 0;

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;

	virtual void SetRenderer(std::shared_ptr<IRenderer> Renderer) = 0;
	virtual std::shared_ptr<IRenderer> GetRenderer() const = 0;

	virtual void AddSceneEventsListener(ISceneEventsListener* Listener) = 0;
	virtual void RemoveSceneEventsListener(ISceneEventsListener* Listener) = 0;

	virtual std::shared_ptr<ISceneNode> GetRootSceneNode() const = 0;
	virtual std::shared_ptr<IUIControl> GetRootUIControl() const = 0;

	virtual void SetCameraController(std::shared_ptr<ICameraController> CameraController) = 0;
	virtual std::shared_ptr<ICameraController> GetCameraController() const = 0;

	// Passes will go to this
	virtual void Accept(IVisitor* visitor) = 0;


	//
	// SceneNode
	//


	template<class T>
	inline std::shared_ptr<ISceneNode> CreateSceneNodeByClass(ObjectClass ObjectClass, std::shared_ptr<ISceneNode> Parent = nullptr)
	{
		return GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(ObjectClass, *this, Parent);
	}

	template<class T>
	inline std::shared_ptr<ISceneNode> CreateSceneNode(std::shared_ptr<ISceneNode> Parent = nullptr)
	{
		//static_assert(std::is_convertible<T*, ISceneNode*>::value, "T must inherit ISceneNode as public.");
		return GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(T::GetClassT(), *this, Parent);
	}

	template<class T>
	inline std::shared_ptr<T> CreateSceneNodeT(std::shared_ptr<ISceneNode> Parent = nullptr)
	{
		//static_assert(std::is_convertible<T*, ISceneNode*>::value, "T must inherit ISceneNode as public.");
		auto createdNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(T::GetClassT(), *this, Parent);
		return std::dynamic_pointer_cast<T>(createdNode);
	}



	//
	// UIControl
	//



	template<class T>
	inline std::shared_ptr<IUIControl> CreateUIControlByClass(ObjectClass ObjectClass, std::shared_ptr<IUIControl> Parent = nullptr)
	{
		return GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(ObjectClass, *this, Parent);
	}

	template<class T>
	inline std::shared_ptr<IUIControl> CreateUIControl(std::shared_ptr<IUIControl> Parent = nullptr)
	{
		return GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(T::GetClassT(), *this, Parent);
	}

	template<class T>
	inline std::shared_ptr<T> CreateUIControlT(std::shared_ptr<IUIControl> Parent = nullptr)
	{
		return std::dynamic_pointer_cast<T>(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(T::GetClassT(), *this, Parent));
	}
};


ZN_INTERFACE ZN_API	ISceneFinder
{
	virtual ~ISceneFinder() {}

	virtual std::map<float, std::shared_ptr<ISceneNode>> FindNearestNodes(
		glm::vec3 Position,
		float Distance,
		std::function<bool(std::shared_ptr<ISceneNode>)> Filter = nullptr,
		std::shared_ptr<ISceneNode> RootForFinder = nullptr
	) const = 0;

	virtual std::map<float, std::shared_ptr<ISceneNode>> FindIntersection (
		const Ray& Ray,
		std::function<bool(std::shared_ptr<ISceneNode>)> Filter = nullptr, 
		std::shared_ptr<ISceneNode> RootForFinder = nullptr
	) const = 0;

	virtual std::vector<std::shared_ptr<ISceneNode>> FindIntersections(
		const Frustum& Frustum, 
		std::function<bool(std::shared_ptr<ISceneNode>)> Filter = nullptr, 
		std::shared_ptr<ISceneNode> RootForFinder = nullptr
	) const = 0;


	virtual std::map<float, std::shared_ptr<IModel>> FindIntersectionWithModel(
		const Ray& Ray,
		std::function<bool(std::shared_ptr<ISceneNode>)> FilterForSceneNodes = nullptr,
		std::function<bool(std::shared_ptr<IModel>)> FilterForModels = nullptr,
		std::shared_ptr<ISceneNode> RootForFinder = nullptr
	) const = 0;

	virtual std::vector<std::shared_ptr<IModel>> FindIntersectionsWithModel(
		const Frustum& Frustum,
		std::function<bool(std::shared_ptr<ISceneNode>)> FilterForSceneNodes = nullptr,
		std::function<bool(std::shared_ptr<IModel>)> FilterForModels = nullptr,
		std::shared_ptr<ISceneNode> RootForFinder = nullptr
	) const = 0;
};
