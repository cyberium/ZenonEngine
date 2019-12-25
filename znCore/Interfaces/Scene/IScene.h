#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
ZN_INTERFACE ISceneNodesFactory;
// FORWARD END





ZN_INTERFACE OW_ENGINE_API IScene : public std::enable_shared_from_this<IScene>
{
	virtual ~IScene() {}

	virtual void CreateRootNode() = 0;
	virtual std::shared_ptr<ISceneNode> GetRootNode() const = 0;

	// Passes will go to this
	virtual void Accept(IVisitor* visitor) = 0;

	// Events
	virtual SceneChangeEvent& SceneChangeEvent() = 0;
	virtual void RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, std::shared_ptr<ISceneNode> OwnerNode, std::shared_ptr<ISceneNode> ChildNode) = 0;

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
	inline std::shared_ptr<T> CreateSceneNode(std::weak_ptr<ISceneNode> Parent, Args &&... _Args)
	{
		static_assert(std::is_convertible<T*, ISceneNode*>::value, "T must inherit ISceneNode as public.");

		std::shared_ptr<T> newNode = std::make_shared<T>(std::forward<Args>(_Args)...);
		newNode->SetScene(shared_from_this());
		newNode->SetParent(Parent);
		newNode->RegisterComponents();
		newNode->Initialize();
		newNode->RaiseOnParentChanged();

		return newNode;
	}

	// Creates new SceneNode and initialize it. You !must! call this method instead of creating nodes in code
	template<class T, typename... Args>
	inline std::shared_ptr<T> CreateWrappedSceneNode(std::string WrapperNodeTypeName, std::weak_ptr<ISceneNode> Parent, Args &&... _Args)
	{
		static_assert(std::is_convertible<T*, ISceneNode*>::value, "T must inherit ISceneNode as public.");

		std::shared_ptr<T> newNode = std::make_shared<T>(std::forward<Args>(_Args)...);

		/*if (std::shared_ptr<ISceneNodeWrapper> newNodeWrapper = std::dynamic_pointer_cast<ISceneNodeWrapper>(newNode))
		{
			IBaseManager* baseManager = std::dynamic_pointer_cast<IBaseManagerHolder>(shared_from_this())->GetBaseManager();
			std::shared_ptr<ISceneNodesFactory> sceneNodeFactory = GetManager<ISceneNodesFactory>(baseManager);
			std::shared_ptr<ISceneNode> wrappedNode = sceneNodeFactory->CreateSceneNode(Parent, WrapperNodeTypeName);

			//
			std::dynamic_pointer_cast<ISceneNodeWrapper>(wrappedNode)->SetWrappedNode(newNode);

			newNodeWrapper->SetWrappedNode(wrappedNode);
		}
		else
		{
			throw std::exception(("Unable to create wrapped node '" + WrapperNodeTypeName + "'. T isn't supports 'ISceneNodeWrapper'.").c_str());
		}*/

		newNode->SetScene(shared_from_this());
		newNode->SetParent(Parent);
		newNode->RegisterComponents();
		newNode->Initialize();
		newNode->RaiseOnParentChanged();

		return newNode;
	}
};


//
// For plugins
//
ZN_INTERFACE OW_ENGINE_API ISceneCreator
{
	virtual ~ISceneCreator() {}

	virtual size_t                                  GetScenesCount() const = 0;
	virtual std::string                             GetSceneTypeName(size_t Index) const = 0;
	virtual std::shared_ptr<IScene>					CreateScene(size_t Index) const = 0;
};

ZN_INTERFACE OW_ENGINE_API
	__declspec(uuid("CCF47DFF-A18F-46F2-B413-F17ABF991C50"))
	IScenesFactory
	: public IManager
{
	virtual ~IScenesFactory() {}

	virtual void AddSceneCreator(std::shared_ptr<ISceneCreator> Creator) = 0;
	virtual void RemoveSceneCreator(std::shared_ptr<ISceneCreator> Creator) = 0;

	virtual std::shared_ptr<IScene> CreateScene(std::string SceneTypeName) const = 0;
};

