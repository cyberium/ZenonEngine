#pragma once

//
// CSceneNodeRTSPoint
//
class CSceneNodeRTSPoint
	: public CSceneNode
	, public ISceneNodeRTSPoint
{
public:
	CSceneNodeRTSPoint(IScene& Scene);
	virtual ~CSceneNodeRTSPoint();
};



//
// CSceneNodeRTSPath
//
class CSceneNodeRTSPath
	: public CSceneNode
	, public ISceneNodeRTSPath
{
public:
	CSceneNodeRTSPath(IScene& Scene);
	virtual ~CSceneNodeRTSPath();

	// ISceneNodeRTSPath
	std::shared_ptr<ISceneNodeRTSPoint> AddPoint(glm::vec3 XYZ) override;
	std::shared_ptr<ISceneNodeRTSPoint> InsertPoint(glm::vec3 XYZ, size_t AfterIndex) override;
	std::shared_ptr<ISceneNodeRTSPoint> GetLastPoint() override;
	std::vector<std::shared_ptr<ISceneNodeRTSPoint>> GetPoints() const override;
	std::shared_ptr<ISceneNodeRTSPoint> GetPoint(size_t Index) const override;
};