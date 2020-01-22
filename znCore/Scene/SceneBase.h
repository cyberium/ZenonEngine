#pragma once

#include "3D//SceneNode3D.h"
#include "UI//SceneNodeUI.h"

class ZN_API SceneBase 
	: public IScene
	, public IBaseManagerHolder
	, public Object
{
public:
	SceneBase(IBaseManager* BaseManager);
	virtual ~SceneBase();

	// IScene
	void											CreateRootNodes() override;
	ISceneNode3D*									GetRootNode3D() const override;
	ISceneNodeUI*									GetRootNodeUI() const override;

	// Load & Save
	bool                                            Load(std::shared_ptr<IXMLReader> Reader) override;
	bool                                            Save(std::shared_ptr<IXMLWriter> Writer) override;

	// Visit funcitonal
	void                                            Accept(IVisitor* visitor) override;

	// Events
	Delegate<SceneChangeEventArgs>&					SceneChangeEvent() override;
	void                                            RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, std::shared_ptr<ISceneNode3D> OwnerNode, std::shared_ptr<ISceneNode3D> ChildNode);

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
	bool                                            DoKeyPressed_Rec(const std::shared_ptr<ISceneNodeUI>& Node, KeyEventArgs& e);
	void                                            DoKeyReleased_Rec(const std::shared_ptr<ISceneNodeUI>& Node, KeyEventArgs& e);
	void                                            DoMouseMoved_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseMotionEventArgs& e);
	bool                                            DoMouseButtonPressed_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseButtonEventArgs& e);
	void                                            DoMouseButtonReleased_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseButtonEventArgs& e);
	bool                                            DoMouseWheel_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseWheelEventArgs& e);

protected:
	std::shared_ptr<SceneNode3D>                    m_RootNode3D;
	std::shared_ptr<CUIBaseNode>                    m_RootNodeUI;

private:
	Delegate<SceneChangeEventArgs>					m_SceneChangeEvent;

private:
	IBaseManager*                                   m_BaseManager;
};
