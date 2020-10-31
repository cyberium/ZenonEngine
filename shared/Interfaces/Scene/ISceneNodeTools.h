#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
// FORWARD END


ZN_INTERFACE ZN_API ISceneNode3DCreationArgs
	: public IObjectCreationArgs
{
	virtual ~ISceneNode3DCreationArgs() {}

	virtual IScene* GetScene() const = 0;
	virtual std::shared_ptr<ISceneNode3D> GetParent() const = 0;
};

ZN_INTERFACE ZN_API ISceneNode3DFactory
{
	static ObjectType GetSupportedObjectType() { return otSceneNode3D; }

	virtual ~ISceneNode3DFactory() {}

	virtual std::shared_ptr<ISceneNode3D> CreateSceneNode3D(ObjectClass ObjectClassKey, IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent = nullptr) = 0;
	virtual std::shared_ptr<ISceneNode3D> LoadSceneNode3DXML(const std::shared_ptr<IXMLReader>& Reader, IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent = nullptr) = 0;
	virtual std::shared_ptr<IXMLWriter> SaveSceneNode3DXML(std::shared_ptr<ISceneNode3D> Object) = 0;
	virtual std::shared_ptr<ISceneNode3D> LoadSceneNode3D(const std::shared_ptr<IByteBuffer>& Bytes, IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent = nullptr) = 0;
	virtual std::shared_ptr<IByteBuffer> SaveSceneNode3D(std::shared_ptr<ISceneNode3D> Object) = 0;
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


// ==================================================================


ZN_INTERFACE ZN_API IComponentCreationArgs
	: public IObjectCreationArgs
{
	virtual ~IComponentCreationArgs() {}

	virtual ISceneNode3D& GetSceneNode() = 0;
};

ZN_INTERFACE ZN_API IComponentFactory
{
	static ObjectType GetSupportedObjectType() { return otSceneNodeComponent; }

	virtual ~IComponentFactory() {}

	virtual std::shared_ptr<ISceneNodeComponent> CreateComponent(ObjectClass ObjectClassKey, ISceneNode3D& SceneNode) = 0;
	virtual std::shared_ptr<ISceneNodeComponent> LoadComponentXML(const std::shared_ptr<IXMLReader>& Reader, ISceneNode3D& SceneNode) = 0;
	virtual std::shared_ptr<IXMLWriter> SaveComponentXML(std::shared_ptr<ISceneNodeComponent> Object) = 0;

	template <typename T>
	inline std::shared_ptr<T> CreateComponentT(ObjectClass ObjectClassKey, ISceneNode3D& SceneNode)
	{
		return std::dynamic_pointer_cast<T>(CreateComponent(ObjectClassKey, SceneNode));
	}
};


//



ZN_INTERFACE ZN_API ISceneNodeProvider
{
	virtual ~ISceneNodeProvider() {}

	virtual void CreateInsances(const std::shared_ptr<ISceneNode3D>& Parent) const = 0;
};