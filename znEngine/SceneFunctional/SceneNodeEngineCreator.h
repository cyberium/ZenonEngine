#pragma once

class CSceneNode3DEngineCreator
	: public CSceneNode3DCreator
{
public:
	CSceneNode3DEngineCreator(IBaseManager& BaseManager);
	virtual ~CSceneNode3DEngineCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(ObjectClassType ObjectClassKey) override;
};


class CSceneNodeUIEngineCreator
	: public CSceneNodeUICreator
{
public:
	CSceneNodeUIEngineCreator(IBaseManager& BaseManager);
	virtual ~CSceneNodeUIEngineCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(ObjectClassType ObjectClassKey) override;
};