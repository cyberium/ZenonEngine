#include "stdafx.h"

// General
#include "FBXSkeleton.h"

// Additional
#include "FBXUtils.h"

CFBXSkeleton::CFBXSkeleton(const IBaseManager & BaseManager, const IFBXScene& FBXScene)
	: m_BaseManager(BaseManager)
	, m_FBXScene(FBXScene)
{
}

CFBXSkeleton::~CFBXSkeleton()
{
}

void CFBXSkeleton::Load(fbxsdk::FbxScene* FBXScene)
{
	for (int childindex = 0; childindex < FBXScene->GetRootNode()->GetChildCount(); ++childindex)
	{
		FbxNode *node = FBXScene->GetRootNode()->GetChild(childindex);
		ProcessSkeletonHeirarchyre(node, 0, 0, -1);
	}
}

const CSkeleton& CFBXSkeleton::GetSkeleton() const
{
	return m_Skeleton;
}

CSkeleton & CFBXSkeleton::GetSkeletonEditable()
{
	return m_Skeleton;
}



//
// Private
//
void CFBXSkeleton::ProcessSkeletonHeirarchyre(fbxsdk::FbxNode * node, int depth, int index, int parentindex)
{
	bool isFinded = false;
	if (node->GetNodeAttribute() && node->GetNodeAttribute()->GetAttributeType() && node->GetNodeAttribute()->GetAttributeType() == fbxsdk::FbxNodeAttribute::eSkeleton)
	{
		fbxsdk::FbxSkeleton* skeletonNode = node->GetSkeleton();

		fbxsdk::FbxDouble3 lTranslation = node->LclTranslation.Get();
		Log::Warn("T x = %f, y = %f, z = %f", lTranslation[0], lTranslation[1], lTranslation[2]);

		fbxsdk::FbxDouble3 lRotation = node->EvaluateLocalRotation();
		Log::Warn("R x = %f, y = %f, z = %f", lRotation[0], lRotation[1], lRotation[2]);

		fbxsdk::FbxDouble3 lScaling = node->LclScaling.Get();
		Log::Warn("S x = %f, y = %f, z = %f", lScaling[0], lScaling[1], lScaling[2]);
		Log::Warn("");

		CSkeletonBone bone(node->GetName(), parentindex);
		bone.GlobalTransform = ToGLMMat4(node->EvaluateGlobalTransform());
		bone.LocalTransform = ToGLMMat4(node->EvaluateLocalTransform());
		m_Skeleton.AddBone(bone);

		isFinded = true;
	}

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ProcessSkeletonHeirarchyre(node->GetChild(i), depth + 1, m_Skeleton.GetBones().size(), isFinded ? index : parentindex);
	}
}
