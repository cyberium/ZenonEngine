#pragma once

class CSceneNodeEngineCreator
	: public CObjectClassCreatorBase
{
public:
	CSceneNodeEngineCreator(IBaseManager& BaseManager);
	virtual ~CSceneNodeEngineCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs) override;
};


class CSceneNodeUIEngineCreator
	: public CObjectClassCreatorBase
{
public:
	CSceneNodeUIEngineCreator(IBaseManager& BaseManager);
	virtual ~CSceneNodeUIEngineCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs) override;
};