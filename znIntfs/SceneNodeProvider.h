#pragma once

// FORWARD BEGIN
class SceneNode3D;
// FORWARD END

struct ISceneNodeProvider
{
	virtual void CreateInsances(std::weak_ptr<SceneNode3D> _parent) = 0;
};