#pragma once

#include "UIBaseNode.h"
#include "UIWindow.h"

class CUIScene : public Object
{
public:
	CUIScene();
	virtual ~CUIScene();

	std::shared_ptr<CUIWindowNode> GetRootNode() const;

	void Accept(IVisitor& visitor);

	// Input events
	bool OnKeyPressed(KeyEventArgs& e);
	void OnKeyReleased(KeyEventArgs& e);
	void OnMouseMoved(MouseMotionEventArgs& e);
	bool OnMouseButtonPressed(MouseButtonEventArgs& e);
	void OnMouseButtonReleased(MouseButtonEventArgs& e);
	bool OnMouseWheel(MouseWheelEventArgs& e);

protected:
    // Input events process recursive
    bool DoKeyPressed_Rec(const std::shared_ptr<CUIBaseNode>& Node, KeyEventArgs& e);
    void DoKeyReleased_Rec(const std::shared_ptr<CUIBaseNode>& Node, KeyEventArgs& e);
    void DoMouseMoved_Rec(const std::shared_ptr<CUIBaseNode>& Node, MouseMotionEventArgs& e);
    bool DoMouseButtonPressed_Rec(const std::shared_ptr<CUIBaseNode>& Node, MouseButtonEventArgs& e);
    void DoMouseButtonReleased_Rec(const std::shared_ptr<CUIBaseNode>& Node, MouseButtonEventArgs& e);
    bool DoMouseWheel_Rec(const std::shared_ptr<CUIBaseNode>& Node, MouseWheelEventArgs& e);

private:
	std::shared_ptr<CUIWindowNode> m_pRootNode;
};
