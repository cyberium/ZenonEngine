#pragma once

#include "../ObjectClassCreator.h"

class ZN_API CSceneNodeCreator
	: public CObjectClassCreatorBase
{
public:
	CSceneNodeCreator(IBaseManager& BaseManager);
	virtual ~CSceneNodeCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(ObjectClass ObjectClass, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs) override;
};
