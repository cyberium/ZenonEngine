#include "stdafx.h"

// General
#include "UIControl.h"

CUIControl::CUIControl()
	: m_IsMouseOnNode(false)
	, m_Translate(glm::vec2(0.0f))
	, m_Rotate(glm::vec3(0.0f))
	, m_Scale(1.0f, 1.0f)

	// Transform functinal
	, m_LocalTransform(1.0f)
	, m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
	, m_WorldTransform(1.0f)
	, m_InverseWorldTransform(1.0f)
{
	m_PropertiesGroup = MakeShared(CPropertiesGroup, "SceneNodeProperties", "Some important scene node UI properties.");

	// Name properties
	{
		std::shared_ptr<CPropertyWrapped<std::string>> nameProperty = MakeShared(CPropertyWrapped<std::string>, "Name", "Scene node name.");
		nameProperty->SetValueSetter(std::bind(&Object::SetName, this, std::placeholders::_1));
		nameProperty->SetValueGetter(std::bind(&Object::GetName, this));
		GetProperties()->AddProperty(nameProperty);
	}
}

CUIControl::~CUIControl()
{
}





//
// IUIControl
//
void CUIControl::Initialize()
{

}

void CUIControl::Finalize()
{

}



//
// Childs functional
//
void CUIControl::AddChild(const std::shared_ptr<IUIControl>& childNode)
{
	if (childNode == nullptr)
		throw CException(L"CUIControl: Child node must not be NULL.");

	// 1. Удаляем чилда у текущего родителя (возможно нужно его об этом нотифицировать, например для перерасчета BoundingBox)
	if (auto currentChildParent = childNode->GetParent().lock())
	{
		if (currentChildParent == shared_from_this())
		{
			Log::Warn("CUIControl: Failed to add child to his current parent.");
			return;
		}

		std::dynamic_pointer_cast<CUIControl>(currentChildParent)->RemoveChildInternal(childNode);
	}

	// 2. Добавляем чилда в нового парента (возможно нужно его об этом нотифицировать, например для перерасчета BoundingBox)
	this->AddChildInternal(childNode);

}

void CUIControl::RemoveChild(const std::shared_ptr<IUIControl>& childNode)
{
	if (childNode == nullptr)
	{
		Log::Warn("CUIControl: Child node must not be NULL.");
		return;
	}

	this->RemoveChildInternal(childNode);
}

std::weak_ptr<IUIControl> CUIControl::GetParent() const
{
	return m_ParentNode;
}

const CUIControl::NodeUIList& CUIControl::GetChilds()
{
	return m_Children;
}

void CUIControl::RaiseOnParentChanged()
{
	UpdateWorldTransform();
}



//
// Actions & Properties
//

std::shared_ptr<IPropertiesGroup> CUIControl::GetProperties() const
{
	return m_PropertiesGroup;
}

IScene& CUIControl::GetScene() const
{
	return *m_Scene;
}

void CUIControl::SetTranslate(const glm::vec2& _translate)
{
	m_Translate = _translate;
	UpdateLocalTransform();
}
const glm::vec2& CUIControl::GetTranslation() const
{
	return m_Translate;
}
glm::vec2 CUIControl::GetTranslationAbs() const
{
	glm::vec2 parentTranslate = glm::vec2(0.0f, 0.0f);
	if (auto parent = GetParent().lock())
		parentTranslate = parent->GetTranslationAbs();
	return parentTranslate + GetTranslation();
}

void CUIControl::SetRotation(const glm::vec3& _rotate)
{
	m_Rotate = _rotate;

	UpdateLocalTransform();
}
const glm::vec3& CUIControl::GetRotation() const
{
	return m_Rotate;
}

void CUIControl::SetScale(const glm::vec2& _scale)
{
	m_Scale = _scale;

	UpdateLocalTransform();
}
const glm::vec2& CUIControl::GetScale() const
{
	return m_Scale;
}
glm::vec2 CUIControl::GetScaleAbs() const
{
	glm::vec2 parentScale = glm::vec2(1.0f);
	if (auto parent = GetParent().lock())
		parentScale = parent->GetScaleAbs();
	return parentScale * GetScale();
}

glm::mat4 CUIControl::GetLocalTransform() const
{
	return m_LocalTransform;
}

glm::mat4 CUIControl::GetWorldTransfom() const
{
	return m_WorldTransform;
}



//
// Size & bounds
//

glm::vec2 CUIControl::GetSize() const
{
    return glm::vec2(99999.0f, 999999.0f);
}

BoundingRect CUIControl::GetBoundsAbs()
{
    BoundingRect boundRect = BoundingRect(GetTranslationAbs(), GetTranslationAbs() + GetSize() * GetScaleAbs());

    //for (auto ch : GetChilds())
    //    boundRect.makeUnion(ch->GetBoundsAbs());

    return boundRect;
}

bool CUIControl::IsPointInBoundsAbs(const glm::vec2& Point)
{
    return GetBoundsAbs().isPointInside(Point);
}



//
// Render functional
//
void CUIControl::Accept(IVisitor* visitor)
{
	EVisitResult visitResult = visitor->Visit(this);
	
	if (visitResult & EVisitResult::AllowVisitContent)
	{
		AcceptMesh(visitor);
	}

	if (visitResult & EVisitResult::AllowVisitChilds)
	{
		const auto& childs = GetChilds();
		std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<IUIControl>& Child) {
			Child->Accept(visitor);
		});
	}
}

void CUIControl::AcceptMesh(IVisitor* visitor)
{

}



//
// UI events
//
void CUIControl::SetOnClickCallback(std::function<void(const IUIControl* Node, glm::vec2)> OnClickCallback)
{
	m_OnClickCallback = OnClickCallback;
}



//
// Input events
//
bool CUIControl::OnKeyPressed(KeyEventArgs & e)
{
	return false;
}

void CUIControl::OnKeyReleased(KeyEventArgs & e)
{
	// Do nothing
}

void CUIControl::OnMouseMoved(MouseMotionEventArgs& e)
{
	// Do nothing
}

bool CUIControl::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	// Raise event
	if (m_OnClickCallback)
	{
		m_OnClickCallback(this, glm::vec2(e.X, e.Y) - GetTranslationAbs());
		return true;
	}

	return false;
}

void CUIControl::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	// Do nothing
}

bool CUIControl::OnMouseWheel(MouseWheelEventArgs & e)
{
	return false;
}



//
// Syntetic events
//
void CUIControl::OnMouseEntered()
{
	// Do nothing
}

void CUIControl::OnMouseLeaved()
{
	// Do nothing
}



//
// Private
//
void CUIControl::SetSceneInternal(IScene* Scene)
{
	m_Scene = Scene;
}

void CUIControl::AddChildInternal(const std::shared_ptr<IUIControl>& ChildNode)
{
	_ASSERT(ChildNode != nullptr);

	const auto& iter = std::find(m_Children.begin(), m_Children.end(), ChildNode);
	if (iter != m_Children.end())
		throw CException(L"This parent already has this child.");

	// Add to common list
	m_Children.push_back(ChildNode);

	// And add child to named list
	if (!ChildNode->GetName().empty())
		m_ChildrenByName.insert(NodeUINameMap::value_type(ChildNode->GetName(), ChildNode));

	std::dynamic_pointer_cast<CUIControl>(ChildNode)->SetParentInternal(weak_from_this());

	ChildNode->RaiseOnParentChanged();
}

void CUIControl::RemoveChildInternal(const std::shared_ptr<IUIControl>& ChildNode)
{
	const auto& childListIter = std::find(m_Children.begin(), m_Children.end(), ChildNode);
	if (childListIter == m_Children.end())
		//throw CException(L"Can't remove child because don't found.");
		return;

	// Delete from list
	m_Children.erase(childListIter);

	// TODO: Если единственная ссылка на чилда осталась в списке чилдов, то всё взорвется?

	// Delete from name map
	const auto& childNameMapIter = m_ChildrenByName.find(ChildNode->GetName());
	if (childNameMapIter != m_ChildrenByName.end())
		m_ChildrenByName.erase(childNameMapIter);

	std::dynamic_pointer_cast<CUIControl>(ChildNode)->SetParentInternal(std::weak_ptr<IUIControl>());

	ChildNode->RaiseOnParentChanged();
}

void CUIControl::SetParentInternal(const std::weak_ptr<IUIControl>& parentNode)
{
	m_ParentNode = parentNode;
}



//
// Protected
//
void CUIControl::UpdateLocalTransform()
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

void CUIControl::UpdateWorldTransform()
{
	glm::mat4 parentTransform(1.0f);
	if (auto parent = GetParent().lock())
		parentTransform = parent->GetWorldTransfom();

	m_WorldTransform = parentTransform * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);

	// After world updated, we can update all childs
	for (const auto& it : GetChilds())
		std::dynamic_pointer_cast<CUIControl>(it)->UpdateWorldTransform();
}

IBaseManager& CUIControl::GetBaseManager() const
{
	return GetScene().GetBaseManager();
}



//
// Syntetic events PRIVATE
//
bool CUIControl::IsMouseOnNode() const
{
	return m_IsMouseOnNode;
}

void CUIControl::DoMouseEntered()
{
	m_IsMouseOnNode = true;
}

void CUIControl::DoMouseLeaved()
{
	m_IsMouseOnNode = false;
}
