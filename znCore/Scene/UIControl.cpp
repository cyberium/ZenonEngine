#include "stdafx.h"

// General
#include "UIControl.h"

CUIControl::CUIControl(IScene& Scene)
	: Object(Scene.GetBaseManager())
	, m_Scene(Scene)

	, m_IsMouseOnNode(false)

	, m_Position(glm::vec2(0.0f))
	, m_Rotate(glm::vec3(0.0f))
	, m_Scale(glm::vec2(1.0f))
	, m_Size(glm::vec2(10.0f))

	, m_LocalTransform(1.0f)
	, m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
	, m_WorldTransform(1.0f)
	, m_InverseWorldTransform(1.0f)
{
	m_PropertiesGroup = MakeShared(CPropertiesGroup, "SceneNodeProperties", "Some important scene node UI properties.");


}

CUIControl::~CUIControl()
{
}





//
// IUIControl
//
void CUIControl::Initialize()
{
	// Name properties
	{
		std::shared_ptr<CPropertyWrapped<std::string>> nameProperty = MakeShared(CPropertyWrapped<std::string>, "Name", "Scene node name.", "");
		nameProperty->SetValueSetter(std::bind(&Object::SetName, this, std::placeholders::_1));
		nameProperty->SetValueGetter(std::bind(&Object::GetName, this));
		GetProperties()->AddProperty(nameProperty);
	}
}

void CUIControl::Finalize()
{

}



//
// Childs functional
//
void CUIControl::AddChild(const std::shared_ptr<IUIControl>& childNode)
{
	AddChildInternal(childNode);
	//dynamic_cast<ISceneInternal&>(GetScene()).AddChildInternal(shared_from_this(), childNode);
}

void CUIControl::RemoveChild(const std::shared_ptr<IUIControl>& childNode)
{
	RemoveChildInternal(childNode);
	//dynamic_cast<ISceneInternal&>(GetScene()).RemoveChildInternal(shared_from_this(), childNode);
}

void CUIControl::MakeMeOrphan()
{
	if (auto parent = GetParent())
		std::dynamic_pointer_cast<IUIControlInternal>(parent)->RemoveChildInternal(shared_from_this());
}

std::shared_ptr<IUIControl> CUIControl::GetParent() const
{
	return m_ParentNode.lock();
}

const CUIControl::ControlsList& CUIControl::GetChilds() const
{
	return m_Children;
}

std::shared_ptr<IUIControl> CUIControl::GetChild(std::string Name) const
{
	std::string currClearName = /*GetClearName(*/Name/*).first*/;
	for (const auto& ch : GetChilds())
	{
		std::string childClearName = /*GetClearName(*/ch->GetName()/*).first*/;
		if (childClearName == currClearName)
			return ch;
	}
	return nullptr;
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
	return m_Scene;
}

void CUIControl::SetPosition(glm::vec2 Position)
{
	throw CException("Not implemented!");
}

glm::vec2 CUIControl::GetPosition() const
{
	if (auto parent = GetParent())
		return parent->GetPosition() + GetLocalPosition() * parent->GetScaleAbs();
	return GetLocalPosition();
}

void CUIControl::SetLocalPosition(glm::vec2 Position)
{
	m_Position = Position;
	UpdateLocalTransform();
}

glm::vec2 CUIControl::GetLocalPosition() const
{
	return m_Position;
}

void CUIControl::SetRotation(glm::vec3 Rotation)
{
	m_Rotate = Rotation;
	UpdateLocalTransform();
}

glm::vec3 CUIControl::GetRotation() const
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
	if (auto parent = GetParent())
		return parent->GetScaleAbs() * GetScale();
	return GetScale();
}

glm::mat4 CUIControl::GetLocalTransform() const
{
	return m_LocalTransform;
}

glm::mat4 CUIControl::GetWorldTransfom() const
{
	return m_WorldTransform;
}

glm::mat4 CUIControl::GetParentWorldTransform() const
{
	if (auto parent = GetParent())
		return parent->GetWorldTransfom();
	return glm::mat4(1.0f);;
}



//
// Size & bounds
//

glm::vec2 CUIControl::GetSize() const
{
    return m_Size;
}

BoundingRect CUIControl::GetBoundsAbs()
{
    BoundingRect boundRect = BoundingRect(GetPosition(), GetPosition() + GetSize() * GetScaleAbs());

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
	EVisitResult visitResult = visitor->Visit(shared_from_this());
	
	//if (visitResult & EVisitResult::AllowVisitContent)
	//{
	//	AcceptMesh(visitor);
	//}

	if (visitResult & EVisitResult::AllowVisitChilds)
	{
		const auto& childs = GetChilds();
		std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<IUIControl>& Child) {
			Child->Accept(visitor);
		});
	}
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
		m_OnClickCallback(this, glm::vec2(e.X, e.Y) - GetPosition());
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
// IUIControlInternal
//
void CUIControl::AddChildInternal(std::shared_ptr<IUIControl> ChildNode)
{
	if (ChildNode == nullptr)
		throw CException(L"Unable to add nullptr child node to '%s'.", GetName().c_str());

	if (auto currentChildParent = ChildNode->GetParent())
	{
		if (currentChildParent != shared_from_this())
		{
			std::dynamic_pointer_cast<CUIControl>(currentChildParent)->RemoveChildPrivate(ChildNode);
		}
	}

	this->AddChildPrivate(ChildNode);
}

void CUIControl::RemoveChildInternal(std::shared_ptr<IUIControl> ChildNode)
{
	if (ChildNode == nullptr)
		throw CException("Unable to remove nullptr node from '%s'.", GetName().c_str());

	this->RemoveChildPrivate(ChildNode);
}

void CUIControl::RaiseOnParentChangedInternal()
{
	UpdateWorldTransform();
}



//
// Protected
//
IBaseManager& CUIControl::GetBaseManager() const
{
	return GetScene().GetBaseManager();
}

IRenderDevice & CUIControl::GetRenderDevice() const
{
	return GetScene().GetRenderDevice();
}

void CUIControl::SetSize(glm::vec2 Size)
{
	m_Size = Size;
}

glm::mat4 CUIControl::CalculateLocalTransform() const
{
	glm::mat4 localTransform(1.0f);
	localTransform = glm::translate(localTransform, glm::vec3(m_Position, 0.0f));
	localTransform = glm::rotate(localTransform, m_Rotate.x, glm::vec3(1, 0, 0));
	localTransform = glm::rotate(localTransform, m_Rotate.y, glm::vec3(0, 1, 0));
	localTransform = glm::rotate(localTransform, m_Rotate.z, glm::vec3(0, 0, 1));
	localTransform = glm::scale(localTransform, glm::vec3(m_Scale, 1.0f));
	return localTransform;
}

void CUIControl::UpdateLocalTransform()
{
	m_LocalTransform = CalculateLocalTransform();
	m_InverseLocalTransform = glm::inverse(m_LocalTransform);

	UpdateWorldTransform();
}

void CUIControl::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);

	// After world updated, we can update all childs
	for (const auto& it : GetChilds())
		std::dynamic_pointer_cast<CUIControl>(it)->UpdateWorldTransform();
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



//
// Private
//
void CUIControl::AddChildPrivate(std::shared_ptr<IUIControl> ChildNode)
{
	_ASSERT(ChildNode != nullptr);

	const auto& iter = std::find(m_Children.begin(), m_Children.end(), ChildNode);
	if (iter != m_Children.end())
		throw CException(L"This parent already has this child.");

	m_Children.push_back(ChildNode);

	std::dynamic_pointer_cast<CUIControl>(ChildNode)->SetParentPrivate(weak_from_this());
	std::dynamic_pointer_cast<IUIControlInternal>(ChildNode)->RaiseOnParentChangedInternal();
}

void CUIControl::RemoveChildPrivate(std::shared_ptr<IUIControl> ChildNode)
{
	const auto& childListIter = std::find(m_Children.begin(), m_Children.end(), ChildNode);
	if (childListIter == m_Children.end())
		throw CException(L"Can't remove child because don't found.");

	m_Children.erase(childListIter);

	std::dynamic_pointer_cast<CUIControl>(ChildNode)->SetParentPrivate(std::weak_ptr<IUIControl>());
	std::dynamic_pointer_cast<IUIControlInternal>(ChildNode)->RaiseOnParentChangedInternal();
}

void CUIControl::SetParentPrivate(std::weak_ptr<IUIControl> parentNode)
{
	m_ParentNode = parentNode;
}
