#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class ZN_API CFBXSkeleton
	: public IFBXSkeleton
{
public:
	CFBXSkeleton(const IBaseManager& BaseManager, const IFBXScene& FBXScene);
	virtual ~CFBXSkeleton();

	void Load(fbxsdk::FbxScene* FBXScene);

	// IFBXSkeleton
	std::shared_ptr<ISkeletonBone> GetRootBone() const override;
	size_t GetBoneIndexByName(const std::string& BoneName) const override;
	std::shared_ptr<ISkeletonBone> GetBoneByName(const std::string& BoneName) const override;
	const std::vector<std::shared_ptr<ISkeletonBone>>& GetBones() const override;

protected:
	void ProcessSkeletonHeirarchyre(fbxsdk::FbxNode * Node, int depth, int index, int parentindex);

private:
	std::shared_ptr<ISkeletonBone> m_RootNode;
	std::vector<std::shared_ptr<ISkeletonBone>> m_Bones;

private:
	const IBaseManager& m_BaseManager;
	const IFBXScene& m_FBXScene;
};

#endif
