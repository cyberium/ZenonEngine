#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
// FORWARD END

const ObjectClassType cSceneNode3D = 527338441;
const ObjectClassType cSceneNodeUI = 525832104;
const ObjectClassType cSceneNodeUI_Text = 525832105;

ZN_INTERFACE ZN_API ISceneNode3DCreator
{
	virtual ~ISceneNode3DCreator() {}

	virtual std::shared_ptr<ISceneNode3D> CreateSceneNode3D(ISceneNode3D * Parent, ObjectClassType ObjectClassKey) = 0;
};

ZN_INTERFACE ZN_API ISceneNodeUICreator
{
	virtual ~ISceneNodeUICreator() {}

	virtual std::shared_ptr<ISceneNodeUI> CreateSceneNodeUI(ISceneNodeUI * Parent, ObjectClassType ObjectClassKey) = 0;
};


ZN_INTERFACE ZN_API ISceneNodeProvider
{
	virtual ~ISceneNodeProvider() {}

	virtual void CreateInsances(const std::shared_ptr<ISceneNode3D>& Parent) const = 0;
};