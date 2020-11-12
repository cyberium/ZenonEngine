#pragma once

#include "../ObjectClassCreator.h"

class ZN_API CUIControlCreator
	: public CObjectClassCreatorBase
{
public:
	CUIControlCreator(IBaseManager& BaseManager);
	virtual ~CUIControlCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(ObjectClass ObjectClass, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs) override;
};