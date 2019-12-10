#include "stdafx.h"

// General
#include "SceneNodeUI.h"

// Additional
#include "TransformComponentUI.h"

CUIBaseNode::CUIBaseNode()
	: m_IsMouseOnNode(false)
{
}

CUIBaseNode::~CUIBaseNode()
{
}

std::shared_ptr<CUIBaseNode> CUIBaseNode::shared_from_this()
{
    return std::dynamic_pointer_cast<CUIBaseNode>(SceneNodeBase::shared_from_this());
}

std::weak_ptr<CUIBaseNode> CUIBaseNode::weak_from_this()
{
    return std::weak_ptr<CUIBaseNode>(shared_from_this());
}

void CUIBaseNode::RegisterComponents()
{
    SetTransformComponent(AddComponent(std::make_shared<CTransformComponentUI>(shared_from_this())));
}

//
// Size & bounds
//

glm::vec2 CUIBaseNode::GetSize()
{
    return glm::vec2(99999.0f, 999999.0f);
}

BoundingRect CUIBaseNode::GetBoundsAbs()
{
    std::shared_ptr<ITransformComponentUI> transformUI = GetComponent<ITransformComponentUI>();

    BoundingRect boundRect = BoundingRect(transformUI->GetTranslationAbs(), transformUI->GetTranslationAbs() + GetSize() * transformUI->GetScaleAbs());

    //for (auto ch : GetChilds())
    //    boundRect.makeUnion(ch->GetBoundsAbs());

    return boundRect;
}

bool CUIBaseNode::IsPointInBoundsAbs(glm::vec2 Point)
{
    return GetBoundsAbs().isPointInside(Point);
}



//
// Render functional
//
bool CUIBaseNode::Accept(IVisitor* visitor)
{
	bool visitResult = visitor->VisitUI(this);

	if (!visitResult)
		return false;

    // Visit childs
    for (const auto& child : GetChilds())
        child->Accept(visitor);

	// Visit meshes
	AcceptMesh(visitor);

	return visitResult;
}

bool CUIBaseNode::AcceptMesh(IVisitor* visitor)
{
	return false;
}



//
// Input events
//
bool CUIBaseNode::OnKeyPressed(KeyEventArgs & e)
{
	return false;
}

void CUIBaseNode::OnKeyReleased(KeyEventArgs & e)
{
	// Do nothing
}

void CUIBaseNode::OnMouseMoved(MouseMotionEventArgs& e)
{
	// Do nothing
}

bool CUIBaseNode::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    // Raise 'Clicked' callback
    {
        UIBaseNodeClickedEventArgs args(shared_from_this());
        Clicked(args);
    }

	return false;
}

void CUIBaseNode::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	// Do nothing
}

bool CUIBaseNode::OnMouseWheel(MouseWheelEventArgs & e)
{
	return false;
}



//
// Syntetic events
//
void CUIBaseNode::OnMouseEntered()
{
	// Do nothing
}

void CUIBaseNode::OnMouseLeaved()
{
	// Do nothing
}



//
// Syntetic events PRIVATE
//
bool CUIBaseNode::IsMouseOnNode() const
{
	return m_IsMouseOnNode;
}

void CUIBaseNode::DoMouseEntered()
{
	m_IsMouseOnNode = true;
}

void CUIBaseNode::DoMouseLeaved()
{
	m_IsMouseOnNode = false;
}

void CUIBaseNode::SetTransformComponent(std::shared_ptr<ITransformComponentUI> TransformComponent)
{
    m_Components_Transform = TransformComponent;
}
