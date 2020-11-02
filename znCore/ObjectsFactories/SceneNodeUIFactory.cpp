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
// IUIControlFactory
//
std::shared_ptr<IUIControl> CSceneNodeUIFactory::CreateSceneNodeUI(ObjectClass ObjectClassKey, IScene& Scene, const std::shared_ptr<IUIControl>& Parent)
{
	CSceneNodeUICreationArgs creationArgs(Scene, Parent);
	return std::dynamic_pointer_cast<IUIControl>(CreateObject(ObjectClassKey, &creationArgs));
}
