#pragma once

namespace fbxsdk
{
	class FbxManager;
}


ZN_INTERFACE IFBXScene;

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

	virtual const IFBXScene& GetScene() const = 0;
	virtual std::weak_ptr<IFBXNode> GetParent() const = 0;
	virtual const std::vector<std::shared_ptr<IFBXNode>>& GetChilds() const = 0;
	virtual std::shared_ptr<IFBXMaterial> GetMaterial(int Index) const = 0;
	virtual std::shared_ptr<IFBXModel> GetModel() const = 0;
	virtual std::shared_ptr<IFBXLight> GetLight() const = 0;
};

ZN_INTERFACE ZN_API IFBXScene
{
	virtual ~IFBXScene() {}

	virtual std::shared_ptr<IFBXNode> GetRootNode() const = 0;
	virtual std::shared_ptr<IFBXSkeleton> GetSkeleton() const = 0;
	virtual std::shared_ptr<IFBXAnimation> GetAnimation() const = 0;
};

ZN_INTERFACE ZN_API __declspec(uuid("653A8D4D-5E21-4734-8296-91A2E99AE767")) IFBXManager
	: public IManager
{
#ifdef ZN_FBX_SDK_ENABLE
	virtual std::shared_ptr<IFBXScene> LoadFBX(const std::string& FileName) = 0;
#endif
};

inline void DoAddModels(const std::shared_ptr<IModelsComponent3D>& ModelsComponent, std::shared_ptr<IFBXNode> Node)
{
	if (Node->GetModel())
		ModelsComponent->AddModel(Node->GetModel()->GetModel());

	for (const auto& c : Node->GetChilds())
		DoAddModels(ModelsComponent, c);
}
