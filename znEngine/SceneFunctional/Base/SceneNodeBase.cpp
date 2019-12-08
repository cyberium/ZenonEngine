#include "stdafx.h"

// General
#include "SceneNodeBase.h"

// Additional
#include "Loader.h"

SceneNodeBase::SceneNodeBase()
	: m_Name("SceneNodeBase")
{
}

SceneNodeBase::~SceneNodeBase()
{
	m_Children.clear();
}

void SceneNodeBase::Initialize()
{
	
}

void SceneNodeBase::Finalize()
{
	
}

const std::string& SceneNodeBase::GetName() const
{
	return m_Name;
}

void SceneNodeBase::SetName(const std::string& name)
{
	m_Name = name;
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
void SceneNodeBase::SetScene(std::shared_ptr<IScene> Scene)
{
    m_Scene = Scene;
}

std::shared_ptr<IScene> SceneNodeBase::GetScene() const
{
    return m_Scene.lock();
}



//
// Childs functional
//
void SceneNodeBase::AddChild(std::shared_ptr<ISceneNode> childNode)
{
	if (childNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
		if (iter == m_Children.end())
		{
			std::dynamic_pointer_cast<ISceneNodeInternal>(childNode)->SetParentInternal(weak_from_this());
            RaiseOnParentChanged();

			m_Children.push_back(childNode);
			if (!childNode->GetName().empty())
				m_ChildrenByName.insert(NodeNameMap::value_type(childNode->GetName(), childNode));
		}
	}
}

void SceneNodeBase::RemoveChild(std::shared_ptr<ISceneNode> childNode)
{
	if (childNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
		if (iter != m_Children.end())
		{
			std::dynamic_pointer_cast<ISceneNodeInternal>(childNode)->SetParentInternal(std::weak_ptr<ISceneNode>());
            RaiseOnParentChanged();

			m_Children.erase(iter);
			NodeNameMap::iterator iter2 = m_ChildrenByName.find(childNode->GetName());
			if (iter2 != m_ChildrenByName.end())
				m_ChildrenByName.erase(iter2);
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

	RaiseOnParentChanged();
}

std::shared_ptr<ISceneNode> SceneNodeBase::GetParent() const
{
	return m_ParentNode.lock();
}

NodeList SceneNodeBase::GetChilds()
{
	return m_Children;
}


void SceneNodeBase::UpdateCamera(const ICamera* camera)
{
	// Do nothing...
}

void SceneNodeBase::UpdateViewport(const Viewport * viewport)
{
	// Do nothing...
}

bool SceneNodeBase::Accept(IVisitor* visitor)
{
	bool visitResult = visitor->Visit(this);

	// ROOT NODE DON'T HAVE COMPONENTS!!!

	const auto& childs = GetChilds();
	std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<ISceneNode>& Child)
	{
		Child->Accept(visitor);
	});

	return visitResult;
}

void SceneNodeBase::OnUpdate(UpdateEventArgs & e)
{
}



//
// ISceneNodeInternal
//
void SceneNodeBase::SetParentInternal(std::weak_ptr<ISceneNode> parentNode)
{
	m_ParentNode = parentNode;
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
    for (auto c : m_Components)
    {
        c.second->OnParentChanged();
    }
}
