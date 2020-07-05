#pragma once

const ObjectClassType cSceneNode_FBXNode = 527323442;
ZN_INTERFACE ZN_API IFBXSceneNode3D
{
	virtual ~IFBXSceneNode3D() {}

	virtual void InitializeFromFile(const std::string& FileName) = 0;
	virtual std::shared_ptr<IModel> GetModel() const = 0;
};

ZN_INTERFACE ZN_API IFBXManager
{
	virtual std::shared_ptr<IFBXSceneNode3D> CreateSceneNode(IScene* Scene, std::string SceneName) = 0;
};
