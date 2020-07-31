#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>

#include "SceneFunctional/3D/AnimatedValue.h"
#include "SceneFunctional/3D/Skeleton.h"

class ZN_API CFBXSkeleton
	: public IFBXSkeleton
{
public:
	CFBXSkeleton(const IBaseManager& BaseManager, const IFBXScene& FBXScene);
	virtual ~CFBXSkeleton();

	void Load(fbxsdk::FbxScene* FBXScene);

	// IFBXSkeleton
	const CSkeleton& GetSkeleton() const override;
	CSkeleton& GetSkeletonEditable() override;

protected:
	void ProcessSkeletonHeirarchyre(FbxNode * node, int depth, int index, int parentindex);

private:
	CSkeleton           m_Skeleton;
	const IBaseManager& m_BaseManager;
	const IFBXScene&    m_FBXScene;
};

#endif
