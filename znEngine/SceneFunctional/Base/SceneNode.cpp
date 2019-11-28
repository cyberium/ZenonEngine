#include "stdafx.h"

// Include
#include "Scene.h"

// General
#include "SceneNode.h"

// Additional
#include "Loader.h"

SceneNode::SceneNode()
	: m_Name("SceneNode")
{
}

SceneNode::~SceneNode()
{
	m_Children.clear();
}

void SceneNode::Initialize()
{
}

void SceneNode::Finalize()
{
}

const std::shared_ptr<IBaseManager> SceneNode::GetBaseManager() const
{
	return GetScene()->GetBaseManager();
}

const std::string& SceneNode::GetName() const
{
	return m_Name;
}

void SceneNode::SetName(const std::string& name)
{
	m_Name = name;
}



//
// Components engine
//
bool SceneNode::IsComponentExists(GUID ComponentID)
{
	const auto& components = GetComponents();
    return components.find(ComponentID) != components.end();
}

std::shared_ptr<ISceneNodeComponent> SceneNode::GetComponent(GUID ComponentID)
{
	const auto& components = GetComponents();
	const auto& component = components.find(ComponentID);
    if (component == components.end())
        return std::shared_ptr<ISceneNodeComponent>();

    return component->second;
}

std::shared_ptr<ISceneNodeComponent> SceneNode::AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component)
{
    m_Components[ComponentID] = Component;
    return Component;
}

const SceneNode::ComponentsMap& SceneNode::GetComponents() const
{
    return m_Components;
}

void SceneNode::RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message)
{
	const auto& components = GetComponents();
	std::for_each(components.begin(), components.end(), [&Component, &Message](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& ComponentMapIter) 
	{
		ComponentMapIter.second->OnMessage(Component, Message);
	});
}



//
// Scene access
//
void SceneNode::SetScene(std::shared_ptr<Scene> Scene)
{
    m_Scene = Scene;
}

std::shared_ptr<Scene> SceneNode::GetScene() const
{
    return m_Scene.lock();
}



//
// Childs functional
//
void SceneNode::AddChild(std::shared_ptr<SceneNode> childNode)
{
	if (childNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
		if (iter == m_Children.end())
		{
			childNode->m_ParentNode = shared_from_this();
            RaiseOnParentChanged();

			m_Children.push_back(childNode);
			if (!childNode->GetName().empty())
				m_ChildrenByName.insert(NodeNameMap::value_type(childNode->GetName(), childNode));
		}
	}
}

void SceneNode::RemoveChild(std::shared_ptr<SceneNode> childNode)
{
	if (childNode)
	{
		NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), childNode);
		if (iter != m_Children.end())
		{
			childNode->m_ParentNode.reset();
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

void SceneNode::SetParent(std::weak_ptr<SceneNode> parentNode)
{
	// Remove from current parent
	std::shared_ptr<SceneNode> currentParent = m_ParentNode.lock();
	if (currentParent != nullptr)
	{
		currentParent->RemoveChild(shared_from_this());
		m_ParentNode.reset();
	}

	// Add to new parent
	if (std::shared_ptr<SceneNode> newParent = parentNode.lock())
		newParent->AddChild(SceneNode::shared_from_this());
}

std::shared_ptr<SceneNode> SceneNode::GetParent() const
{
	return m_ParentNode.lock();
}

SceneNode::NodeList SceneNode::GetChilds()
{
	return m_Children;
}


void SceneNode::UpdateCamera(const Camera* camera)
{
	// Do nothing...
}

bool SceneNode::Accept(IVisitor* visitor)
{
    fail1();
	return false;
}

void SceneNode::OnUpdate(UpdateEventArgs & e)
{
}



//
// Protected
//

void SceneNode::RaiseOnParentChanged()
{
    for (auto c : m_Components)
    {
        c.second->OnParentChanged();
    }
}
