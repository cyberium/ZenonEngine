#pragma once

class CComponentsEngineCreator
	: public CObjectClassCreatorBase
{
public:
	CComponentsEngineCreator(IBaseManager& BaseManager);
	virtual ~CComponentsEngineCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs) override;
};