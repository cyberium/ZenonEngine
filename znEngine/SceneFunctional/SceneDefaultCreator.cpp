#include "stdafx.h"

// General
#include "SceneDefaultCreator.h"

// Additional
#include "GameState_Default.h"

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
	return 2;
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

	return nullptr;
}
