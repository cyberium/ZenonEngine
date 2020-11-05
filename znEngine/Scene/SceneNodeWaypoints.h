#pragma once

#include "EngineSceneIntfs.h"


//
// CSceneNodePoint
//
class CSceneNodePoint
	: public CSceneNode
	, public ISceneNodePoint
{
public:
	CSceneNodePoint(IScene& Scene);
	virtual ~CSceneNodePoint();
};


//
// CSceneNodePath
//
class CSceneNodePath
	: public CSceneNode
	, public ISceneNodePath
{
public:
	CSceneNodePath(IScene& Scene);
	virtual ~CSceneNodePath();

	// ISceneNodePath
	std::shared_ptr<ISceneNodePoint> AddPoint(glm::vec3 XYZ) override;
	std::shared_ptr<ISceneNodePoint> InsertPoint(glm::vec3 XYZ, size_t AfterIndex) override;
	std::shared_ptr<ISceneNodePoint> GetLastPoint() override;
	std::vector<std::shared_ptr<ISceneNodePoint>> GetPoints() const override;
};