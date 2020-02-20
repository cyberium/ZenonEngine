#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
// FORWARD END

ZN_INTERFACE ZN_API ISceneNodeProvider
{
	virtual ~ISceneNodeProvider() {}

	virtual void CreateInsances(const std::shared_ptr<ISceneNode3D>& Parent) const = 0;
};