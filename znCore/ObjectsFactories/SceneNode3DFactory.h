#pragma once

#include "ObjectClassFactory.h"

class ZN_API CSceneNode3DFactory
	: public CObjectClassFactory
	, public ISceneNode3DFactory
{
public:
	CSceneNode3DFactory(IBaseManager& BaseManager, ObjectType Type);
	virtual ~CSceneNode3DFactory();

	// ISceneNode3DCreator
	std::shared_ptr<ISceneNode3D> CreateSceneNode3D(ObjectClass ObjectClassKey,       IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent) override;
	std::shared_ptr<ISceneNode3D> LoadSceneNode3D(std::shared_ptr<IByteBuffer> Bytes, IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent) override;
};


class ZN_API CSceneNodeUIFactory
	: public CObjectClassFactory
	, public ISceneNodeUIFactory
{
public:
	CSceneNodeUIFactory(IBaseManager& BaseManager, ObjectType Type);
	virtual ~CSceneNodeUIFactory();

	// ISceneNodeUICreator
	std::shared_ptr<ISceneNodeUI> CreateSceneNodeUI(IScene* Scene, ObjectClass ObjectClassKey, const std::shared_ptr<ISceneNodeUI>& Parent) override;

};