#include "stdafx.h"

// General
#include "UIControlFactory.h"

CUIControlFactory::CUIControlFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type)
	: CObjectClassFactory(BaseManager, TypeName, Type)
{
}

CUIControlFactory::~CUIControlFactory()
{}



//
// IUIControlFactory
//
std::shared_ptr<IUIControl> CUIControlFactory::CreateSceneNodeUI(ObjectClass ObjectClassKey, IScene& Scene, const std::shared_ptr<IUIControl>& Parent)
{
	CSceneNodeUICreationArgs creationArgs(Scene, Parent);
	return std::dynamic_pointer_cast<IUIControl>(CreateObject(ObjectClassKey, &creationArgs));
}
