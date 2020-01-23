#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE ISceneNodesFactory;
// FORWARD END





ZN_INTERFACE ZN_API __declspec(novtable) IScene 
	: public std::enable_shared_from_this<IScene>
{
	virtual ~IScene() {}

	virtual void CreateRootNodes() = 0;
	virtual ISceneNode3D* GetRootNode3D() const = 0;
	virtual ISceneNodeUI* GetRootNodeUI() const = 0;

	// Load & Save
	virtual bool Load(std::shared_ptr<IXMLReader> Reader) = 0;
	virtual bool Save(std::shared_ptr<IXMLWriter> Writer) = 0;

	// Passes will go to this
	virtual void Accept(IVisitor* visitor) = 0;

	// Events
	virtual SceneChangeEvent& SceneChangeEvent() = 0;
	virtual void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, std::shared_ptr<ISceneNode3D> OwnerNode, std::shared_ptr<ISceneNode3D> ChildNode) = 0;

	// Events
	virtual void OnUpdate(UpdateEventArgs& e) = 0;

	// Keyboard events
	virtual bool OnKeyPressed(KeyEventArgs& e) = 0;
	virtual void OnKeyReleased(KeyEventArgs& e) = 0;
	virtual void OnKeyboardFocus(EventArgs& e) = 0;
	virtual void OnKeyboardBlur(EventArgs& e) = 0;

	// Mouse events
	virtual void OnMouseMoved(MouseMotionEventArgs& e) = 0;
	virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) = 0;
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) = 0;
	virtual bool OnMouseWheel(MouseWheelEventArgs& e) = 0;
	virtual void OnMouseLeave(EventArgs& e) = 0;
	virtual void OnMouseFocus(EventArgs& e) = 0;
	virtual void OnMouseBlur(EventArgs& e) = 0;


	// Templates


	// Creates new SceneNode and initialize it. You !must! call this method instead of creating nodes in code
	template<class T, typename... Args>
	inline T* CreateSceneNode(ISceneNode3D* Parent, Args &&... _Args)
	{
		static_assert(std::is_convertible<T*, ISceneNode3D*>::value, "T must inherit ISceneNode3D as public.");

		std::shared_ptr<T> newNode = std::make_shared<T>(std::forward<Args>(_Args)...);
		newNode->SetScene(weak_from_this());
		newNode->RegisterComponents();
		newNode->Initialize();
		newNode->SetParent(Parent);

		return newNode.get();
	}

	// Creates new SceneNode and initialize it. You !must! call this method instead of creating nodes in code
	template<class T, typename... Args>
	inline T* CreateSceneNodeUI(ISceneNodeUI* Parent, Args &&... _Args)
	{
		static_assert(std::is_convertible<T*, ISceneNodeUI*>::value, "T must inherit ISceneNodeUI as public.");

		std::shared_ptr<T> newNode = std::make_shared<T>(std::forward<Args>(_Args)...);
		newNode->SetScene(weak_from_this());
		newNode->Initialize();
		newNode->SetParent(Parent);

		return newNode.get();
	}
};

#if 0

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

#endif

