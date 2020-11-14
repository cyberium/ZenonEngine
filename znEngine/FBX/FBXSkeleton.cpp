#include "stdafx.h"

// General
#include "FBXSkeleton.h"

// Additional
#include "FBXUtils.h"


namespace
{
	// Get the matrix of the given pose
	fbxsdk::FbxAMatrix GetPoseMatrix(fbxsdk::FbxPose* pPose, int pNodeIndex)
	{
		FbxAMatrix lPoseMatrix;
		FbxMatrix lMatrix = pPose->GetMatrix(pNodeIndex);

		memcpy((double*)lPoseMatrix, (double*)lMatrix, sizeof(lMatrix.mData));

		return lPoseMatrix;
	}


	// Get the geometry offset to a node. It is never inherited by the children.
	fbxsdk::FbxAMatrix GetGeometry(fbxsdk::FbxNode* pNode)
	{
		const fbxsdk::FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const fbxsdk::FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const fbxsdk::FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);
		return FbxAMatrix(lT, lR, lS);
	}

	// Get the global position of the node for the current pose.
	// If the specified node is not part of the pose or no pose is specified, get its
	// global position at the current time.
	FbxAMatrix GetGlobalPosition(FbxNode* pNode, const FbxTime& pTime, fbxsdk::FbxPose* pPose, fbxsdk::FbxAMatrix* pParentGlobalPosition = nullptr)
	{
		FbxAMatrix lGlobalPosition;
		bool lPositionFound = false;

		if (pPose)
		{
			int lNodeIndex = pPose->Find(pNode);

			if (lNodeIndex > -1)
			{
				// The bind pose is always a global matrix.
				// If we have a rest pose, we need to check if it is
				// stored in global or local space.
				if (pPose->IsBindPose() || !pPose->IsLocalMatrix(lNodeIndex))
				{
					lGlobalPosition = GetPoseMatrix(pPose, lNodeIndex);
				}
				else
				{
					// We have a local matrix, we need to convert it to
					// a global space matrix.
					FbxAMatrix lParentGlobalPosition;

					if (pParentGlobalPosition)
					{
						lParentGlobalPosition = *pParentGlobalPosition;
					}
					else
					{
						if (pNode->GetParent())
						{
							lParentGlobalPosition = GetGlobalPosition(pNode->GetParent(), pTime, pPose);
						}
					}

					FbxAMatrix lLocalPosition = GetPoseMatrix(pPose, lNodeIndex);
					lGlobalPosition = lParentGlobalPosition * lLocalPosition;
				}

				lPositionFound = true;
			}
		}

		if (!lPositionFound)
		{
			// There is no pose entry for that node, get the current global position instead.

			// Ideally this would use parent global position and local position to compute the global position.
			// Unfortunately the equation 
			//    lGlobalPosition = pParentGlobalPosition * lLocalPosition
			// does not hold when inheritance type is other than "Parent" (RSrs).
			// To compute the parent rotation and scaling is tricky in the RrSs and Rrs cases.
			lGlobalPosition = pNode->EvaluateGlobalTransform(pTime);
		}

		return lGlobalPosition;
	}



}


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



//
// IFBXSkeleton
//
size_t CFBXSkeleton::GetBoneIndexByName(const std::string & BoneName) const
{
	for (size_t i = 0; i < m_Bones.size(); ++i)
		if (::_stricmp(m_Bones[i]->GetName().c_str(), BoneName.c_str()) == 0)
			return i;
	throw CException("Bone '%s' not found.", BoneName.c_str());
}

std::shared_ptr<ISkeletonBone> CFBXSkeleton::GetBoneByName(const std::string& BoneName) const
{
	for (size_t i = 0; i < m_Bones.size(); ++i)
		if (::_stricmp(m_Bones[i]->GetName().c_str(), BoneName.c_str()) == 0)
			return m_Bones[i];
	throw CException("Bone '%s' not found.", BoneName.c_str());
}

const std::vector<std::shared_ptr<ISkeletonBone>>& CFBXSkeleton::GetBones() const
{
	return m_Bones;
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

		std::shared_ptr<CSkeletonBone> bone = MakeShared(CSkeletonBone, node->GetName(), parentindex);

		fbxsdk::FbxTime keyTime;
		keyTime.SetFrame(15);

		bone->LocalTransform = ToGLMMat4(node->EvaluateLocalTransform());
		m_Bones.push_back(bone);

		isFinded = true;
	}

	for (int i = 0; i < node->GetChildCount(); i++)
	{
		ProcessSkeletonHeirarchyre(node->GetChild(i), depth + 1, m_Bones.size(), isFinded ? index : parentindex);
	}
}
