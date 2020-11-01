#pragma once

class CSceneNode3DEngineCreator
	: public CObjectClassCreatorBase
{
public:
	CSceneNode3DEngineCreator(IBaseManager& BaseManager);
	virtual ~CSceneNode3DEngineCreator();

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