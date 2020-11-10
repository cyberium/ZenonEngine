#pragma once

class CComponentsCreator
	: public CObjectClassCreatorBase
{
public:
	CComponentsCreator(IBaseManager& BaseManager);
	virtual ~CComponentsCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs) override;
};