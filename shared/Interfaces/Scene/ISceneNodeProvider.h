#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
// FORWARD END

ZN_INTERFACE ZN_API ISceneNodeProvider
{
	virtual ~ISceneNodeProvider() {}

	virtual void CreateInsances(std::weak_ptr<ISceneNode3D> _parent) = 0;
};