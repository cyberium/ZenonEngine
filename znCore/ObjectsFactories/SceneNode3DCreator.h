#pragma once

#include "ObjectClassCreator.h"

class ZN_API CSceneNode3DCreator
	: public CObjectClassCreator
	, public ISceneNode3DCreator
{
public:
	CSceneNode3DCreator(IBaseManager& BaseManager);
	virtual ~CSceneNode3DCreator();

	// IObjectClassCreator
	//virtual std::shared_ptr<IObject> CreateObject(size_t Index) override;

	// ISceneNode3DCreator
	std::shared_ptr<ISceneNode3D> CreateSceneNode3D(ISceneNode3D* Parent, ObjectClassType ObjectClassKey) override;

protected:
	ISceneNode3D* m_Parent;
};


class ZN_API CSceneNodeUICreator
	: public CObjectClassCreator
	, public ISceneNodeUICreator
{
public:
	CSceneNodeUICreator(IBaseManager& BaseManager);
	virtual ~CSceneNodeUICreator();

	// ISceneNodeUICreator
	std::shared_ptr<ISceneNodeUI> CreateSceneNodeUI(ISceneNodeUI* Parent, ObjectClassType ObjectClassKey) override;

protected:
	ISceneNodeUI* m_Parent;
};