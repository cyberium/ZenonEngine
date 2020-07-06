#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
// FORWARD END


ZN_INTERFACE ZN_API ISceneNode3DCreationArgs
	: public IObjectCreationArgs
{
	virtual ~ISceneNode3DCreationArgs() {}

	virtual IScene* GetScene() = 0;
	virtual std::shared_ptr<ISceneNode3D> GetParent() const = 0;
};

ZN_INTERFACE ZN_API ISceneNode3DFactory
{
	static ObjectType GetSupportedObjectType() { return otSceneNode3D; }

	virtual ~ISceneNode3DFactory() {}

	virtual std::shared_ptr<ISceneNode3D> CreateSceneNode3D(ObjectClass ObjectClassKey, IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent = nullptr) = 0;
	virtual std::shared_ptr<ISceneNode3D> LoadSceneNode3D(std::shared_ptr<IByteBuffer> Bytes, IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent = nullptr) = 0;
};

// ==================================================================

ZN_INTERFACE ZN_API ISceneNodeUICreationArgs
	: public IObjectCreationArgs
{
	virtual ~ISceneNodeUICreationArgs() {}

	virtual IScene* GetScene() const = 0;
	virtual std::shared_ptr<ISceneNodeUI> GetParent() const = 0;
};

ZN_INTERFACE ZN_API ISceneNodeUIFactory
{
	static ObjectType GetSupportedObjectType() { return otSceneNodeUI; }

	virtual ~ISceneNodeUIFactory() {}

	virtual std::shared_ptr<ISceneNodeUI> CreateSceneNodeUI(IScene* Scene, ObjectClass ObjectClassKey, const std::shared_ptr<ISceneNodeUI>& Parent = nullptr) = 0;
};


ZN_INTERFACE ZN_API ISceneNodeProvider
{
	virtual ~ISceneNodeProvider() {}

	virtual void CreateInsances(const std::shared_ptr<ISceneNode3D>& Parent) const = 0;
};