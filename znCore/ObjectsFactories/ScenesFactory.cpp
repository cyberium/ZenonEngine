#include "stdafx.h"

// General
#include "ScenesFactory.h"

CScenesFactory::CScenesFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type)
	: CObjectClassFactory(BaseManager, TypeName, Type)
{
}

CScenesFactory::~CScenesFactory()
{
}

//
// IScenesFactory
//
std::shared_ptr<IScene> CScenesFactory::CreateScene(ObjectClass ObjectClassKey)
{
	return std::dynamic_pointer_cast<IScene>(CreateObject(ObjectClassKey, nullptr));
}
