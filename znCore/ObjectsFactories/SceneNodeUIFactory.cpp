#include "stdafx.h"

// General
#include "SceneNodeUIFactory.h"

// Additional
#include "Scene/UI/SceneNodeUI.h"


CSceneNodeUIFactory::CSceneNodeUIFactory(IBaseManager& BaseManager, ObjectType Type)
	: CObjectClassFactory(BaseManager, Type)
{
}

CSceneNodeUIFactory::~CSceneNodeUIFactory()
{}

//
// ISceneNodeUIFactory
//
std::shared_ptr<ISceneNodeUI> CSceneNodeUIFactory::CreateSceneNodeUI(IScene* Scene, ObjectClass ObjectClassKey, const std::shared_ptr<ISceneNodeUI>& Parent)
{
	CSceneNodeUICreationArgs creationArgs(Scene, Parent);
	return std::dynamic_pointer_cast<ISceneNodeUI>(CreateObject(ObjectClassKey, &creationArgs));
}
