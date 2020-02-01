#include "stdafx.h"

// General
#include "SceneNode3D.h"

// Additional
#include "Scene/Actions.h"
#include "Scene/Properties.h"

#include "MeshComponent3D.h"
#include "ColliderComponent3D.h"
#include "LightComponent3D.h"

SceneNode3D::SceneNode3D()
	: m_Translate(0.0f)
	, m_Rotate(vec3(0.0f))
	, m_RotateQuat(quat())
	, m_IsRotateQuat(false)
	, m_Scale(1.0f)

	// Transform functinal
	, m_LocalTransform(1.0f)
	, m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
	, m_WorldTransform(1.0f)
	, m_InverseWorldTransform(1.0f)
{
	SetName("SceneNode3D");

	m_InverseLocalTransform = glm::inverse(m_LocalTransform);

	m_ActionsGroup = std::make_shared<CActionsGroup>("General");
	m_PropertiesGroup = std::make_shared<CPropertiesGroup>("General", "Some important scene node properties.");

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

	
}

SceneNode3D::~SceneNode3D()
{}



//
// ISceneNode3D
//
void SceneNode3D::Initialize()
{
}

void SceneNode3D::Finalize()
{
}

void SceneNode3D::SetType(std::string Type)
{
	m_Type = Type;
}

std::string SceneNode3D::GetType() const
{
	return m_Type;
}

void SceneNode3D::SetName(std::string Name)
{
	m_Name = Name;
}

std::string SceneNode3D::GetName() const
{
	return m_Name;
}

//
// Childs functional
//
void SceneNode3D::AddChild(std::shared_ptr<ISceneNode3D> childNode)
{
	if (childNode == nullptr)
		_ASSERT_EXPR(false, L"Child node must not be NULL.");

	Node3DList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
	if (iter == m_Children.end())
	{
		std::dynamic_pointer_cast<SceneNode3D>(childNode)->SetParentInternal(weak_from_this());

		m_Children.push_back(childNode);
		if (!childNode->GetName().empty())
			m_ChildrenByName.insert(Node3DNameMap::value_type(childNode->GetName(), childNode));

		GetScene()->RaiseSceneChangeEvent(ESceneChangeType::NodeAdded, shared_from_this(), childNode);
	}
}

void SceneNode3D::RemoveChild(std::shared_ptr<ISceneNode3D> childNode)
{
	if (childNode)
	{
		_ASSERT_EXPR(false, L"Child node must not be NULL.");
	}

	Node3DList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
	if (iter != m_Children.end())
	{
		std::dynamic_pointer_cast<SceneNode3D>(childNode)->SetParentInternal(std::weak_ptr<ISceneNode3D>());

		m_Children.erase(iter);
		Node3DNameMap::iterator iter2 = m_ChildrenByName.find(childNode->GetName());
		if (iter2 != m_ChildrenByName.end())
			m_ChildrenByName.erase(iter2);

		GetScene()->RaiseSceneChangeEvent(ESceneChangeType::NodeRemoved, shared_from_this(), childNode);
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

void SceneNode3D::SetParent(ISceneNode3D* parentNode)
{
	// Remove from current parent
	std::shared_ptr<ISceneNode3D> currentParent = m_ParentNode.lock();
	if (currentParent != nullptr)
	{
		currentParent->RemoveChild(shared_from_this());
		m_ParentNode.reset();
	}

	// Add to new parent
	if (parentNode != nullptr)
		parentNode->AddChild(shared_from_this());
}

ISceneNode3D* SceneNode3D::GetParent() const
{
	return m_ParentNode.lock().get();
}

const SceneNode3D::Node3DList& SceneNode3D::GetChilds()
{
	return m_Children;
}

void SceneNode3D::RaiseOnParentChanged()
{
	// Don't forget about update world transform
	UpdateWorldTransform();

	for (auto c : m_Components)
	{
		c.second->OnParentChanged();
	}
}

IActionsGroup * SceneNode3D::GetActions() const
{
	return m_ActionsGroup.get();
}

IPropertiesGroup * SceneNode3D::GetProperties() const
{
	return m_PropertiesGroup.get();
}

IScene * SceneNode3D::GetScene() const
{
	return m_Scene.lock().get();
}




//
// Transform functional
//
void SceneNode3D::SetTranslate(cvec3 _translate)
{
	m_Translate = _translate;
	UpdateLocalTransform();
}

void SceneNode3D::AddTranslate(vec3 Translate)
{
	SetTranslate(GetTranslation() + Translate);
}

cvec3 SceneNode3D::GetTranslation() const
{
	return m_Translate;
}

void SceneNode3D::SetRotation(cvec3 _rotate)
{
	m_Rotate = _rotate;
	UpdateLocalTransform();
}

cvec3 SceneNode3D::GetRotation() const
{
	return m_Rotate;
}

void SceneNode3D::SetRotationQuaternion(cquat _rotate)
{
	m_RotateQuat = _rotate;
	m_IsRotateQuat = true;
	UpdateLocalTransform();
}

cquat SceneNode3D::GetRotationQuaternion() const
{
	return m_RotateQuat;
}

void SceneNode3D::SetScale(cvec3 _scale)
{
	m_Scale = _scale;
	UpdateLocalTransform();
}

cvec3 SceneNode3D::GetScale() const
{
	return m_Scale;
}

mat4 SceneNode3D::GetLocalTransform() const
{
	return m_LocalTransform;
}

mat4 SceneNode3D::GetInverseLocalTransform() const
{
	return m_InverseLocalTransform;
}

void SceneNode3D::SetLocalTransform(cmat4 localTransform)
{
	m_LocalTransform = localTransform;
	m_InverseLocalTransform = glm::inverse(localTransform);

	UpdateWorldTransform();

	// After world updated, we can update all childs
	for (auto it : GetChilds())
		std::dynamic_pointer_cast<SceneNode3D>(it)->UpdateWorldTransform();
}

mat4 SceneNode3D::GetWorldTransfom() const
{
	return m_WorldTransform;
}

mat4 SceneNode3D::GetInverseWorldTransform() const
{
	return m_InverseWorldTransform;
}

glm::mat4 SceneNode3D::GetParentWorldTransform() const
{
	glm::mat4 parentTransform(1.0f);
	if (ISceneNode3D* parent = GetParent())
	{
		parentTransform = parent->GetWorldTransfom();
	}

	return parentTransform;
}

void SceneNode3D::SetWorldTransform(cmat4 worldTransform)
{
	mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());

	SetLocalTransform(inverseParentTransform * worldTransform);
}

void SceneNode3D::ForceRecalculateLocalTransform()
{
	UpdateLocalTransform();
}


//
// Components engine
//
bool SceneNode3D::IsComponentExists(GUID ComponentID) const
{
	const auto& components = GetComponents();
	return components.find(ComponentID) != components.end();
}

ISceneNodeComponent* SceneNode3D::GetComponent(GUID ComponentID) const
{
	const auto& components = GetComponents();
	const auto& component = components.find(ComponentID);
	if (component == components.end())
		return nullptr;

	return component->second.get();
}

ISceneNodeComponent* SceneNode3D::AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component)
{
	m_Components[ComponentID] = Component;
	return Component.get();
}

const ComponentsMap& SceneNode3D::GetComponents() const
{
	return m_Components;
}

void SceneNode3D::RaiseComponentMessage(ISceneNodeComponent* Component, ComponentMessageType Message) const
{
	const auto& components = GetComponents();
	std::for_each(components.begin(), components.end(), [&Component, &Message](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& ComponentMapIter)
	{
		ComponentMapIter.second->OnMessage(Component, Message);
	});
}
void SceneNode3D::RegisterComponents()
{
	ISceneNode3D::AddComponent<CLightComponent3D>(std::make_shared<CLightComponent3D>(this));
    SetMeshComponent(ISceneNode3D::AddComponent(std::make_shared<CMeshComponent3D>(this)));
    SetColliderComponent(ISceneNode3D::AddComponent(std::make_shared<CColliderComponent3D>(this)));
}



//
// Others
//



void SceneNode3D::OnUpdate(UpdateEventArgs & e)
{
	DoUpdate(e);

	const auto& components = GetComponents();
	std::for_each(components.begin(), components.end(), [&e](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& Component)
	{
		_ASSERT(Component.second);
		Component.second->DoUpdate(e);
	});

	const auto& childs = GetChilds();
	std::for_each(childs.begin(), childs.end(), [&e](const std::shared_ptr<ISceneNode3D>& Child)
	{
		Child->OnUpdate(e);
	});
}

void SceneNode3D::DoUpdate(UpdateEventArgs & e)
{
	// Do nothing...
}

void SceneNode3D::UpdateCamera(const ICameraComponent3D* camera)
{
	// Do nothing...
}

void SceneNode3D::UpdateViewport(const Viewport& viewport)
{
	// Do nothing...
}

bool SceneNode3D::Load(std::shared_ptr<IXMLReader> Reader)
{
	return false;
}

bool SceneNode3D::Save(std::shared_ptr<IXMLWriter> Writer)
{
	/*

	std::shared_ptr<IXMLWriter> componentWriter = Writer->CreateChild("TransformComponent3D");

	std::shared_ptr<IXMLWriter> translate = componentWriter->CreateChild("Translate");
	translate->AddAttribute("X", std::to_string(GetTranslation().x));
	translate->AddAttribute("Y", std::to_string(GetTranslation().y));
	translate->AddAttribute("Z", std::to_string(GetTranslation().z));

	std::shared_ptr<IXMLWriter> rotate = componentWriter->CreateChild("Rotate");
	rotate->AddAttribute("X", std::to_string(GetRotation().x));
	rotate->AddAttribute("Y", std::to_string(GetRotation().y));
	rotate->AddAttribute("Z", std::to_string(GetRotation().z));

	std::shared_ptr<IXMLWriter> scale = componentWriter->CreateChild("Scale");
	scale->AddAttribute("X", std::to_string(GetScale().x));
	scale->AddAttribute("Y", std::to_string(GetScale().y));
	scale->AddAttribute("Z", std::to_string(GetScale().z));
	
	*/

	std::shared_ptr<IXMLWriter> thisSceneNodeWriter = Writer->CreateChild("SceneNode");
	thisSceneNodeWriter->AddAttribute("Name", GetName());

	if (GetComponents().size() > 0)
	{
		std::shared_ptr<IXMLWriter> componentsWriter = thisSceneNodeWriter->CreateChild("Components");
		for (const auto& component : GetComponents())
			component.second->Save(componentsWriter);
	}

	if (GetChilds().size() > 0)
	{
		std::shared_ptr<IXMLWriter> childsWriter = thisSceneNodeWriter->CreateChild("Childs");
		for (const auto& child : GetChilds())
			child->Save(childsWriter);
	}

	return true;
}

bool SceneNode3D::Accept(IVisitor* visitor)
{
	bool visitResult = visitor->Visit(this);

	//if (visitResult && !GetComponent<IColliderComponent3D>()->CheckFrustum(((AbstractPass*)visitor)->GetRenderEventArgs()->Camera))
	//{
	//	return false;
	//}

	if (visitResult)
	{
		const auto& components = GetComponents();
		std::for_each(components.begin(), components.end(), [&visitor](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& Component)
		{
			Component.second->Accept(visitor);
		});
	}

	// Now visit children
	const auto& childs = GetChilds();
	std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<ISceneNode3D>& Child)
	{
#ifdef LOADER_ENABLED
		std::shared_ptr<ILoadable> loadable = std::dynamic_pointer_cast<ILoadable>(child);
		if (loadable != nullptr && !loadable->isLoaded())
			continue;
#endif
		Child->Accept(visitor);
	});

	return visitResult;
}



//
// Public
//
void SceneNode3D::SetScene(std::weak_ptr<IScene> Scene)
{
	m_Scene = Scene;
}

void SceneNode3D::SetParentInternal(std::weak_ptr<ISceneNode3D> parentNode)
{
	m_ParentNode = parentNode;
	RaiseOnParentChanged();
}



//
// Protected
//
void SceneNode3D::UpdateLocalTransform()
{
	glm::mat4 localTransform = glm::mat4(1.0f);

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

	SetLocalTransform(localTransform);

	RaiseComponentMessage(nullptr, UUID_OnTransformChanged);
}

void SceneNode3D::UpdateWorldTransform()
{
	m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
	m_InverseWorldTransform = glm::inverse(m_WorldTransform);
	
	RaiseComponentMessage(nullptr, UUID_OnTransformChanged);
}

IBaseManager* SceneNode3D::GetBaseManager() const
{
	return dynamic_cast<IBaseManagerHolder*>(GetScene())->GetBaseManager();
}

void SceneNode3D::SetMeshComponent(IMeshComponent3D* MeshComponent)
{
	m_Components_Mesh = MeshComponent;
}

void SceneNode3D::SetColliderComponent(IColliderComponent3D* ColliderComponent)
{
    m_Components_Collider = ColliderComponent;
}
