#include "stdafx.h"

// General
#include "SceneNodeUI.h"

// Additional
#include "Scene/Actions.h"
#include "Scene/Properties.h"

SceneNodeUI::SceneNodeUI()
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
	SetClassName("SceneNodeUI");

	m_PropertiesGroup = std::make_shared<CPropertiesGroup>("SceneNodeProperties", "Some important scene node UI properties.");

	// Name properties
	{
		std::shared_ptr<CPropertyWrapped<std::string>> nameProperty = std::make_shared<CPropertyWrapped<std::string>>("Name", "Scene node name.");
		nameProperty->SetValueSetter(std::bind(&Object::SetName, this, std::placeholders::_1));
		nameProperty->SetValueGetter(std::bind(&Object::GetName, this));
		GetProperties()->AddProperty(nameProperty);
	}
}

SceneNodeUI::~SceneNodeUI()
{
}





//
// ISceneNodeUI
//
void SceneNodeUI::Initialize()
{

}

void SceneNodeUI::Finalize()
{

}



//
// Childs functional
//
void SceneNodeUI::AddChild(const std::shared_ptr<ISceneNodeUI>& childNode)
{
	if (childNode == nullptr)
		throw CException(L"SceneNodeUI: Child node must not be NULL.");

	// 1. Удаляем чилда у текущего родителя (возможно нужно его об этом нотифицировать, например для перерасчета BoundingBox)
	if (auto currentChildParent = childNode->GetParent().lock())
	{
		if (currentChildParent == shared_from_this())
		{
			Log::Warn("SceneNodeUI: Failed to add child to his current parent.");
			return;
		}

		std::dynamic_pointer_cast<SceneNodeUI>(currentChildParent)->RemoveChildInternal(childNode);
	}

	// 2. Добавляем чилда в нового парента (возможно нужно его об этом нотифицировать, например для перерасчета BoundingBox)
	this->AddChildInternal(childNode);

}

void SceneNodeUI::RemoveChild(const std::shared_ptr<ISceneNodeUI>& childNode)
{
	if (childNode == nullptr)
	{
		Log::Warn("SceneNodeUI: Child node must not be NULL.");
		return;
	}

	this->RemoveChildInternal(childNode);
}

std::weak_ptr<ISceneNodeUI> SceneNodeUI::GetParent() const
{
	return m_ParentNode;
}

const SceneNodeUI::NodeUIList& SceneNodeUI::GetChilds()
{
	return m_Children;
}

void SceneNodeUI::RaiseOnParentChanged()
{
	UpdateWorldTransform();
}



//
// Actions & Properties
//

std::shared_ptr<IPropertiesGroup> SceneNodeUI::GetProperties() const
{
	return m_PropertiesGroup;
}

IScene * SceneNodeUI::GetScene() const
{
	return m_Scene.lock().get();
}

void SceneNodeUI::SetTranslate(const glm::vec2& _translate)
{
	m_Translate = _translate;
	UpdateLocalTransform();
}
const glm::vec2& SceneNodeUI::GetTranslation() const
{
	return m_Translate;
}
glm::vec2 SceneNodeUI::GetTranslationAbs() const
{
	glm::vec2 parentTranslate = glm::vec2(0.0f, 0.0f);
	if (auto parent = GetParent().lock())
		parentTranslate = parent->GetTranslationAbs();
	return parentTranslate + GetTranslation();
}

void SceneNodeUI::SetRotation(const glm::vec3& _rotate)
{
	m_Rotate = _rotate;

	UpdateLocalTransform();
}
const glm::vec3& SceneNodeUI::GetRotation() const
{
	return m_Rotate;
}

void SceneNodeUI::SetScale(const glm::vec2& _scale)
{
	m_Scale = _scale;

	UpdateLocalTransform();
}
const glm::vec2& SceneNodeUI::GetScale() const
{
	return m_Scale;
}
glm::vec2 SceneNodeUI::GetScaleAbs() const
{
	glm::vec2 parentScale = glm::vec2(1.0f);
	if (auto parent = GetParent().lock())
		parentScale = parent->GetScaleAbs();
	return parentScale * GetScale();
}

glm::mat4 SceneNodeUI::GetLocalTransform() const
{
	return m_LocalTransform;
}

glm::mat4 SceneNodeUI::GetWorldTransfom() const
{
	return m_WorldTransform;
}



//
// Size & bounds
//

glm::vec2 SceneNodeUI::GetSize() const
{
    return glm::vec2(99999.0f, 999999.0f);
}

BoundingRect SceneNodeUI::GetBoundsAbs()
{
    BoundingRect boundRect = BoundingRect(GetTranslationAbs(), GetTranslationAbs() + GetSize() * GetScaleAbs());

    //for (auto ch : GetChilds())
    //    boundRect.makeUnion(ch->GetBoundsAbs());

    return boundRect;
}

bool SceneNodeUI::IsPointInBoundsAbs(const glm::vec2& Point)
{
    return GetBoundsAbs().isPointInside(Point);
}



//
// Render functional
//
void SceneNodeUI::Accept(IVisitor* visitor)
{
	EVisitResult visitResult = visitor->Visit(this);
	
	if (visitResult & EVisitResult::AllowVisitContent)
	{
		AcceptMesh(visitor);
	}

	if (visitResult & EVisitResult::AllowVisitChilds)
	{
		const auto& childs = GetChilds();
		std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<ISceneNodeUI>& Child) {
			Child->Accept(visitor);
		});
	}
}

void SceneNodeUI::AcceptMesh(IVisitor* visitor)
{

}



//
// UI events
//
void SceneNodeUI::SetOnClickCallback(std::function<void(const ISceneNodeUI* Node, glm::vec2)> OnClickCallback)
{
	m_OnClickCallback = OnClickCallback;
}



//
// Input events
//
bool SceneNodeUI::OnKeyPressed(KeyEventArgs & e)
{
	return false;
}

void SceneNodeUI::OnKeyReleased(KeyEventArgs & e)
{
	// Do nothing
}

void SceneNodeUI::OnMouseMoved(MouseMotionEventArgs& e)
{
	// Do nothing
}

bool SceneNodeUI::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	// Raise event
	if (m_OnClickCallback)
	{
		m_OnClickCallback(this, glm::vec2(e.X, e.Y) - GetTranslationAbs());
		return true;
	}

	return false;
}

void SceneNodeUI::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	// Do nothing
}

bool SceneNodeUI::OnMouseWheel(MouseWheelEventArgs & e)
{
	return false;
}



//
// Syntetic events
//
void SceneNodeUI::OnMouseEntered()
{
	// Do nothing
}

void SceneNodeUI::OnMouseLeaved()
{
	// Do nothing
}



//
// Private
//
void SceneNodeUI::SetSceneInternal(const std::weak_ptr<IScene>& Scene)
{
	m_Scene = Scene;
}

void SceneNodeUI::AddChildInternal(const std::shared_ptr<ISceneNodeUI>& ChildNode)
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

	std::dynamic_pointer_cast<SceneNodeUI>(ChildNode)->SetParentInternal(weak_from_this());

	ChildNode->RaiseOnParentChanged();
}

void SceneNodeUI::RemoveChildInternal(const std::shared_ptr<ISceneNodeUI>& ChildNode)
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

	std::dynamic_pointer_cast<SceneNodeUI>(ChildNode)->SetParentInternal(std::weak_ptr<ISceneNodeUI>());

	ChildNode->RaiseOnParentChanged();
}

void SceneNodeUI::SetParentInternal(const std::weak_ptr<ISceneNodeUI>& parentNode)
{
	m_ParentNode = parentNode;
}



//
// Protected
//
void SceneNodeUI::UpdateLocalTransform()
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

void SceneNodeUI::UpdateWorldTransform()
{
	glm::mat4 parentTransform(1.0f);
	if (auto parent = GetParent().lock())
		parentTransform = parent->GetWorldTransfom();

	m_WorldTransform = parentTransform * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);

	// After world updated, we can update all childs
	for (const auto& it : GetChilds())
		std::dynamic_pointer_cast<SceneNodeUI>(it)->UpdateWorldTransform();
}

IBaseManager& SceneNodeUI::GetBaseManager() const
{
	return dynamic_cast<IBaseManagerHolder*>(GetScene())->GetBaseManager();
}



//
// Syntetic events PRIVATE
//
bool SceneNodeUI::IsMouseOnNode() const
{
	return m_IsMouseOnNode;
}

void SceneNodeUI::DoMouseEntered()
{
	m_IsMouseOnNode = true;
}

void SceneNodeUI::DoMouseLeaved()
{
	m_IsMouseOnNode = false;
}
