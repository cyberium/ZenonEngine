#pragma once

// FORWARD BEGIN
class SceneNodeModel3D;
// FORWARD END

struct ISceneNodeProvider
{
	virtual void CreateInsances(std::weak_ptr<SceneNodeModel3D> _parent) = 0;
};