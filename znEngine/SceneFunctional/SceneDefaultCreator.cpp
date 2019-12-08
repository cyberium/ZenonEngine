#include "stdafx.h"

// General
#include "SceneDefaultCreator.h"

// Additional
#include "SceneFunctional/Base/SceneBase.h"

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

	throw std::exception(("CSceneDefaultCreator::GetSceneTypeName: Index '" + std::to_string(Index) + "' out of bounds.").c_str());
}

std::shared_ptr<IScene> CSceneDefaultCreator::CreateScene(size_t Index) const
{
	if (Index == 0)
	{
		std::shared_ptr<IScene> scene = std::make_shared<SceneBase>(m_BaseManager);
		scene->CreateRootNode();
		return scene;
	}

	throw std::exception(("CSceneDefaultCreator::CreateScene: Index '" + std::to_string(Index) + "' out of bounds.").c_str());
}
