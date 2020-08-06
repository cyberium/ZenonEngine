#include "stdafx.h"

// General
#include "SceneCreator.h"

// Additional
#include "Scene/Scene_Default.h"

CSceneEngineCreator::CSceneEngineCreator(IBaseManager& BaseManager)
	: CObjectClassCreator(BaseManager)
{
	AddKey("SceneBase", cSceneBase);
	AddKey("SceneDefault", cSceneDefault);
}

CSceneEngineCreator::~CSceneEngineCreator()
{}

std::shared_ptr<IObject> CSceneEngineCreator::CreateObject(size_t Index, const IObjectCreationArgs * ObjectCreationArgs)
{
	if (Index == 0)
	{
		std::shared_ptr<IScene> scene = std::make_shared<SceneBase>(GetBaseManager());
		return scene;
	}
	else if (Index == 1)
	{
		std::shared_ptr<IScene> scene = std::make_shared<CSceneDefault>(GetBaseManager());
		return scene;
	}

	return nullptr;
}
