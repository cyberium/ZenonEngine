#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
ZN_INTERFACE IUIControl;
// FORWARD END


ZN_INTERFACE ZN_API ISceneNodeCreationArgs
	: public IObjectCreationArgs
{
	virtual ~ISceneNodeCreationArgs() {}

	virtual IScene& GetScene() const = 0;
	virtual std::shared_ptr<ISceneNode> GetParent() const = 0;
};

ZN_INTERFACE ZN_API ISceneNodeFactory
{
	static ObjectType GetSupportedObjectType() { return otSceneNode; }

	virtual ~ISceneNodeFactory() {}

	virtual std::shared_ptr<ISceneNode> CreateSceneNode3D(ObjectClass ObjectClassKey, IScene& Scene, const std::shared_ptr<ISceneNode>& Parent = nullptr) = 0;
	virtual std::shared_ptr<ISceneNode> LoadSceneNode3DXML(const std::shared_ptr<IXMLReader>& Reader, IScene& Scene, const std::shared_ptr<ISceneNode>& Parent = nullptr) = 0;
	virtual std::shared_ptr<IXMLWriter> SaveSceneNode3DXML(std::shared_ptr<ISceneNode> Object) = 0;
	virtual std::shared_ptr<ISceneNode> LoadSceneNode3D(const std::shared_ptr<IByteBuffer>& Bytes, IScene& Scene, const std::shared_ptr<ISceneNode>& Parent = nullptr) = 0;
	virtual std::shared_ptr<IByteBuffer> SaveSceneNode3D(std::shared_ptr<ISceneNode> Object) = 0;
};

// ==================================================================

ZN_INTERFACE ZN_API IUIControlCreationArgs
	: public IObjectCreationArgs
{
	virtual ~IUIControlCreationArgs() {}

	virtual IScene& GetScene() const = 0;
	virtual std::shared_ptr<IUIControl> GetParent() const = 0;
};

ZN_INTERFACE ZN_API IUIControlFactory
{
	static ObjectType GetSupportedObjectType() { return otUIControl; }

	virtual ~IUIControlFactory() {}

	virtual std::shared_ptr<IUIControl> CreateSceneNodeUI(ObjectClass ObjectClassKey, IScene& Scene, const std::shared_ptr<IUIControl>& Parent = nullptr) = 0;
};


// ==================================================================


ZN_INTERFACE ZN_API IComponentCreationArgs
	: public IObjectCreationArgs
{
	virtual ~IComponentCreationArgs() {}

	virtual ISceneNode& GetSceneNode() const = 0;
};

ZN_INTERFACE ZN_API IComponentFactory
{
	static ObjectType GetSupportedObjectType() { return otSceneNodeComponent; }

	virtual ~IComponentFactory() {}

	virtual std::shared_ptr<ISceneNodeComponent> CreateComponent(ObjectClass ObjectClassKey, ISceneNode& SceneNode) = 0;
	virtual std::shared_ptr<ISceneNodeComponent> LoadComponentXML(const std::shared_ptr<IXMLReader>& Reader, ISceneNode& SceneNode) = 0;
	virtual std::shared_ptr<IXMLWriter> SaveComponentXML(std::shared_ptr<ISceneNodeComponent> Object) = 0;

	template <typename T>
	inline std::shared_ptr<T> CreateComponentT(ObjectClass ObjectClassKey, ISceneNode& SceneNode)
	{
		return std::dynamic_pointer_cast<T>(CreateComponent(ObjectClassKey, SceneNode));
	}
};


//



ZN_INTERFACE ZN_API ISceneNodeProvider
{
	virtual ~ISceneNodeProvider() {}

	virtual void CreateInsances(const std::shared_ptr<ISceneNode>& Parent) const = 0;
};