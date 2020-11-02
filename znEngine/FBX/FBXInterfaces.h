#pragma once

ZN_INTERFACE IFBXScene;
ZN_INTERFACE IFBXNode;

#include "Scene/Components/Skeleton/Skeleton.h"


ZN_INTERFACE ZN_API IFBXAnimation
{
	virtual ~IFBXAnimation() {}

	virtual const std::vector<SAnimation>& GetAnimations() const = 0;
};

ZN_INTERFACE ZN_API IFBXLight
{
	virtual ~IFBXLight() {}

	virtual const SLight& GetLight() const = 0;
};

ZN_INTERFACE ZN_API IFBXMaterial
{
	virtual ~IFBXMaterial() {}

	virtual std::shared_ptr<IMaterial> GetMaterial() = 0;
};

ZN_INTERFACE ZN_API IFBXModel
{
	virtual ~IFBXModel() {}

	virtual const IFBXNode& GetOwner() const = 0;
	virtual std::shared_ptr<IModel> GetModel() = 0;
};

ZN_INTERFACE ZN_API IFBXSkeleton
{
	virtual ~IFBXSkeleton() {}

	virtual const CSkeleton& GetSkeleton() const = 0;
	virtual CSkeleton& GetSkeletonEditable () = 0;
};

ZN_INTERFACE ZN_API IFBXNode
{
	virtual ~IFBXNode() {}

	virtual const IFBXScene& GetFBXScene() const = 0;
	virtual glm::mat4 GetTransform() const = 0;
	virtual glm::mat4 GetParentWorldTransform() const = 0;
	virtual std::weak_ptr<IFBXNode> GetParent() const = 0;
	virtual const std::vector<std::shared_ptr<IFBXNode>>& GetChilds() const = 0;
	virtual std::shared_ptr<IFBXMaterial> GetFBXMaterial(int Index) const = 0;
	virtual std::shared_ptr<IFBXModel> GetFBXModel() const = 0;
	virtual std::shared_ptr<IFBXLight> GetFBXLight() const = 0;
};

ZN_INTERFACE ZN_API IFBXScene
{
	virtual ~IFBXScene() {}

	virtual std::shared_ptr<IFBXNode> GetFBXRootNode() const = 0;
	virtual const std::vector<std::shared_ptr<IFBXModel>>& GetFBXModels() const = 0;
	virtual std::shared_ptr<IFBXSkeleton> GetFBXSkeleton() const = 0;
	virtual std::shared_ptr<IFBXAnimation> GetFBXAnimation() const = 0;
	virtual const IznLoaderParams* GetLoaderParams() const = 0;
	virtual std::shared_ptr<IModel> MergeModels() = 0;
};

ZN_INTERFACE ZN_API IFBXScenePrivate
{
	virtual ~IFBXScenePrivate() {}

	virtual void AddModel(const std::shared_ptr<IFBXModel>& Model) = 0;
};

ZN_INTERFACE ZN_API IFBXSceneLoader
{
	virtual ~IFBXSceneLoader() {}

	virtual std::shared_ptr<IFBXScene> LoadScene(const std::string& ModelFileName, const IznLoaderParams* LoaderParams) const = 0;
	virtual std::shared_ptr<IFBXScene> LoadScene(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams) const = 0;
};


class CznFBXLoaderParams
	: public IznLoaderParams
{
public:
	CznFBXLoaderParams()
		: MakeCenterIsX0Z(false)
		, ApplyFullTransform(false)
	{}

	std::string TexturesPathRoot;
	std::string OverrideTexture;
	std::unordered_map<uint8, uint8> TexturesTypeChange;
	bool MakeCenterIsX0Z;
	bool ApplyFullTransform;
};



inline void DoAddModels(const std::shared_ptr<ISceneNode>& ParentNode, std::shared_ptr<IFBXNode> Node)
{
	const auto& baseManager = ParentNode->GetScene().GetBaseManager();
	auto sceneNode = baseManager.GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, ParentNode->GetScene(), ParentNode);

	if (auto fbxModel = Node->GetFBXModel())
		sceneNode->GetComponent<IModelsComponent3D>()->SetModel(fbxModel->GetModel());

	for (const auto& c : Node->GetChilds())
		DoAddModels(sceneNode, c);
}
