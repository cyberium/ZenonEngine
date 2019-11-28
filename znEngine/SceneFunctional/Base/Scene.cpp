#include "stdafx.h"

// General
#include "Scene.h"

Scene::Scene(std::shared_ptr<IBaseManager> BaseManager)
	: m_BaseManager(BaseManager)
{}

Scene::~Scene()
{}

const std::shared_ptr<IBaseManager> Scene::GetBaseManager() const
{
	return m_BaseManager;
}
