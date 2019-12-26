#include "stdafx.h"

// General
#include "SceneNodeUI.h"


CUIBaseNode::CUIBaseNode()
	: m_IsMouseOnNode(false)
	, m_Translate(vec2())
	, m_Rotate(vec3())
	, m_Scale(1.0f, 1.0f)
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



//
// ISceneNodeUI
//
void CUIBaseNode::SetTranslate(cvec2 _translate)
{
	m_Translate = _translate;

	UpdateLocalTransform();

	//RaiseComponentMessage(UUID_TransformComponent_OnTranslateChanged);
}
cvec2 CUIBaseNode::GetTranslation() const
{
	return m_Translate;
}
glm::vec2 CUIBaseNode::GetTranslationAbs() const
{
	glm::vec2 parentTranslate = glm::vec2(0.0f, 0.0f);
	if (std::shared_ptr<ISceneNodeUI> parent = std::dynamic_pointer_cast<ISceneNodeUI>(GetParent()))
		parentTranslate = parent->GetTranslationAbs();
	return parentTranslate + GetTranslation();
}

void CUIBaseNode::SetRotation(cvec3 _rotate)
{
	m_Rotate = _rotate;

	UpdateLocalTransform();
}
cvec3 CUIBaseNode::GetRotation() const
{
	return m_Rotate;
}

void CUIBaseNode::SetScale(cvec2 _scale)
{
	m_Scale = _scale;

	UpdateLocalTransform();
}
cvec2 CUIBaseNode::GetScale() const
{
	return m_Scale;
}
glm::vec2 CUIBaseNode::GetScaleAbs() const
{
	glm::vec2 parentScale = vec2(1.0f, 1.0f);
	if (std::shared_ptr<ISceneNodeUI> parent = std::dynamic_pointer_cast<ISceneNodeUI>(GetParent()))
		parentScale = parent->GetScaleAbs();
	return parentScale * GetScale();
}


void CUIBaseNode::RegisterComponents()
{

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
    BoundingRect boundRect = BoundingRect(GetTranslationAbs(), GetTranslationAbs() + GetSize() * GetScaleAbs());

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
// Protected
//
void CUIBaseNode::UpdateLocalTransform()
{
	m_LocalTransform = glm::mat4(1.0f);

	m_LocalTransform = glm::translate(m_LocalTransform, glm::vec3(m_Translate, 0.0f));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.x, glm::vec3(1, 0, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.y, glm::vec3(0, 1, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.z, glm::vec3(0, 0, 1));
	m_LocalTransform = glm::scale(m_LocalTransform, glm::vec3(m_Scale, 1.0f));
	m_InverseLocalTransform = glm::inverse(m_LocalTransform);

	//RaiseComponentMessage(UUID_TransformComponent_OnLocalTransformChanged);

	// Don't forget to update world transform
	UpdateWorldTransform();
}

void CUIBaseNode::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);

	//RaiseComponentMessage(UUID_TransformComponent_OnWorldTransformChanged);
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
