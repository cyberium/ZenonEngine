#include "stdafx.h"

// General
#include "Scene3D.h"

Scene3D::Scene3D()
{
	m_pRootNode = std::make_shared<SceneNode3D>();
}

Scene3D::~Scene3D()
{ }

std::shared_ptr<SceneNode3D> Scene3D::GetRootNode() const
{
	return m_pRootNode;
}

void Scene3D::Accept(IVisitor& visitor)
{
	if (m_pRootNode)
		m_pRootNode->Accept(visitor);
}

void Scene3D::OnUpdate(UpdateEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnUpdate(e);
}
