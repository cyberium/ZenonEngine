#include "stdafx.h"

// General
#include "SceneCreator.h"

CSceneEngineCreator::CSceneEngineCreator(IBaseManager& BaseManager)
	: CObjectClassCreator(BaseManager)
{
	AddKey("SceneBase", cSceneBase);
}

CSceneEngineCreator::~CSceneEngineCreator()
{}

std::shared_ptr<IObject> CSceneEngineCreator::CreateObject(size_t Index, const IObjectCreationArgs * ObjectCreationArgs)
{
	/*if (Index == 0)
	{
		std::shared_ptr<IScene> scene = MakeShared(SceneBase, GetBaseManager());
		return scene;
	}*/

	return nullptr;
}
