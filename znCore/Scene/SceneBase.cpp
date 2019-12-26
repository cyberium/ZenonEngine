#include "stdafx.h"

// General
#include "SceneBase.h"

// Additonal
#include "UI/SceneNodeUI.h"

SceneBase::SceneBase(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{}

SceneBase::~SceneBase()
{}



//
// IScene
//
void SceneBase::CreateRootNode()
{
	m_RootNode = CreateSceneNode<SceneNodeBase>(std::shared_ptr<ISceneNode>());
	m_RootNode->SetName("Root node");
}

std::shared_ptr<ISceneNode> SceneBase::GetRootNode() const
{
	return m_RootNode;
}

bool SceneBase::Load(std::shared_ptr<IXMLReader> Reader)
{
	return false;
}

bool SceneBase::Save(std::shared_ptr<IXMLWriter> Writer)
{
	std::shared_ptr<IXMLWriter> writer = Writer->CreateChild("Scene");
	writer->AddAttribute("IsScene", "true");

	for (const auto& rootChild : GetRootNode()->GetChilds())
		rootChild->Save(writer);

	return false;
}

void SceneBase::Accept(IVisitor * visitor)
{
	if (m_RootNode)
		m_RootNode->Accept(visitor);
}

Delegate<SceneChangeEventArgs>& SceneBase::SceneChangeEvent()
{
	return m_SceneChangeEvent;
}

void SceneBase::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, std::shared_ptr<ISceneNode> OwnerNode, std::shared_ptr<ISceneNode> ChildNode)
{
	m_SceneChangeEvent(SceneChangeEventArgs(this, shared_from_this(), SceneChangeType, OwnerNode, ChildNode));
}



//
// RenderWindow events
//
void SceneBase::OnUpdate(UpdateEventArgs& e)
{
	if (m_RootNode)
		m_RootNode->OnUpdate(e);
}



//
// Keyboard events
//
bool SceneBase::OnKeyPressed(KeyEventArgs & e)
{
	if (m_RootNode)
		return DoKeyPressed_Rec(m_RootNode, e);

	return false;
}

void SceneBase::OnKeyReleased(KeyEventArgs & e)
{
	if (m_RootNode)
		DoKeyReleased_Rec(m_RootNode, e);
}

void SceneBase::OnKeyboardFocus(EventArgs & e)
{
}

void SceneBase::OnKeyboardBlur(EventArgs & e)
{
}



//
// Mouse events
//
void SceneBase::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_RootNode)
		DoMouseMoved_Rec(m_RootNode, e);
}

bool SceneBase::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (m_RootNode)
		return DoMouseButtonPressed_Rec(m_RootNode, e);

	return false;
}

void SceneBase::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_RootNode)
		DoMouseButtonReleased_Rec(m_RootNode, e);
}

bool SceneBase::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_RootNode)
		return DoMouseWheel_Rec(m_RootNode, e);

	return false;
}

void SceneBase::OnMouseLeave(EventArgs & e)
{
}

void SceneBase::OnMouseFocus(EventArgs & e)
{
}

void SceneBase::OnMouseBlur(EventArgs & e)
{
}



//
// Input events process recursive
//
bool SceneBase::DoKeyPressed_Rec(const std::shared_ptr<ISceneNode>& Node, KeyEventArgs & e)
{
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			if (DoKeyPressed_Rec(child, e))
				return true;

		if (NodeAsUINode->OnKeyPressed(e))
			return true;
	}

	return false;
}

void SceneBase::DoKeyReleased_Rec(const std::shared_ptr<ISceneNode>& Node, KeyEventArgs & e)
{
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
	if (NodeAsUINode != nullptr)
	{

		for (auto child : NodeAsUINode->GetChilds())
		{
			DoKeyReleased_Rec(child, e);
		}

		NodeAsUINode->OnKeyReleased(e);
	}
}

void SceneBase::DoMouseMoved_Rec(const std::shared_ptr<ISceneNode>& Node, MouseMotionEventArgs & e)
{
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
	if (NodeAsUINode != nullptr)
	{
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
}

bool SceneBase::DoMouseButtonPressed_Rec(const std::shared_ptr<ISceneNode>& Node, MouseButtonEventArgs & e)
{
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			if (DoMouseButtonPressed_Rec(child, e))
				return true;

		if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
			if (NodeAsUINode->OnMouseButtonPressed(e))
				return true;
	}

	return false;
}

void SceneBase::DoMouseButtonReleased_Rec(const std::shared_ptr<ISceneNode>& Node, MouseButtonEventArgs & e)
{
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			DoMouseButtonReleased_Rec(child, e);

		NodeAsUINode->OnMouseButtonReleased(e);
	}
}

bool SceneBase::DoMouseWheel_Rec(const std::shared_ptr<ISceneNode>& Node, MouseWheelEventArgs & e)
{
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
	if (NodeAsUINode != nullptr)
	{

		for (auto child : NodeAsUINode->GetChilds())
		{
			if (DoMouseWheel_Rec(child, e))
				return true;
		}

		if (NodeAsUINode->IsPointInBoundsAbs(e.GetPoint()))
			if (NodeAsUINode->OnMouseWheel(e))
				return true;
	}

	return false;
}



//
// IBaseManagerHolder
//
IBaseManager* SceneBase::GetBaseManager() const
{
	return m_BaseManager;
}
