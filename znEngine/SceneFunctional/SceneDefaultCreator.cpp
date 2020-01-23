#include "stdafx.h"

#if 0

// General
#include "SceneDefaultCreator.h"

CSceneDefaultCreator::CSceneDefaultCreator(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{}

CSceneDefaultCreator::~CSceneDefaultCreator()
{}



//
// ISceneNodeCreator
//
size_t CSceneDefaultCreator::GetScenesCount() const
{
	return 1;
}

std::string CSceneDefaultCreator::GetSceneTypeName(size_t Index) const
{
	if (Index == 0)
	{
		return "SceneBase";
	}

	return nullptr;
}

std::shared_ptr<IScene> CSceneDefaultCreator::CreateScene(size_t Index) const
{
	if (Index == 0)
	{
		std::shared_ptr<IScene> scene = std::make_shared<SceneBase>(m_BaseManager);
		scene->CreateRootNode3D();
		return scene;
	}

	return nullptr;
}

#endif
