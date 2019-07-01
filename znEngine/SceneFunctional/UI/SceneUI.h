#pragma once

#include "SceneFunctional/Base/Scene.h"

#include "SceneNodeUI.h"
#include "UIWindow.h"

class OW_ENGINE_API SceneUI : public Scene
{
public:
	explicit                                        SceneUI();
	virtual                                         ~SceneUI();

    void                                            CreateRootNode();
	std::shared_ptr<CUIWindowNode>                  GetRootNode() const;

    virtual void                                    Accept(std::shared_ptr<IVisitor> visitor) override;

    virtual void                                    OnUpdate(UpdateEventArgs& e) override;

	// Input events
	bool                                            OnKeyPressed(KeyEventArgs& e);
	void                                            OnKeyReleased(KeyEventArgs& e);
	void                                            OnMouseMoved(MouseMotionEventArgs& e);
	bool                                            OnMouseButtonPressed(MouseButtonEventArgs& e);
	void                                            OnMouseButtonReleased(MouseButtonEventArgs& e);
	bool                                            OnMouseWheel(MouseWheelEventArgs& e);

protected:
    // Input events process recursive
    bool                                            DoKeyPressed_Rec(const std::shared_ptr<SceneNode>& Node, KeyEventArgs& e);
    void                                            DoKeyReleased_Rec(const std::shared_ptr<SceneNode>& Node, KeyEventArgs& e);
    void                                            DoMouseMoved_Rec(const std::shared_ptr<SceneNode>& Node, MouseMotionEventArgs& e);
    bool                                            DoMouseButtonPressed_Rec(const std::shared_ptr<SceneNode>& Node, MouseButtonEventArgs& e);
    void                                            DoMouseButtonReleased_Rec(const std::shared_ptr<SceneNode>& Node, MouseButtonEventArgs& e);
    bool                                            DoMouseWheel_Rec(const std::shared_ptr<SceneNode>& Node, MouseWheelEventArgs& e);

private:
	std::shared_ptr<CUIWindowNode>                  m_RootNode;
};
