#include "stdafx.h"

// General
#include "SceneDefaultCreator.h"

// Additional
#include "Scene_Default.h"
#include "Scene_TCEditor.h"

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
	return 3;
}

std::string CSceneDefaultCreator::GetSceneTypeName(size_t Index) const
{
	if (Index == 0)
	{
		return "SceneBase";
	}
	else if (Index == 1)
	{
		return "SceneDefault";
	}
	else if (Index == 2)
	{
		return "SceneTCEditor";
	}

	return nullptr;
}

std::shared_ptr<IScene> CSceneDefaultCreator::CreateScene(size_t Index) const
{
	if (Index == 0)
	{
		std::shared_ptr<IScene> scene = std::make_shared<SceneBase>(m_BaseManager);
		return scene;
	}
	else if (Index == 1)
	{
		return std::make_shared<CGameState_World>(m_BaseManager);
	}
	else if (Index == 2)
	{
		return std::make_shared<CScene_TCEditor>(m_BaseManager);
	}

	return nullptr;
}
