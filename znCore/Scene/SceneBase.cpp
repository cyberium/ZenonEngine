#include "stdafx.h"

// General
#include "SceneBase.h"

// Additonal
#include "3D/SceneNode3D.h"
#include "UI/SceneNodeUI.h"

SceneBase::SceneBase(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{}

SceneBase::~SceneBase()
{}



//
// IScene
//
void SceneBase::CreateRootNodes()
{
	m_RootNode3D = std::shared_ptr<SceneNode3D>();
	m_RootNode3D->SetScene(weak_from_this());
	m_RootNode3D->RegisterComponents();
	m_RootNode3D->Initialize();
	m_RootNode3D->SetParent(nullptr);
	m_RootNode3D->SetName("Root node 3D");

	m_RootNodeUI = std::shared_ptr<CUIBaseNode>();
	m_RootNodeUI->SetScene(weak_from_this());
	m_RootNodeUI->Initialize();
	m_RootNodeUI->SetParent(nullptr);
	m_RootNodeUI->SetName("Root node 3D");
}

ISceneNode3D* SceneBase::GetRootNode3D() const
{
	return m_RootNode3D.get();
}

ISceneNodeUI * SceneBase::GetRootNodeUI() const
{
	return m_RootNodeUI.get();
}

bool SceneBase::Load(std::shared_ptr<IXMLReader> Reader)
{
	return false;
}

bool SceneBase::Save(std::shared_ptr<IXMLWriter> Writer)
{
	std::shared_ptr<IXMLWriter> writer = Writer->CreateChild("Scene");
	writer->AddAttribute("IsScene", "true");

	for (const auto& rootChild : GetRootNode3D()->GetChilds())
		rootChild->Save(writer);

	return false;
}

void SceneBase::Accept(IVisitor * visitor)
{
	if (m_RootNode3D)
		m_RootNode3D->Accept(visitor);
}

Delegate<SceneChangeEventArgs>& SceneBase::SceneChangeEvent()
{
	return m_SceneChangeEvent;
}

void SceneBase::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, std::shared_ptr<ISceneNode3D> OwnerNode, std::shared_ptr<ISceneNode3D> ChildNode)
{
	m_SceneChangeEvent(SceneChangeEventArgs(this, shared_from_this(), SceneChangeType, OwnerNode, ChildNode));
}



//
// RenderWindow events
//
void SceneBase::OnUpdate(UpdateEventArgs& e)
{
	if (m_RootNode3D)
		m_RootNode3D->OnUpdate(e);
}



//
// Keyboard events
//
bool SceneBase::OnKeyPressed(KeyEventArgs & e)
{
	if (m_RootNodeUI)
		return DoKeyPressed_Rec(m_RootNodeUI, e);

	return false;
}

void SceneBase::OnKeyReleased(KeyEventArgs & e)
{
	if (m_RootNodeUI)
		DoKeyReleased_Rec(m_RootNodeUI, e);
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
	if (m_RootNodeUI)
		DoMouseMoved_Rec(m_RootNodeUI, e);
}

bool SceneBase::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (m_RootNodeUI)
		return DoMouseButtonPressed_Rec(m_RootNodeUI, e);

	return false;
}

void SceneBase::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_RootNodeUI)
		DoMouseButtonReleased_Rec(m_RootNodeUI, e);
}

bool SceneBase::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_RootNodeUI)
		return DoMouseWheel_Rec(m_RootNodeUI, e);

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
bool SceneBase::DoKeyPressed_Rec(const std::shared_ptr<ISceneNodeUI>& Node, KeyEventArgs & e)
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

void SceneBase::DoKeyReleased_Rec(const std::shared_ptr<ISceneNodeUI>& Node, KeyEventArgs & e)
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

void SceneBase::DoMouseMoved_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseMotionEventArgs & e)
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

bool SceneBase::DoMouseButtonPressed_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseButtonEventArgs & e)
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

void SceneBase::DoMouseButtonReleased_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseButtonEventArgs & e)
{
	std::shared_ptr<CUIBaseNode> NodeAsUINode = std::dynamic_pointer_cast<CUIBaseNode>(Node);
	if (NodeAsUINode != nullptr)
	{
		for (auto child : NodeAsUINode->GetChilds())
			DoMouseButtonReleased_Rec(child, e);

		NodeAsUINode->OnMouseButtonReleased(e);
	}
}

bool SceneBase::DoMouseWheel_Rec(const std::shared_ptr<ISceneNodeUI>& Node, MouseWheelEventArgs & e)
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
