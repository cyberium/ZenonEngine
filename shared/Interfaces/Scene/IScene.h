#pragma once

// FORWARD BEGIN
ZN_INTERFACE IRenderWindow;
ZN_INTERFACE ISceneNode;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE ISceneNodesFactory;
ZN_INTERFACE ISceneFinder;
// FORWARD END


ZN_INTERFACE ZN_API ISceneEventsListener
{
	virtual ~ISceneEventsListener() {};

	virtual void OnSceneNodeAdded(std::shared_ptr<ISceneNode> ChildNode, std::shared_ptr<ISceneNode> ParentNode) = 0;
	virtual void OnSceneNodeRemoved(std::shared_ptr<ISceneNode> ChildNode, std::shared_ptr<ISceneNode> ParentNode) = 0;
};

ZN_INTERFACE ZN_API ISceneInternal
{
	virtual ~ISceneInternal() {};

	virtual void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& OwnerNode, const std::shared_ptr<ISceneNode>& ChildNode) = 0;
};


ZN_INTERFACE ZN_API IScene 
	: public IObject
    , public std::enable_shared_from_this<IScene>
{
	virtual ~IScene() {}

	virtual IBaseManager& GetBaseManager() const = 0;
	virtual IRenderDevice& GetRenderDevice() const = 0;
	virtual IRenderWindow& GetRenderWindow() const = 0;

	virtual void SetRenderer(std::shared_ptr<IRenderer> Renderer) = 0;
	virtual std::shared_ptr<IRenderer> GetRenderer() const = 0;

	virtual void AddEventListener(std::shared_ptr<ISceneEventsListener> Listener) = 0;
	virtual void RemoveEventListener(std::shared_ptr<ISceneEventsListener> Listener) = 0;

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;

	virtual std::shared_ptr<ISceneNode> GetRootNode3D() const = 0;
	virtual std::shared_ptr<ISceneNodeUI> GetRootNodeUI() const = 0;

	virtual void SetCameraController(std::shared_ptr<ICameraController> CameraController) = 0;
	virtual std::shared_ptr<ICameraController> GetCameraController() const = 0;

	virtual const ISceneFinder& GetFinder() const = 0;

	// Passes will go to this
	virtual void Accept(IVisitor* visitor) = 0;

	// TODO: Shit code. Maybe need thread safe child containers?
	virtual void Freeze() = 0;
	virtual void Unfreeze() = 0;
	virtual void AddChild(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode) = 0;
	virtual void RemoveChild(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode) = 0;


	// Templates


	template<class T, typename... Args>
	inline std::shared_ptr<T> CreateSceneNode3DInternal(Args &&... _Args)
	{
		static_assert(std::is_convertible<T*, ISceneNode*>::value, "T must inherit ISceneNode as public.");

		std::shared_ptr<T> node = MakeShared(T, *this);
		node->RegisterComponents();
		node->Initialize();

		// Delayed loader.
		//if (Parent == nullptr)
		//	this->AddChild(GetRootNode3D(), node);
		//else
		//	this->AddChild(Parent, node);

		return node;
	}

	template<class T, typename... Args>
	inline std::shared_ptr<T> CreateSceneNodeUIInternal(Args &&... _Args)
	{
		static_assert(std::is_convertible<T*, ISceneNodeUI*>::value, "T must inherit ISceneNodeUI as public.");

		std::shared_ptr<T> newNode = MakeShared(T, std::forward<Args>(_Args)...);
		newNode->SetSceneInternal(weak_from_this());
		newNode->Initialize();

		//if (Parent)
		//	Parent->AddChild(newNode);
		//else if (GetRootNodeUI())
		//	GetRootNodeUI()->AddChild(newNode);

		return newNode;
	}
};


ZN_INTERFACE ZN_API	ISceneFinder
{
	virtual ~ISceneFinder() {}

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
