#include "stdafx.h"

// General
#include "SkeletonComponent.h"

CSkeletonComponent3D::CSkeletonComponent3D(const ISceneNode3D& OwnerNode, const CSkeleton& Skeleton)
	: CComponentBase(OwnerNode)
{
	for (const auto& b : Skeleton.GetBones())
	{
		auto bone = std::make_shared<CSkeletonComponentBone3D>(b);
		AddBone(bone);
	}

	for (const auto& bone : m_Bones)
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(bone)->SetParentAndChildsInternals(m_Bones);

	m_BonesList.resize(m_Bones.size());
	m_StructuredBuffer = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(m_BonesList, EAccess::CPUWrite);
}

CSkeletonComponent3D::~CSkeletonComponent3D()
{
}



//
// ISkeletonComponent3D
//
std::shared_ptr<ISkeletonComponentBone3D> CSkeletonComponent3D::GetBone(size_t Index) const
{
	_ASSERT(Index < m_Bones.size());
	return m_Bones.at(Index);
}

const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& CSkeletonComponent3D::GetBones() const
{
	return m_Bones;
}

std::shared_ptr<IStructuredBuffer> CSkeletonComponent3D::GetBonesBuffer() const
{
	return m_StructuredBuffer;
}

std::vector<glm::mat4> CSkeletonComponent3D::CreatePose(size_t BoneStartIndex, size_t BonesCount) const
{
	if (BonesCount == 0)
		BonesCount = m_Bones.size();

	std::vector<glm::mat4> result;
	result.reserve(BonesCount);
	for (size_t i = BoneStartIndex; i < BoneStartIndex + BonesCount; i++)
		result.push_back(m_Bones[i]->GetPivotMatrix() * m_Bones[i]->GetMatrix());
	return result;
}


//
// CComponentBase
//
void CSkeletonComponent3D::Update(const UpdateEventArgs & e)
{
	for (const auto& b : m_Bones)
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(b)->Reset();

	for (const auto& b : m_Bones)
		std::dynamic_pointer_cast<ISkeletonComponentBoneInternal3D>(b)->Calculate(GetOwnerNode(), e.CameraForCulling);

	m_BonesList = CreatePose();
	m_StructuredBuffer->Set(m_BonesList);
}



//
// Protected
//
void CSkeletonComponent3D::AddBone(std::shared_ptr<ISkeletonComponentBone3D> Bone)
{
	m_Bones.push_back(Bone);
}
