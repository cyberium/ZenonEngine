#pragma once

class OW_ENGINE_API SceneBase 
	: public IScene
	, public IBaseManagerHolder
	, public Object
{
public:
	SceneBase(IBaseManager* BaseManager);
	virtual ~SceneBase();

	// IScene
	void											CreateRootNode();
	std::shared_ptr<ISceneNode>                     GetRootNode() const;

	// Passes
	void                                            Accept(IVisitor* visitor);

	// Events
    void                                            OnUpdate(UpdateEventArgs& e);

	// Keyboard events
	virtual bool                                    OnKeyPressed(KeyEventArgs& e);
	virtual void                                    OnKeyReleased(KeyEventArgs& e);
	virtual void                                    OnKeyboardFocus(EventArgs& e);
	virtual void                                    OnKeyboardBlur(EventArgs& e);

	// Mouse events
	virtual void                                    OnMouseMoved(MouseMotionEventArgs& e);
	virtual bool                                    OnMouseButtonPressed(MouseButtonEventArgs& e);
	virtual void                                    OnMouseButtonReleased(MouseButtonEventArgs& e);
	virtual bool                                    OnMouseWheel(MouseWheelEventArgs& e);
	virtual void                                    OnMouseLeave(EventArgs& e);
	virtual void                                    OnMouseFocus(EventArgs& e);
	virtual void                                    OnMouseBlur(EventArgs& e);

	// IBaseManagerHolder
	IBaseManager*                                   GetBaseManager() const override final;

protected:
	// Input events process recursive
	bool                                            DoKeyPressed_Rec(const std::shared_ptr<ISceneNode>& Node, KeyEventArgs& e);
	void                                            DoKeyReleased_Rec(const std::shared_ptr<ISceneNode>& Node, KeyEventArgs& e);
	void                                            DoMouseMoved_Rec(const std::shared_ptr<ISceneNode>& Node, MouseMotionEventArgs& e);
	bool                                            DoMouseButtonPressed_Rec(const std::shared_ptr<ISceneNode>& Node, MouseButtonEventArgs& e);
	void                                            DoMouseButtonReleased_Rec(const std::shared_ptr<ISceneNode>& Node, MouseButtonEventArgs& e);
	bool                                            DoMouseWheel_Rec(const std::shared_ptr<ISceneNode>& Node, MouseWheelEventArgs& e);

protected:
	std::shared_ptr<ISceneNode>                     m_RootNode;

private:
	IBaseManager*                                   m_BaseManager;
};
