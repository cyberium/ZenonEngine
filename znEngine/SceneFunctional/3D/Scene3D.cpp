#include "stdafx.h"

// General
#include "Scene3D.h"

// Additional
#include "SceneNode3D.h"

Scene3D::Scene3D(std::shared_ptr<IBaseManager> BaseManager)
	: SceneBase(BaseManager)
{}

Scene3D::~Scene3D()
{}

void Scene3D::CreateRootNode()
{
    m_RootNode = CreateSceneNode<SceneNode3D>(std::shared_ptr<ISceneNode>());
	m_RootNode->SetName("Root node");
}
