#include "stdafx.h"

// General
#include "ComponentsCreator.h"


CComponentsCreator::CComponentsCreator(IBaseManager& BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{

}

CComponentsCreator::~CComponentsCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CComponentsCreator::CreateObject(ObjectClass ObjectClass, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	std::shared_ptr<IObject> createdComponent = GetObjectClassFunctorCretor(ObjectClass)(ObjectCreationArgs);
	if (createdComponent == nullptr)
		throw CException("ComponentsEngineCreator: CreateObject: Unable to create '%s'.", GetObjectClassName(ObjectClass).c_str());

	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdComponent))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("ComponentsEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	return createdComponent;
}