#pragma once

// FORWARD BEGIN
ZN_INTERFACE IFBXScene;
ZN_INTERFACE IFBXNode;
// FORWARD END


ZN_INTERFACE ZN_API IFBXAnimation
{
	virtual ~IFBXAnimation() {}

	virtual const std::vector<std::shared_ptr<IAnimation>>& GetAnimations() const = 0;
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

ZN_INTERFACE ZN_API IFBXMaterials
{
	virtual ~IFBXMaterials() {}

	virtual std::shared_ptr<IMaterial> GetMaterial(const std::string& MaterialName) const = 0;
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

	virtual size_t GetBoneIndexByName(const std::string& BoneName) const = 0;
	virtual std::shared_ptr<ISkeletonBone> GetBoneByName(const std::string& BoneName) const = 0;
	virtual const std::vector<std::shared_ptr<ISkeletonBone>>& GetBones() const = 0;
};

ZN_INTERFACE ZN_API IFBXNode
{
	virtual ~IFBXNode() {}

	virtual const IFBXScene& GetFBXScene() const = 0;
	virtual glm::mat4 GetTransform() const = 0;
	virtual glm::mat4 GetParentWorldTransform() const = 0;
	virtual std::weak_ptr<IFBXNode> GetParent() const = 0;
	virtual const std::vector<std::shared_ptr<IFBXNode>>& GetChilds() const = 0;
	virtual std::string GetFBXMaterialNameByIndex(size_t Index) const = 0;
	virtual std::shared_ptr<IFBXModel> GetFBXModel() const = 0;
	virtual std::shared_ptr<IFBXLight> GetFBXLight() const = 0;
};

ZN_INTERFACE ZN_API IFBXScene
{
	virtual ~IFBXScene() {}

	virtual std::string GetFBXFilePath() const = 0;
	virtual std::shared_ptr<IFBXNode> GetFBXRootNode() const = 0;
	virtual const std::vector<std::shared_ptr<IFBXModel>>& GetFBXModels() const = 0;
	virtual std::shared_ptr<IFBXSkeleton> GetFBXSkeleton() const = 0;
	virtual std::shared_ptr<IFBXAnimation> GetFBXAnimation() const = 0;
	virtual std::shared_ptr<IFBXMaterials> GetFBXMaterials() const = 0;
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
	std::unordered_map<std::string, std::string> OverrideTextureByMaterial;
	std::unordered_map<uint8, uint8> TexturesTypeChange;
	bool MakeCenterIsX0Z;
	bool ApplyFullTransform;
};



inline void DoAddModels(const std::shared_ptr<ISceneNode>& ParentNode, std::shared_ptr<IFBXNode> Node)
{
	const auto& baseManager = ParentNode->GetScene().GetBaseManager();
	auto sceneNode = baseManager.GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, ParentNode->GetScene(), ParentNode);

	if (auto fbxModel = Node->GetFBXModel())
		sceneNode->GetComponentT<IModelsComponent3D>()->SetModel(fbxModel->GetModel());

	for (const auto& c : Node->GetChilds())
		DoAddModels(sceneNode, c);
}
