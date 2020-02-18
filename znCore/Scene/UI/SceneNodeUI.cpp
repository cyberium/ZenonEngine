#include "stdafx.h"

// General
#include "SceneNodeUI.h"

// Additional
#include "Scene/Actions.h"
#include "Scene/Properties.h"

CUIBaseNode::CUIBaseNode()
	: m_IsMouseOnNode(false)
	, m_Translate(vec2())
	, m_Rotate(vec3())
	, m_Scale(1.0f, 1.0f)

	// Transform functinal
	, m_LocalTransform(1.0f)
	, m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
	, m_WorldTransform(1.0f)
	, m_InverseWorldTransform(1.0f)
{
	SetName("CUIBaseNode");

	m_InverseLocalTransform = glm::inverse(m_LocalTransform);

	m_ActionsGroup = std::make_shared<CActionsGroup>("General");
	m_PropertiesGroup = std::make_shared<CPropertiesGroup>("General", "Some important scene node properties.");

	// Name properties
	{
		std::shared_ptr<CPropertyWrapped<std::string>> nameProperty = std::make_shared<CPropertyWrapped<std::string>>("Name", "Scene node name.");
		nameProperty->SetValueSetter(std::bind(&CUIBaseNode::SetName, this, std::placeholders::_1));
		nameProperty->SetValueGetter(std::bind(&CUIBaseNode::GetName, this));
		GetProperties()->AddProperty(nameProperty);
	}
}

CUIBaseNode::~CUIBaseNode()
{
}





//
// ISceneNodeUI
//
void CUIBaseNode::Initialize()
{

}

void CUIBaseNode::Finalize()
{

}



//
// Name & Type
//
std::string CUIBaseNode::GetType() const
{
	return m_Type;
}

void CUIBaseNode::SetType(std::string Type)
{
	m_Type = Type;
}

std::string CUIBaseNode::GetName() const
{
	return m_Name;
}

void CUIBaseNode::SetName(std::string Name)
{
	m_Name = Name;
}



//
// Childs functional
//
void CUIBaseNode::AddChild(std::shared_ptr<ISceneNodeUI> childNode)
{
	if (childNode == nullptr)
		_ASSERT_EXPR(false, L"Child node must not be NULL.");

	NodeUIList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
	if (iter == m_Children.end())
	{
		std::dynamic_pointer_cast<CUIBaseNode>(childNode)->m_ParentNode = weak_from_this();

		m_Children.push_back(childNode);
		if (!childNode->GetName().empty())
			m_ChildrenByName.insert(NodeUINameMap::value_type(childNode->GetName(), childNode));

		//GetScene()->RaiseSceneChangeEvent(ESceneChangeType::NodeAdded, shared_from_this(), childNode);
	}
}

void CUIBaseNode::RemoveChild(std::shared_ptr<ISceneNodeUI> childNode)
{
	if (childNode)
	{
		_ASSERT_EXPR(false, L"Child node must not be NULL.");
	}

	NodeUIList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
	if (iter != m_Children.end())
	{
		std::dynamic_pointer_cast<CUIBaseNode>(childNode)->m_ParentNode = std::weak_ptr<ISceneNodeUI>();

		m_Children.erase(iter);
		NodeUINameMap::iterator iter2 = m_ChildrenByName.find(childNode->GetName());
		if (iter2 != m_ChildrenByName.end())
			m_ChildrenByName.erase(iter2);

		//GetScene()->RaiseSceneChangeEvent(ESceneChangeType::NodeRemoved, shared_from_this(), childNode);
	}
	else
	{
		// Maybe this node appears lower in the hierarchy...
		for (auto child : m_Children)
		{
			child->RemoveChild(childNode);
		}
	}
}

void CUIBaseNode::SetParent(ISceneNodeUI* parentNode)
{
	// Remove from current parent
	std::shared_ptr<ISceneNodeUI> currentParent = m_ParentNode.lock();
	if (currentParent != nullptr)
	{
		currentParent->RemoveChild(shared_from_this());
		m_ParentNode.reset();
	}

	// Add to new parent
	if (ISceneNodeUI* newParent = parentNode)
		newParent->AddChild(shared_from_this());
}

ISceneNodeUI* CUIBaseNode::GetParent() const
{
	return m_ParentNode.lock().get();
}

const CUIBaseNode::NodeUIList& CUIBaseNode::GetChilds()
{
	return m_Children;
}



//
// Actions & Properties
//
IActionsGroup * CUIBaseNode::GetActions() const
{
	return m_ActionsGroup.get();
}

IPropertiesGroup * CUIBaseNode::GetProperties() const
{
	return m_PropertiesGroup.get();
}

IScene * CUIBaseNode::GetScene() const
{
	return m_Scene.lock().get();
}

void CUIBaseNode::SetTranslate(cvec2 _translate)
{
	m_Translate = _translate;

	UpdateLocalTransform();
}
cvec2 CUIBaseNode::GetTranslation() const
{
	return m_Translate;
}
glm::vec2 CUIBaseNode::GetTranslationAbs() const
{
	glm::vec2 parentTranslate = glm::vec2(0.0f, 0.0f);
	if (ISceneNodeUI* parent = GetParent())
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
	if (ISceneNodeUI* parent = GetParent())
		parentScale = parent->GetScaleAbs();
	return parentScale * GetScale();
}

mat4 CUIBaseNode::GetLocalTransform() const
{
	return m_LocalTransform;
}

mat4 CUIBaseNode::GetWorldTransfom() const
{
	return m_WorldTransform;
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
void CUIBaseNode::Accept(IVisitor* visitor)
{
	if (visitor->Visit(this))
	{
		AcceptMesh(visitor);
	}

	const auto& childs = GetChilds();
	std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<ISceneNodeUI>& Child) {
		Child->Accept(visitor);
	});
}

void CUIBaseNode::AcceptMesh(IVisitor* visitor)
{

}



//
// ISceneNodeUIWithWrapper
//
void CUIBaseNode::SetWrapper(const ISceneNodeUI * WrapperNode)
{
	m_WrapperNode = WrapperNode;
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
// Public
//
void CUIBaseNode::SetScene(std::weak_ptr<IScene> Scene)
{
	m_Scene = Scene;
}

void CUIBaseNode::SetParentInternal(std::weak_ptr<ISceneNodeUI> parentNode)
{
	m_ParentNode = parentNode;
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

	// Don't forget to update world transform
	UpdateWorldTransform();
}

void CUIBaseNode::UpdateWorldTransform()
{
	glm::mat4 parentTransform(1.0f);
	if (CUIBaseNode* parent = dynamic_cast<CUIBaseNode*>(GetParent()))
	{
		parentTransform = parent->m_WorldTransform;
	}

	m_WorldTransform = parentTransform * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);
}

IBaseManager& CUIBaseNode::GetBaseManager() const
{
	return dynamic_cast<IBaseManagerHolder*>(GetScene())->GetBaseManager();
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
