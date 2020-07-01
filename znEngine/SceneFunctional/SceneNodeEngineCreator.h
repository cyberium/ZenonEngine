#pragma once

#include "FBX/FBXInterfaces.h"

class CSceneNode3DEngineCreator
	: public CObjectClassCreator
{
public:
	CSceneNode3DEngineCreator(IBaseManager& BaseManager);
	virtual ~CSceneNode3DEngineCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs) override;

private:
	std::shared_ptr<IFBXManager> m_FBXManager;
};


class CSceneNodeUIEngineCreator
	: public CObjectClassCreator
{
public:
	CSceneNodeUIEngineCreator(IBaseManager& BaseManager);
	virtual ~CSceneNodeUIEngineCreator();

	// IObjectClassCreator
	virtual std::shared_ptr<IObject> CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs) override;
};