#include "stdafx.h"

#if 0

// General
#include "SceneNodeProxie.h"

CSceneNodeProxie::CSceneNodeProxie()
{}

CSceneNodeProxie::~CSceneNodeProxie()
{}



//
// ISceneNode
//
void CSceneNodeProxie::Initialize()
{
	m_SceneNode->Initialize();
}

void CSceneNodeProxie::Finalize()
{
	m_SceneNode->Finalize();
}

std::string CSceneNodeProxie::GetName() const
{
	return "CSceneNodeProxie"; 

}

void CSceneNodeProxie::SetName(const std::string & name)
{
	m_SceneNode->SetName(name);
}

bool CSceneNodeProxie::IsComponentExists(GUID ComponentID)
{
	return m_SceneNode->IsComponentExists(ComponentID);
}

std::shared_ptr<ISceneNodeComponent> CSceneNodeProxie::GetComponent(GUID ComponentID)
{
	return m_SceneNode->GetComponent(ComponentID);
}

std::shared_ptr<ISceneNodeComponent> CSceneNodeProxie::AddComponent(GUID ComponentID, std::shared_ptr<ISceneNodeComponent> Component)
{
	return m_SceneNode->AddComponent(ComponentID, Component);
}

const ComponentsMap& CSceneNodeProxie::GetComponents() const
{
	return m_SceneNode->GetComponents();
}

void CSceneNodeProxie::RaiseComponentMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message)
{
	m_SceneNode->RaiseComponentMessage(Component, Message);
}

void CSceneNodeProxie::RegisterComponents()
{
	m_SceneNode->RegisterComponents();
}

void CSceneNodeProxie::SetScene(std::shared_ptr<IScene> Scene)
{
	m_SceneNode->SetScene(Scene);
}

std::shared_ptr<IScene> CSceneNodeProxie::GetScene() const
{
	return m_SceneNode->GetScene();
}

void CSceneNodeProxie::AddChild(std::shared_ptr<ISceneNode> childNode)
{
	m_SceneNode->AddChild(childNode);
}

void CSceneNodeProxie::RemoveChild(std::shared_ptr<ISceneNode> childNode)
{
	m_SceneNode->RemoveChild(childNode);
}

void CSceneNodeProxie::SetParent(std::weak_ptr<ISceneNode> parentNode)
{
	m_SceneNode->SetParent(parentNode);
}

std::shared_ptr<ISceneNode> CSceneNodeProxie::GetParent() const
{
	return m_SceneNode->GetParent();
}

std::vector<std::shared_ptr<ISceneNode>> CSceneNodeProxie::GetChilds()
{
	return m_SceneNode->GetChilds();
}

void CSceneNodeProxie::UpdateCamera(const ICamera * camera)
{
	m_SceneNode->UpdateCamera(camera);
}

void CSceneNodeProxie::UpdateViewport(const Viewport * viewport)
{
	m_SceneNode->UpdateViewport(viewport);
}

bool CSceneNodeProxie::Accept(IVisitor * visitor)
{
	bool visitResult = false;

	if (ISceneNode* ss_3d = dynamic_cast<ISceneNode*>(m_SceneNode))
	{
		visitResult = visitor->Visit3D(this);
	}
	else if (ISceneNodeUI* ss_ui = dynamic_cast<ISceneNodeUI*>(m_SceneNode))
	{
		visitResult = visitor->VisitUI(this);
	}

	return visitResult;
}

void CSceneNodeProxie::OnUpdate(UpdateEventArgs & e)
{
	m_SceneNode->OnUpdate(e);
}

std::shared_ptr<ISettingGroup> CSceneNodeProxie::GetProperties() const
{
	return m_SceneNode->GetProperties();
}



//
// ISceneNodeWrapper
//
void CSceneNodeProxie::SetWrappedNode(std::shared_ptr<ISceneNode> ThisNode)
{
	_ASSERT(m_SceneNode == nullptr);
	m_SceneNode = ThisNode;
	_ASSERT(m_SceneNode != nullptr);
}

std::shared_ptr<ISceneNode> CSceneNodeProxie::GetWrappedNode() const
{
	return m_SceneNode;
}

void CSceneNodeProxie::RaiseOnParentChanged()
{
	m_SceneNode->RaiseOnParentChanged();
}



//
// Protected
//
IBaseManager& CSceneNodeProxie::GetBaseManager() const
{
	return std::dynamic_pointer_cast<IBaseManagerHolder>(GetScene())->GetBaseManager();
}


#endif
