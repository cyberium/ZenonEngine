#include "stdafx.h"

// General
#include "SceneUI.h"

// Additional
#include "SceneNodeUI.h"
#include "UIWindow.h"

SceneUI::SceneUI(std::shared_ptr<IBaseManager> BaseManager)
	: SceneBase(BaseManager)
{}

SceneUI::~SceneUI()
{}

void SceneUI::CreateRootNode()
{
    m_RootNode = CreateSceneNode<CUIWindowNode>(std::shared_ptr<ISceneNode>());
}

//
// Input events
//
bool SceneUI::OnKeyPressed(KeyEventArgs & e)
{
	if (m_RootNode)
		return DoKeyPressed_Rec(m_RootNode, e);

	return false;
}

void SceneUI::OnKeyReleased(KeyEventArgs & e)
{
	if (m_RootNode)
        DoKeyReleased_Rec(m_RootNode, e);
}

void SceneUI::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_RootNode)
        DoMouseMoved_Rec(m_RootNode, e);
}

bool SceneUI::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (m_RootNode)
		return DoMouseButtonPressed_Rec(m_RootNode, e);

	return false;
}

void SceneUI::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_RootNode)
        DoMouseButtonReleased_Rec(m_RootNode, e);
}

bool SceneUI::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_RootNode)
		return DoMouseWheel_Rec(m_RootNode, e);

	return false;
}



//
// Input events process recursive
//
bool SceneUI::DoKeyPressed_Rec(const std::shared_ptr<ISceneNode>& Node, KeyEventArgs & e)
{
    std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
    _ASSERT(NodeAsUINode != nullptr);

    for (auto child : NodeAsUINode->GetChilds())
    {
        if (DoKeyPressed_Rec(child, e))
            return true;
    }

    if (NodeAsUINode->OnKeyPressed(e))
        return true;

    return false;
}

void SceneUI::DoKeyReleased_Rec(const std::shared_ptr<ISceneNode>& Node, KeyEventArgs & e)
{
    std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
    _ASSERT(NodeAsUINode != nullptr);

    for (auto child : NodeAsUINode->GetChilds())
    {
        DoKeyReleased_Rec(child, e);
    }

    NodeAsUINode->OnKeyReleased(e);
}

void SceneUI::DoMouseMoved_Rec(const std::shared_ptr<ISceneNode>& Node, MouseMotionEventArgs & e)
{
    std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
    _ASSERT(NodeAsUINode != nullptr);

    for (auto child : NodeAsUINode->GetChilds())
    {
        DoMouseMoved_Rec(child, e);
    }

    NodeAsUINode->OnMouseMoved(e);

    // Synteric events impl
    if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
    {
        if (!NodeAsUINode->IsMouseOnNode())
        {
            NodeAsUINode->OnMouseEntered();
            NodeAsUINode->DoMouseEntered();
        }
    }
    else
    {
        if (NodeAsUINode->IsMouseOnNode())
        {
            NodeAsUINode->OnMouseLeaved();
            NodeAsUINode->DoMouseLeaved();
        }
    }
}

bool SceneUI::DoMouseButtonPressed_Rec(const std::shared_ptr<ISceneNode>& Node, MouseButtonEventArgs & e)
{
    std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
    _ASSERT(NodeAsUINode != nullptr);

    for (auto child : NodeAsUINode->GetChilds())
    {
        if (DoMouseButtonPressed_Rec(child, e))
            return true;
    }

    if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
        if (NodeAsUINode->OnMouseButtonPressed(e))
            return true;

    return false;
}

void SceneUI::DoMouseButtonReleased_Rec(const std::shared_ptr<ISceneNode>& Node, MouseButtonEventArgs & e)
{
    std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
    _ASSERT(NodeAsUINode != nullptr);

    for (auto child : NodeAsUINode->GetChilds())
    {
        DoMouseButtonReleased_Rec(child, e);
    }

    NodeAsUINode->OnMouseButtonReleased(e);
}

bool SceneUI::DoMouseWheel_Rec(const std::shared_ptr<ISceneNode>& Node, MouseWheelEventArgs & e)
{
    std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
    _ASSERT(NodeAsUINode != nullptr);

    for (auto child : NodeAsUINode->GetChilds())
    {
        if (DoMouseWheel_Rec(child, e))
            return true;
    }

    if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
        if (NodeAsUINode->OnMouseWheel(e))
            return true;

    return false;
}
