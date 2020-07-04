#include "stdafx.h"

// General
#include "SceneNode3D.h"

// Additional
#include "Scene/Actions.h"
#include "Scene/Properties.h"

#include "ModelsComponent3D.h"
#include "ColliderComponent3D.h"
#include "LightComponent3D.h"

SceneNode3D::SceneNode3D()
	: m_Translate(0.0f)
	, m_Rotate(glm::vec3(0.0f))
	, m_RotateQuat(glm::quat())
	, m_IsRotateQuat(false)
	, m_Scale(1.0f)

	// Transform functinal
	, m_LocalTransform(1.0f)
	, m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
	, m_WorldTransform(1.0f)
	, m_InverseWorldTransform(1.0f)
{
	SetClassName("SceneNode3D");

	m_PropertiesGroup = std::make_shared<CPropertiesGroup>("SceneNodeProperties", "Some important scene node 3d properties.");
}

SceneNode3D::~SceneNode3D()
{}



//
// ISceneNode3D
//
void SceneNode3D::Initialize()
{
	// Name properties
	{
		std::shared_ptr<CPropertyWrapped<std::string>> nameProperty = std::make_shared<CPropertyWrapped<std::string>>("Name", "Scene node name.");
		nameProperty->SetValueSetter(std::bind(&SceneNode3D::SetName, this, std::placeholders::_1));
		nameProperty->SetValueGetter(std::bind(&SceneNode3D::GetName, this));
		GetProperties()->AddProperty(nameProperty);
	}

	// Transform properties
	{
		std::shared_ptr<IPropertiesGroup> propertiesGroup = std::make_shared<CPropertiesGroup>("Transform", "Transorm of this 3D node. Like translation, rotation and scale.");

		std::shared_ptr<CPropertyWrapped<glm::vec3>> translateProperty = std::make_shared<CPropertyWrapped<glm::vec3>>("Translate", "Position of this node in world. Relative to parent.");
		translateProperty->SetValueSetter(std::bind(&SceneNode3D::SetTranslate, this, std::placeholders::_1));
		translateProperty->SetValueGetter(std::bind(&SceneNode3D::GetTranslation, this));
		propertiesGroup->AddProperty(translateProperty);

		std::shared_ptr<CPropertyWrapped<glm::vec3>> rotationProperty = std::make_shared<CPropertyWrapped<glm::vec3>>("Rotate", "Rotation of this node. Relative to parent.");
		rotationProperty->SetValueSetter(std::bind(&SceneNode3D::SetRotation, this, std::placeholders::_1));
		rotationProperty->SetValueGetter(std::bind(&SceneNode3D::GetRotation, this));
		propertiesGroup->AddProperty(rotationProperty);

		std::shared_ptr<CPropertyWrapped<glm::vec3>> scaleProperty = std::make_shared<CPropertyWrapped<glm::vec3>>("Scale", "Scale of this node. Relative to parent.");
		scaleProperty->SetValueSetter(std::bind(&SceneNode3D::SetScale, this, std::placeholders::_1));
		scaleProperty->SetValueGetter(std::bind(&SceneNode3D::GetScale, this));
		propertiesGroup->AddProperty(scaleProperty);

		GetProperties()->AddProperty(propertiesGroup);
	}

	// Actions
	{
		std::shared_ptr<CAction> removeAction = std::make_shared<CAction>("Remove", "Remove this node from world. this action affected on childs!");
		removeAction->SetAction([this]() -> bool {
			auto sceneNode = this->shared_from_this();
			if (sceneNode->GetScene() == nullptr)
				return false;

			sceneNode->GetScene()->RemoveChild(sceneNode->GetParent().lock(), sceneNode);
			return true;
		});

		GetProperties()->AddProperty(removeAction);
	}
}

void SceneNode3D::Finalize()
{}


//
// Childs functional
//
void SceneNode3D::AddChild(const std::shared_ptr<ISceneNode3D>& childNode)
{
	if (childNode == nullptr)
		throw CException(L"SceneNode3D: Child node must not be NULL.");

	// 1. Удаляем чилда у текущего родителя (возможно нужно его об этом нотифицировать, например для перерасчета BoundingBox)
	if (!childNode->GetParent().expired())
	{
		if (auto currentChildParent = childNode->GetParent().lock())
		{
			if (currentChildParent != shared_from_this())
			{
				std::dynamic_pointer_cast<SceneNode3D>(currentChildParent)->RemoveChildInternal(childNode);
				//Log::Warn("SceneNode3D: Failed to add child to his current parent.");
				//return;
			}
		}
	}

	// 2. Добавляем чилда в нового парента (возможно нужно его об этом нотифицировать, например для перерасчета BoundingBox)
	this->AddChildInternal(childNode);
}

void SceneNode3D::RemoveChild(const std::shared_ptr<ISceneNode3D>& childNode)
{
	if (childNode == nullptr)
	{
		Log::Warn("SceneNode3D: Child node must not be NULL.");
		return;
	}

	this->RemoveChildInternal(childNode);
}

std::weak_ptr<ISceneNode3D> SceneNode3D::GetParent() const
{
	return m_ParentNode;
}

const SceneNode3D::Node3DList& SceneNode3D::GetChilds()
{
	return m_Children;
}

void SceneNode3D::ClearChilds()
{
}

void SceneNode3D::RaiseOnParentChanged()
{
	// Don't forget about update world transform
	UpdateWorldTransform();

	for (auto c : m_Components)
	{
		c.second->OnMessage(nullptr, UUID_OnParentChanged);
	}
}

std::shared_ptr<IPropertiesGroup> SceneNode3D::GetProperties() const
{
	return m_PropertiesGroup;
}

IScene * SceneNode3D::GetScene() const
{
	return m_Scene.lock().get();
}



//
// Transform functional
//
void SceneNode3D::SetTranslate(const glm::vec3& _translate)
{
	m_Translate = _translate;
	UpdateLocalTransform();
}

void SceneNode3D::AddTranslate(const glm::vec3& Translate)
{
	SetTranslate(GetTranslation() + Translate);
}

const glm::vec3& SceneNode3D::GetTranslation() const
{
	return m_Translate;
}

void SceneNode3D::SetRotation(const glm::vec3& _rotate)
{
	m_Rotate = _rotate;
	UpdateLocalTransform();
}

const glm::vec3& SceneNode3D::GetRotation() const
{
	return m_Rotate;
}

void SceneNode3D::SetRotationQuaternion(const glm::quat& _rotate)
{
	m_RotateQuat = _rotate;
	m_IsRotateQuat = true;
	UpdateLocalTransform();
}

const glm::quat& SceneNode3D::GetRotationQuaternion() const
{
	return m_RotateQuat;
}

void SceneNode3D::SetScale(const glm::vec3& _scale)
{
	m_Scale = _scale;
	UpdateLocalTransform();
}

const glm::vec3& SceneNode3D::GetScale() const
{
	return m_Scale;
}

glm::mat4 SceneNode3D::GetLocalTransform() const
{
	return m_LocalTransform;
}

glm::mat4 SceneNode3D::GetInverseLocalTransform() const
{
	return m_InverseLocalTransform;
}

void SceneNode3D::SetLocalTransform(const glm::mat4& localTransform)
{
	m_LocalTransform = localTransform;
	m_InverseLocalTransform = glm::inverse(localTransform);

	UpdateWorldTransform();
}

glm::mat4 SceneNode3D::GetWorldTransfom() const
{
	return m_WorldTransform;
}

glm::mat4 SceneNode3D::GetInverseWorldTransform() const
{
	return m_InverseWorldTransform;
}

glm::mat4 SceneNode3D::GetParentWorldTransform() const
{
	glm::mat4 parentTransform(1.0f);
	if (auto parent = GetParent().lock())
		parentTransform = parent->GetWorldTransfom();

	return parentTransform;
}

void SceneNode3D::SetWorldTransform(const glm::mat4& worldTransform)
{
	glm::mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());

	SetLocalTransform(inverseParentTransform * worldTransform);
}



//
// Components engine
//
bool SceneNode3D::IsComponentExists(GUID ComponentID) const
{
	const auto& components = GetComponents();
	return components.find(ComponentID) != components.end();
}

std::shared_ptr<ISceneNodeComponent> SceneNode3D::GetComponent(GUID ComponentID) const
{
	const auto& components = GetComponents();
	const auto& component = components.find(ComponentID);
	if (component == components.end())
		return nullptr;

	return component->second;
}

std::shared_ptr<ISceneNodeComponent> SceneNode3D::AddComponent(GUID ComponentID, const std::shared_ptr<ISceneNodeComponent>& Component)
{
	m_Components[ComponentID] = Component;
	return Component;
}

const ComponentsMap& SceneNode3D::GetComponents() const
{
	return m_Components;
}

void SceneNode3D::RaiseComponentMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) const
{
	const auto& components = GetComponents();
	std::for_each(components.begin(), components.end(), [&Component, &Message](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& ComponentMapIter) {
		ComponentMapIter.second->OnMessage(Component, Message);
	});
}
void SceneNode3D::RegisterComponents()
{
	m_Components_Models = AddComponent(std::make_shared<CModelsComponent3D>(*this));
	m_Components_Collider = AddComponent(std::make_shared<CColliderComponent3D>(*this));
}

const std::shared_ptr<IColliderComponent3D>& SceneNode3D::GetColliderComponent() const
{
	return m_Components_Collider;
}

const std::shared_ptr<IModelsComponent3D>& SceneNode3D::GetModelsComponent() const
{
	return m_Components_Models;
}



//
// Others
//
void SceneNode3D::Update(const UpdateEventArgs & e)
{
	// Do nothing...
}

void SceneNode3D::Accept(IVisitor* visitor)
{
	//if (const auto& loadableObject = dynamic_cast<ILoadable*>(this))
	//	if (loadableObject->GetState() != ILoadable::ELoadableState::Loaded)
	//		return;
	
	EVisitResult visitResult = visitor->Visit(this);

	if (visitResult & EVisitResult::AllowVisitContent)
	{
		const auto& components = GetComponents();
		std::for_each(components.begin(), components.end(), [&visitor](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& Component) {
			if (Component.second)
				Component.second->Accept(visitor);
		});
	}

	if (visitResult & EVisitResult::AllowVisitChilds)
	{
		const auto& childs = GetChilds();
		std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<ISceneNode3D>& Child) {
			if (Child != nullptr)
				Child->Accept(visitor);
		});
	}
}



//
// ISceneNode3DInternal
//
void SceneNode3D::SetSceneInternal(const std::weak_ptr<IScene>& Scene)
{
	m_Scene = Scene;
}

void SceneNode3D::AddChildInternal(const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	_ASSERT(ChildNode != nullptr);

	const auto& iter = std::find(m_Children.begin(), m_Children.end(), ChildNode);
	if (iter != m_Children.end())
		throw CException(L"This parent already has this child.");

	// Add to common list
	m_Children.push_back(ChildNode);

	// And add child to named list
	if (!ChildNode->GetName().empty())
		m_ChildrenByName.insert(Node3DNameMap::value_type(ChildNode->GetName(), ChildNode));

	std::dynamic_pointer_cast<SceneNode3D>(ChildNode)->SetParentInternal(weak_from_this());

	// TODO: Какой ивент посылать первым?
	ChildNode->RaiseOnParentChanged();
	GetScene()->RaiseSceneChangeEvent(ESceneChangeType::NodeAddedToParent, shared_from_this(), ChildNode);
}

void SceneNode3D::RemoveChildInternal(const std::shared_ptr<ISceneNode3D>& ChildNode)
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

	std::dynamic_pointer_cast<SceneNode3D>(ChildNode)->SetParentInternal(std::weak_ptr<ISceneNode3D>());

	// TODO: Какой ивент посылать первым?
	ChildNode->RaiseOnParentChanged();
	GetScene()->RaiseSceneChangeEvent(ESceneChangeType::NodeRemovedFromParent, shared_from_this(), ChildNode);
}

void SceneNode3D::SetParentInternal(const std::weak_ptr<ISceneNode3D>& parentNode)
{
	m_ParentNode = parentNode;
}



glm::mat4 SceneNode3D::CalculateLocalTransform() const
{
	glm::mat4 localTransform(1.0f);

	localTransform = glm::translate(localTransform, m_Translate);
	if (m_IsRotateQuat)
	{
		localTransform *= glm::toMat4(m_RotateQuat);
	}
	else
	{
		localTransform = glm::rotate(localTransform, m_Rotate.x, glm::vec3(1, 0, 0));
		localTransform = glm::rotate(localTransform, m_Rotate.y, glm::vec3(0, 1, 0));
		localTransform = glm::rotate(localTransform, m_Rotate.z, glm::vec3(0, 0, 1));
	}
	localTransform = glm::scale(localTransform, m_Scale);

	return localTransform;
}

//
// Protected
//
void SceneNode3D::UpdateLocalTransform()
{
	SetLocalTransform(CalculateLocalTransform());
	RaiseComponentMessage(nullptr, UUID_OnLocalTransformChanged);
}

void SceneNode3D::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);
	
	// After world updated, we can update all childs
	for (const auto& it : GetChilds())
		std::dynamic_pointer_cast<SceneNode3D>(it)->UpdateWorldTransform();

	RaiseComponentMessage(nullptr, UUID_OnWorldTransformChanged);
}

IBaseManager& SceneNode3D::GetBaseManager() const
{
	return dynamic_cast<IBaseManagerHolder*>(GetScene())->GetBaseManager();
}
