#include "stdafx.h"

// General
#include "UIBaseNode.h"

// Additional
#include "UIWindow.h"
#include "Application.h"

CUIBaseNode::CUIBaseNode()
	: m_Name("CUIBaseNode")
	, m_Translate(vec2())
	, m_Rotate(vec3())
	, m_Scale(1.0f, 1.0f)
	, m_IsMouseOnNode(false)
{
}

CUIBaseNode::~CUIBaseNode()
{
}

cstring CUIBaseNode::GetName() const
{
	return m_Name;
}

void CUIBaseNode::SetName(cstring name)
{
	m_Name = name;
}


//
// Translate
//
void CUIBaseNode::SetTranslate(cvec2 _translate) 
{ 
	m_Translate = _translate;

	UpdateLocalTransform();

    // Raise 'Moved' callback
    {
        UIBaseNodeMovedEventArgs args;
        Moved(args);

        for (auto ch : GetChilds())
        {
            ch->Moved(args);
        }
    }
}

glm::vec2 CUIBaseNode::GetTranslation() const
{ 
	return m_Translate; 
}

glm::vec2 CUIBaseNode::GetTranslationAbs() const
{
	glm::vec2 parentTranslate = vec2(0.0f, 0.0f);
	if (std::shared_ptr<CUIBaseNode> parent = m_pParentNode.lock())
		parentTranslate = parent->GetTranslationAbs();
	return parentTranslate + GetTranslation();
}


//
// Rotate
//
void CUIBaseNode::SetRotation(cvec3 _rotate) 
{ 
	m_Rotate = _rotate;

	UpdateLocalTransform();
}
glm::vec3 CUIBaseNode::GetRotation() const
{ 
	return m_Rotate; 
}


//
// Scale
//
void CUIBaseNode::SetScale(cvec2 _scale) 
{ 
	m_Scale = _scale;

	UpdateLocalTransform();
}

glm::vec2 CUIBaseNode::GetScale() const
{ 
	return m_Scale; 
}

glm::vec2 CUIBaseNode::GetScaleAbs() const
{
    glm::vec2 parentScale = vec2(1.0f, 1.0f);
    if (std::shared_ptr<CUIBaseNode> parent = m_pParentNode.lock())
        parentScale = parent->GetScaleAbs();
    return parentScale * GetScale();
}


//
// Size & bounds
//

glm::vec2 CUIBaseNode::GetSize() const
{
    return glm::vec2(99999.0f, 999999.0f);
}

BoundingRect CUIBaseNode::GetBoundsAbs() const
{
    BoundingRect boundRect = BoundingRect(GetTranslationAbs(), GetTranslationAbs() + GetSize() * GetScaleAbs());

    for (auto ch : GetChilds())
        boundRect.makeUnion(ch->GetBoundsAbs());

    return boundRect;
}

bool CUIBaseNode::IsPointInBoundsAbs(glm::vec2 Point) const
{
    return GetBoundsAbs().isPointInside(Point);
}

// Local transform

mat4 CUIBaseNode::GetLocalTransform() const
{
	return m_LocalTransform;
}

void CUIBaseNode::SetLocalTransform(cmat4 localTransform)
{
	m_LocalTransform = localTransform;
}

// World transform

mat4 CUIBaseNode::GetWorldTransfom() const
{
	return GetParentWorldTransform() * m_LocalTransform;
}

void CUIBaseNode::SetWorldTransform(cmat4 worldTransform)
{
	mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());
	SetLocalTransform(inverseParentTransform * worldTransform);
}



//
// Parent & childs functional
//
void CUIBaseNode::SetParent(std::weak_ptr<CUIBaseNode> parentNode)
{
	std::shared_ptr<CUIBaseNode> me = shared_from_this();

	// Delete from current parent
	if (std::shared_ptr<CUIBaseNode> currentParent = m_pParentNode.lock())
	{
		if (std::shared_ptr<CUIWindowNode> currentParentAsWindow = std::dynamic_pointer_cast<CUIWindowNode, CUIBaseNode>(currentParent))
		{
			currentParentAsWindow->RemoveChild(me);
			m_pParentNode.reset();
		}
	}

	if (std::shared_ptr<CUIBaseNode> parent = parentNode.lock())
	{
		std::shared_ptr<CUIWindowNode> parentAsWindow = std::dynamic_pointer_cast<CUIWindowNode, CUIBaseNode>(parent);
		if (parentAsWindow)
		{
			parentAsWindow->AddChild(me);
		}
		else
		{
			fail2("Only 'CUIWindowNode' can be parent of any ui node.");
		}
	}
}

void CUIBaseNode::SetParentInternal(std::weak_ptr<CUIBaseNode> parent)
{
	m_pParentNode = parent;
}

std::shared_ptr<CUIBaseNode> CUIBaseNode::GetParent() const
{
    return m_pParentNode.lock();
}

std::vector<std::shared_ptr<CUIBaseNode>> CUIBaseNode::GetChilds() const
{
    return std::vector<std::shared_ptr<CUIBaseNode>>();
}


mat4 CUIBaseNode::GetParentWorldTransform() const
{
	if (std::shared_ptr<CUIBaseNode> parent = m_pParentNode.lock())
	{
		return parent->GetWorldTransfom();
	}

	return mat4(1.0f);
}

//

void CUIBaseNode::UpdateLocalTransform()
{
	m_LocalTransform = mat4();

	m_LocalTransform = glm::translate(m_LocalTransform, vec3(m_Translate, 0.0f));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.x, vec3(1, 0, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.y, vec3(0, 1, 0));
	m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.z, vec3(0, 0, 1));
	m_LocalTransform = glm::scale(m_LocalTransform, vec3(m_Scale, 1.0f));
}

void CUIBaseNode::UpdateViewport(const Viewport* viewport)
{
	// Do nothing
}



//
// Render functional
//
bool CUIBaseNode::Accept(IVisitor& visitor)
{
	bool visitResult = visitor.Visit(*this);
	if (!visitResult)
		return false;

    // Visit childs
    for (auto child : GetChilds())
        child->Accept(visitor);

	// Visit meshes
	AcceptMesh(visitor);

	return visitResult;
}

bool CUIBaseNode::AcceptMesh(IVisitor & visitor)
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