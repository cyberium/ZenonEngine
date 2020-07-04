#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE ISceneNodesFactory;
// FORWARD END


ZN_INTERFACE ZN_API IScene 
	: public std::enable_shared_from_this<IScene>
{
	virtual ~IScene() {}

	virtual void SetRenderWindow(const std::shared_ptr<IRenderWindow>& RenderWindow) = 0;
	virtual void ConnectEvents(const std::shared_ptr<IRenderWindowEvents>& WindowEvents) = 0;
	virtual void DisconnectEvents(const std::shared_ptr<IRenderWindowEvents>& WindowEvents) = 0;

	virtual void Initialize() = 0;
	virtual void Finalize() = 0;

	virtual std::shared_ptr<ISceneNode3D> GetRootNode3D() const = 0;
	virtual std::shared_ptr<ISceneNodeUI> GetRootNodeUI() const = 0;

	virtual void SetCameraController(std::shared_ptr<ICameraController> CameraController) = 0;
	virtual std::shared_ptr<ICameraController> GetCameraController() const = 0;

	// Passes will go to this
	virtual void Accept(IVisitor* visitor) = 0;

	// TODO: Shit code. Maybe need thread safe child containers?
	virtual void Freeze() = 0;
	virtual void Unfreeze() = 0;
	virtual void AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) = 0;
	virtual void RemoveChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) = 0;

	// Events
	virtual SceneChangeEvent& SceneChangeEvent() = 0;
	virtual void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode) = 0;


	// Templates


	template<class T, typename... Args>
	inline std::shared_ptr<T> CreateSceneNode(const std::shared_ptr<ISceneNode3D>& Parent, Args &&... _Args)
	{
		static_assert(std::is_convertible<T*, ISceneNode3D*>::value, "T must inherit ISceneNode3D as public.");

		std::shared_ptr<T> node = std::make_shared<T>(std::forward<Args>(_Args)...);
		node->SetSceneInternal(weak_from_this());
		node->RegisterComponents();
		node->Initialize();

		// Delayed loader.
		if (Parent == nullptr)
			this->AddChild(GetRootNode3D(), node);
		else
			this->AddChild(Parent, node);

		return node;
	}

	template<class T, typename... Args>
	inline std::shared_ptr<T> CreateSceneNodeUI(const std::shared_ptr<ISceneNodeUI>& Parent, Args &&... _Args)
	{
		static_assert(std::is_convertible<T*, ISceneNodeUI*>::value, "T must inherit ISceneNodeUI as public.");

		std::shared_ptr<T> newNode = std::make_shared<T>(std::forward<Args>(_Args)...);
		newNode->SetSceneInternal(weak_from_this());
		newNode->Initialize();

		if (Parent)
			Parent->AddChild(newNode);
		else if (GetRootNodeUI())
			GetRootNodeUI()->AddChild(newNode);

		return newNode;
	}
};



//
// For plugins
//
ZN_INTERFACE ZN_API ISceneCreator
{
	virtual ~ISceneCreator() {}

	virtual size_t                                  GetScenesCount() const = 0;
	virtual std::string                             GetSceneTypeName(size_t Index) const = 0;
	virtual std::shared_ptr<IScene>					CreateScene(size_t Index) const = 0;
};

ZN_INTERFACE ZN_API
	__declspec(uuid("CCF47DFF-A18F-46F2-B413-F17ABF991C50"))
	IScenesFactory
	: public IManager
{
	virtual ~IScenesFactory() {}

	virtual void AddSceneCreator(std::shared_ptr<ISceneCreator> Creator) = 0;
	virtual void RemoveSceneCreator(std::shared_ptr<ISceneCreator> Creator) = 0;

	virtual std::shared_ptr<IScene> CreateScene(std::string SceneTypeName) const = 0;
};
