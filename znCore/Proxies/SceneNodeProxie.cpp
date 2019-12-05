#include "stdafx.h"

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

const std::string & CSceneNodeProxie::GetName() const
{
	return m_SceneNode->GetName();
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

bool CSceneNodeProxie::Accept(IVisitor * visitor)
{
	return m_SceneNode->Accept(visitor);
}

void CSceneNodeProxie::OnUpdate(UpdateEventArgs & e)
{
	m_SceneNode->OnUpdate(e);
}



//
// ISceneNodeWrapper
//
void CSceneNodeProxie::SetThisNode(std::shared_ptr<ISceneNode> ThisNode)
{
	_ASSERT(m_SceneNode == nullptr);
	m_SceneNode = ThisNode;
	_ASSERT(m_SceneNode != nullptr);
}
