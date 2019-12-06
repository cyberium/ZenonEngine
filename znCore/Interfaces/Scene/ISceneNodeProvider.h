#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API ISceneNodeProvider
{
	virtual ~ISceneNodeProvider() {}

	virtual void CreateInsances(std::weak_ptr<ISceneNode> _parent) = 0;
};