#pragma once

namespace fbxsdk
{
	class FbxManager;
}


ZN_INTERFACE IFBXScene;
ZN_INTERFACE IFBXNode;

#include "Scene/Skeleton.h"

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

//const ObjectClass cSceneNode_FBXNode = UINT16_MAX - 101u;
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
};

ZN_INTERFACE ZN_API IFBXScenePrivate
{
	virtual ~IFBXScenePrivate() {}

	virtual void AddModel(const std::shared_ptr<IFBXModel>& Model) = 0;
};


ZN_INTERFACE ZN_API __declspec(uuid("653A8D4D-5E21-4734-8296-91A2E99AE767")) IFBXManager
	: public IManager
{
#ifdef ZN_FBX_SDK_ENABLE
	virtual std::shared_ptr<IFBXScene> LoadFBX(const std::string& FileName) = 0;
#endif
};



inline void DoAddModels(const std::shared_ptr<ISceneNode3D>& ParentNode, std::shared_ptr<IFBXNode> Node)
{
	auto baseManagerHolder = dynamic_cast<IBaseManagerHolder*>(ParentNode->GetScene());
	const auto& baseManager = baseManagerHolder->GetBaseManager();
	auto sceneNode = baseManager.GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, ParentNode->GetScene(), ParentNode);

	if (auto fbxModel = Node->GetFBXModel())
		sceneNode->GetModelsComponent()->SetModel(fbxModel->GetModel());

	for (const auto& c : Node->GetChilds())
		DoAddModels(sceneNode, c);
}
