#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
// FORWARD END

ZN_INTERFACE ZN_API ISceneNodeCreator
{
	virtual ~ISceneNodeCreator() {}

	virtual size_t GetSceneNodesCount() const = 0;
	virtual std::string GetSceneNodeTypeName(size_t Index) const = 0;
	virtual std::shared_ptr<ISceneNode3D> CreateSceneNode3D(ISceneNode3D* Parent, size_t Index) const = 0;
	virtual std::shared_ptr<ISceneNodeUI> CreateSceneNodeUI(ISceneNodeUI* Parent, size_t Index) const = 0;
};


ZN_INTERFACE ZN_API __declspec(uuid("9C3ACF8D-F30D-47AE-BBA1-D71DEA6B14D4")) ISceneNodesFactory
	: public IManager
{
	virtual ~ISceneNodesFactory() {}

	virtual void AddSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) = 0;
	virtual void RemoveSceneNodeCreator(std::shared_ptr<ISceneNodeCreator> Creator) = 0;

	virtual std::shared_ptr<ISceneNode3D> CreateSceneNode3D(ISceneNode3D* Parent, std::string SceneNodeTypeName) const = 0;
	virtual std::shared_ptr<ISceneNodeUI> CreateSceneNodeUI(ISceneNodeUI* Parent, std::string SceneNodeTypeName) const = 0;
};


ZN_INTERFACE ZN_API ISceneNodeProvider
{
	virtual ~ISceneNodeProvider() {}

	virtual void CreateInsances(const std::shared_ptr<ISceneNode3D>& Parent) const = 0;
};