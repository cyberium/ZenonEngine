#pragma once

#include "../ObjectClassCreator.h"

class ZN_API CComponentsCreator
	: public CObjectClassCreatorBase
{
public:
	CComponentsCreator(IBaseManager& BaseManager);
	virtual ~CComponentsCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(ObjectClass ObjectClass, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs) override;
};