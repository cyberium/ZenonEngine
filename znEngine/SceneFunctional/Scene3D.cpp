#include "stdafx.h"

// General
#include "Scene3D.h"

Scene3D::Scene3D()
{
	m_RootNode = std::make_shared<SceneNode3D>();
}

Scene3D::~Scene3D()
{ }

std::shared_ptr<SceneNode3D> Scene3D::GetRootNode() const
{
	return m_RootNode;
}

void Scene3D::Accept(IVisitor& visitor)
{
	if (m_RootNode)
		m_RootNode->Accept(visitor);
}

void Scene3D::OnUpdate(UpdateEventArgs & e)
{
	if (m_RootNode)
		m_RootNode->OnUpdate(e);
}
