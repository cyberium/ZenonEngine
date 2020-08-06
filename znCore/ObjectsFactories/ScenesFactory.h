#pragma once

#include "ObjectClassFactory.h"

class CScenesFactory
	: public CObjectClassFactory
	, public IScenesFactory
{
public:
	CScenesFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type);
	virtual ~CScenesFactory();

	// ISceneNodeUIFactory
	std::shared_ptr<IScene> CreateScene(ObjectClass ObjectClassKey) override;

private:
	std::vector<std::shared_ptr<ISceneCreator>> m_Creators;
};
