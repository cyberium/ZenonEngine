#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
ZN_INTERFACE IVisitor;
class UpdateEventArgs;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API IScene : public std::enable_shared_from_this<IScene>
{
	virtual ~IScene() {}

	virtual void                                    CreateRootNode() = 0;
	virtual std::shared_ptr<ISceneNode>				GetRootNode() const = 0;

	// Passes will go to this
	virtual void                                    Accept(IVisitor* visitor) = 0;

	// Events
	virtual void                                    OnUpdate(UpdateEventArgs& e) = 0;

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

		std::shared_ptr<ISceneNodeWrapper> newNodeWrapper = std::dynamic_pointer_cast<ISceneNodeWrapper>(newNode);
		if (newNodeWrapper)
			newNodeWrapper->SetThisNode(newNode);

		newNode->SetScene(shared_from_this());
		newNode->SetParent(Parent);
		newNode->RegisterComponents();
		newNode->Initialize();
		newNode->RaiseOnParentChanged();

		return newNode;
	}
};
