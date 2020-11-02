#include "stdafx.h"

// General
#include "SceneNodeUIFactory.h"

CSceneNodeUIFactory::CSceneNodeUIFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type)
	: CObjectClassFactory(BaseManager, TypeName, Type)
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
