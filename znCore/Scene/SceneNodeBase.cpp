#include "stdafx.h"

// General
#include "SceneNodeBase.h"

// Additional
#include "Scene/Actions.h"
#include "Scene/Properties.h"

SceneNodeBase::SceneNodeBase()
	: m_Name("SceneNodeBase")

	// Transform functinal
	, m_LocalTransform(1.0f)
	, m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
	, m_WorldTransform(1.0f)
	, m_InverseWorldTransform(1.0f)
{
	m_InverseLocalTransform = glm::inverse(m_LocalTransform);

	m_ActionsGroup = std::make_shared<CActionsGroup>("General");
	m_PropertiesGroup = std::make_shared<CPropertiesGroup>("General", "Some important scene node properties.");

	std::shared_ptr<CPropertyWrapped<std::string>> nameProperty = std::make_shared<CPropertyWrapped<std::string>>("Name", "Scene node name.");
	nameProperty->SetValueSetter(std::bind(&SceneNodeBase::SetName, this, std::placeholders::_1));
	nameProperty->SetValueGetter(std::bind(&SceneNodeBase::GetName, this));
	GetProperties()->AddProperty(nameProperty);
}

SceneNodeBase::~SceneNodeBase()
{
	m_Children.clear();
}

void SceneNodeBase::Initialize()
{
	// Do nothing
}

void SceneNodeBase::Finalize()
{
	// Do nothing
}

std::string SceneNodeBase::GetType() const
{
	return m_Type;
}

void SceneNodeBase::SetType(std::string Type)
{
	m_Type = Type;
}

std::string SceneNodeBase::GetName() const
{
	return m_Name;
}

void SceneNodeBase::SetName(std::string Name)
{
	m_Name = Name;
}



//
// Transform functional
//
mat4 SceneNodeBase::GetLocalTransform() const
{
	return m_LocalTransform;
}

mat4 SceneNodeBase::GetInverseLocalTransform() const
{
	return m_InverseLocalTransform;
}

void SceneNodeBase::SetLocalTransform(cmat4 localTransform)
{
	m_LocalTransform = localTransform;
	m_InverseLocalTransform = glm::inverse(localTransform);

	/*glm::vec3 scale;
	glm::quat rotation;
	glm::vec3 translation;
	glm::vec3 skew;
	glm::vec4 perspective;
	glm::decompose(m_LocalTransform, scale, rotation, translation, skew, perspective);*/

	UpdateWorldTransform();

	// After world updated, we can update all childs
	for (auto it : GetChilds())
		std::dynamic_pointer_cast<SceneNodeBase>(it)->UpdateWorldTransform();
}



// World transform
mat4 SceneNodeBase::GetWorldTransfom() const
{
	return m_WorldTransform;
}

mat4 SceneNodeBase::GetInverseWorldTransform() const
{
	return m_InverseWorldTransform;
}

mat4 SceneNodeBase::GetParentWorldTransform() const
{
	mat4 parentTransform(1.0f);
	if (std::shared_ptr<ISceneNode> parent = GetParent())
	{
		parentTransform = parent->GetWorldTransfom();
	}

	return parentTransform;
}

void SceneNodeBase::SetWorldTransform(cmat4 worldTransform)
{
	mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());

	SetLocalTransform(inverseParentTransform * worldTransform);
}

void SceneNodeBase::ForceRecalculateLocalTransform()
{
	UpdateLocalTransform();
}



//
// Components engine
//
bool SceneNodeBase::IsComponentExists(GUID ComponentID)
{
	const auto& components = GetComponents();
    return components.find(ComponentID) != components.end();
}

std::shared_ptr<ISceneNodeComponent> SceneNodeBase::GetComponent(GUID ComponentID)
{
	const auto& components = GetComponents();
	const auto& component = components.find(ComponentID);
    if (component == components.end())
        return std::shared_ptr<ISceneNodeComponent>();

    return component->second;
}

std::shared_ptr<ISceneNodeComponent> SceneNodeBase::AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component)
{
    m_Components[ComponentID] = Component;
    return Component;
}

const ComponentsMap& SceneNodeBase::GetComponents() const
{
    return m_Components;
}

void SceneNodeBase::RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message)
{
	const auto& components = GetComponents();
	std::for_each(components.begin(), components.end(), [&Component, &Message](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& ComponentMapIter) 
	{
		ComponentMapIter.second->OnMessage(Component, Message);
	});
}

void SceneNodeBase::RegisterComponents()
{
}




//
// Scene access
//
std::shared_ptr<IScene> SceneNodeBase::GetScene() const
{
    return m_Scene.lock();
}



//
// Childs functional
//
void SceneNodeBase::AddChild(std::shared_ptr<ISceneNode> childNode)
{
	if (childNode == nullptr)
		_ASSERT_EXPR(false, L"Child node must not be NULL.");

	NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
	if (iter == m_Children.end())
	{
		std::dynamic_pointer_cast<ISceneNodeInternal>(childNode)->SetParentInternal(weak_from_this());

		m_Children.push_back(childNode);
		if (!childNode->GetName().empty())
			m_ChildrenByName.insert(NodeNameMap::value_type(childNode->GetName(), childNode));

		GetScene()->RaiseSceneChangeEvent(ESceneChangeType::NodeAdded, shared_from_this(), childNode);
	}
}

void SceneNodeBase::RemoveChild(std::shared_ptr<ISceneNode> childNode)
{
	if (childNode)
	{
		_ASSERT_EXPR(false, L"Child node must not be NULL.");
	}

	NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
	if (iter != m_Children.end())
	{
		std::dynamic_pointer_cast<ISceneNodeInternal>(childNode)->SetParentInternal(std::weak_ptr<ISceneNode>());

		m_Children.erase(iter);
		NodeNameMap::iterator iter2 = m_ChildrenByName.find(childNode->GetName());
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

void SceneNodeBase::SetParent(std::weak_ptr<ISceneNode> parentNode)
{
	// Remove from current parent
	std::shared_ptr<ISceneNode> currentParent = m_ParentNode.lock();
	if (currentParent != nullptr)
	{
		currentParent->RemoveChild(shared_from_this());
		m_ParentNode.reset();
	}

	// Add to new parent
	if (std::shared_ptr<ISceneNode> newParent = parentNode.lock())
		newParent->AddChild(SceneNodeBase::shared_from_this());
}

std::shared_ptr<ISceneNode> SceneNodeBase::GetParent() const
{
	return m_ParentNode.lock();
}

NodeList SceneNodeBase::GetChilds()
{
	return m_Children;
}

void SceneNodeBase::OnUpdate(UpdateEventArgs & e)
{
	DoUpdate(e);

	const auto& components = GetComponents();
	std::for_each(components.begin(), components.end(), [&e](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& Component)
	{
		_ASSERT(Component.second);
		Component.second->DoUpdate(e);
	});

	const auto& childs = GetChilds();
	std::for_each(childs.begin(), childs.end(), [&e](const std::shared_ptr<ISceneNode>& Child)
	{
		Child->OnUpdate(e);
	});
}

void SceneNodeBase::DoUpdate(UpdateEventArgs & e)
{
	// Do nothing...
}

void SceneNodeBase::UpdateCamera(const ICamera* camera)
{
	// Do nothing...
}

void SceneNodeBase::UpdateViewport(const Viewport * viewport)
{
	// Do nothing...
}

bool SceneNodeBase::Load(std::shared_ptr<IXMLReader> Reader)
{
	return false;
}

bool SceneNodeBase::Save(std::shared_ptr<IXMLWriter> Writer)
{
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

bool SceneNodeBase::Accept(IVisitor* visitor)
{
	bool visitResult = visitor->VisitBase(this);

	// ROOT NODE DON'T HAVE COMPONENTS!!!

	const auto& childs = GetChilds();
	std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<ISceneNode>& Child)
	{
		Child->Accept(visitor);
	});

	return visitResult;
}


std::shared_ptr<IActionsGroup> SceneNodeBase::GetActions() const
{
	return std::shared_ptr<IActionsGroup>();
}

std::shared_ptr<IPropertiesGroup> SceneNodeBase::GetProperties() const
{
	return m_PropertiesGroup;
}



//
// ISceneNodeInternal
//
void SceneNodeBase::SetScene(std::shared_ptr<IScene> Scene)
{
	m_Scene = Scene;
}

void SceneNodeBase::SetParentInternal(std::weak_ptr<ISceneNode> parentNode)
{
	m_ParentNode = parentNode;
	RaiseOnParentChanged();
}



void SceneNodeBase::UpdateLocalTransform()
{
}

void SceneNodeBase::UpdateWorldTransform()
{
}

//
// Protected
//
IBaseManager* SceneNodeBase::GetBaseManager() const
{
	return std::dynamic_pointer_cast<IBaseManagerHolder>(GetScene())->GetBaseManager();
}

void SceneNodeBase::RaiseOnParentChanged()
{
	// Don't forget about update world transform
	UpdateWorldTransform();

    for (auto c : m_Components)
    {
        c.second->OnParentChanged();
    }
}
