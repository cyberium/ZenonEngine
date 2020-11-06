#pragma once

#include "EngineSceneIntfs.h"

class CSceneNodeRTSUnit
	: public CSceneNode
	, public ISceneNodeRTSUnit
{
public:
	CSceneNodeRTSUnit(IScene& Scene);
	virtual ~CSceneNodeRTSUnit();

	// ISceneNodeRTSUnit 
	void SetPath(std::shared_ptr<ISceneNodeRTSPath> Path) override;

	// ISceneNode
	void Initialize() override;
	void Update(const UpdateEventArgs& e) override;

private:
	std::shared_ptr<ISceneNodeRTSPath> m_Path;
	size_t m_PathCurrentPoint;
};