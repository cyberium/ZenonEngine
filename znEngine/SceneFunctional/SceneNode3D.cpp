#include "stdafx.h"

// Include
#include "Light.h"

// General
#include "SceneNode3D.h"

// Additional
#include "Loader.h"

SceneNode3D::SceneNode3D()
	: m_Name("SceneNode3D")
	, m_IsLoadingBegin(false)
	, m_IsLoaded(false)
{
	
}

SceneNode3D::~SceneNode3D()
{
	m_Children.clear();
}

const std::string& SceneNode3D::GetName() const
{
	return m_Name;
}

void SceneNode3D::SetName(const std::string& name)
{
	m_Name = name;
}



//
// Components engine
//
bool SceneNode3D::IsComponentExists(GUID ComponentID)
{
    return m_Components.find(ComponentID) != m_Components.end();
}

std::shared_ptr<ISceneNodeComponent> SceneNode3D::GetComponent(GUID ComponentID)
{
    if (m_Components.find(ComponentID) == m_Components.end())
        return std::shared_ptr<ISceneNodeComponent>();

    return m_Components[ComponentID];
}

std::shared_ptr<ISceneNodeComponent> SceneNode3D::AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component)
{
    m_Components[ComponentID] = Component;
    return Component;
}



void SceneNode3D::SetScene(std::shared_ptr<Scene3D> Scene)
{
    m_Scene = Scene;
}

std::shared_ptr<Scene3D> SceneNode3D::GetScene() const
{
    return m_Scene.lock();
}

void SceneNode3D::AddChild(std::shared_ptr<SceneNode3D> childNode)
{
	if (childNode)
	{
		std::lock_guard<std::mutex> lg(m_ChildMutex);

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

void SceneNode3D::RemoveChild(std::shared_ptr<SceneNode3D> childNode)
{
	if (childNode)
	{
		std::lock_guard<std::mutex> lg(m_ChildMutex);

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

void SceneNode3D::SetParent(std::weak_ptr<SceneNode3D> parentNode)
{
	// Remove from current parent
	std::shared_ptr<SceneNode3D> currentParent = m_ParentNode.lock();
	if (currentParent != nullptr)
	{
		currentParent->RemoveChild(shared_from_this());
		m_ParentNode.reset();
	}

	// Add to new parent
	if (std::shared_ptr<SceneNode3D> newParent = parentNode.lock())
		newParent->AddChild(SceneNode3D::shared_from_this());
}

std::weak_ptr<SceneNode3D> SceneNode3D::GetParent() const
{
	return m_ParentNode;
}

SceneNode3D::NodeList SceneNode3D::GetChilds()
{
	std::lock_guard<std::mutex> lg(m_ChildMutex);

	return m_Children;
}


void SceneNode3D::UpdateCamera(const Camera* camera)
{
	// Do nothing...
}

bool SceneNode3D::Accept(IVisitor& visitor)
{
	bool visitResult = visitor.Visit(shared_from_this());
	//if (!visitResult)
	//	return false;

    for (auto c : m_Components)
    {
        c.second->Accept(visitor);
    }

	// Now visit children
	for (auto child : GetChilds())
	{
#ifdef LOADER_ENABLED
		std::shared_ptr<ILoadable> loadable = std::dynamic_pointer_cast<ILoadable, SceneNode3D>(child);
		if (loadable != nullptr && ! loadable->isLoaded())
			continue;
#endif

		child->Accept(visitor);
	}

	return visitResult;
}

void SceneNode3D::OnUpdate(UpdateEventArgs & e)
{
}



//
// // ILoadableObject
//
bool SceneNode3D::PreLoad()
{
	return false;
}

bool SceneNode3D::Load()
{
	return false;
}

bool SceneNode3D::Delete()
{
	return false;
}

void SceneNode3D::setLoadingBegin()
{
	m_IsLoadingBegin = true;
}

bool SceneNode3D::isLoadingBegin() const
{
	return m_IsLoadingBegin;
}

void SceneNode3D::setLoaded()
{
	m_IsLoaded = true;
}

bool SceneNode3D::isLoaded() const
{
	return m_IsLoaded;
}

uint32 SceneNode3D::getPriority() const
{
	return 0;
}



//
// Protected
//
void SceneNode3D::RegisterComponents()
{
    SetTransformComponent(AddComponent(std::make_shared<CTransformComponent>(shared_from_this())));
    AddComponent(std::make_shared<CMeshComponent>(shared_from_this()));
    SetColliderComponent(AddComponent(std::make_shared<CColliderComponent>(shared_from_this())));
    AddComponent(std::make_shared<CLightComponent>(shared_from_this()));
}

void SceneNode3D::SetTransformComponent(std::shared_ptr<CTransformComponent> TransformComponent)
{
    m_Components_Transform = TransformComponent;
}

void SceneNode3D::SetColliderComponent(std::shared_ptr<CColliderComponent> ColliderComponent)
{
    m_Components_Collider = ColliderComponent;
}

void SceneNode3D::RaiseOnParentChanged()
{
    for (auto c : m_Components)
    {
        c.second->OnParentChanged();
    }
}
