#pragma once

#include "SceneFunctional/Base/SceneBase.h"

class OW_ENGINE_API SceneUI 
	: public SceneBase
{
public:
	explicit                                        SceneUI(std::shared_ptr<IBaseManager> BaseManager);
	virtual                                         ~SceneUI();

    void                                            CreateRootNode();

	// Input events
	bool                                            OnKeyPressed(KeyEventArgs& e);
	void                                            OnKeyReleased(KeyEventArgs& e);
	void                                            OnMouseMoved(MouseMotionEventArgs& e);
	bool                                            OnMouseButtonPressed(MouseButtonEventArgs& e);
	void                                            OnMouseButtonReleased(MouseButtonEventArgs& e);
	bool                                            OnMouseWheel(MouseWheelEventArgs& e);

protected:
    // Input events process recursive
    bool                                            DoKeyPressed_Rec(const std::shared_ptr<ISceneNode>& Node, KeyEventArgs& e);
    void                                            DoKeyReleased_Rec(const std::shared_ptr<ISceneNode>& Node, KeyEventArgs& e);
    void                                            DoMouseMoved_Rec(const std::shared_ptr<ISceneNode>& Node, MouseMotionEventArgs& e);
    bool                                            DoMouseButtonPressed_Rec(const std::shared_ptr<ISceneNode>& Node, MouseButtonEventArgs& e);
    void                                            DoMouseButtonReleased_Rec(const std::shared_ptr<ISceneNode>& Node, MouseButtonEventArgs& e);
    bool                                            DoMouseWheel_Rec(const std::shared_ptr<ISceneNode>& Node, MouseWheelEventArgs& e);
};
