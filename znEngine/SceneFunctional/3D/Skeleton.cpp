#include "stdafx.h"

// General
#include "Skeleton.h"

void CSkeleton::AddBone(const CSkeletonBone& Bone)
{
	m_Bones.push_back(Bone);
}

const CSkeletonBone& CSkeleton::GetBone(size_t Index) const
{
	_ASSERT(Index < m_Bones.size());
	return m_Bones.at(Index);
}

size_t CSkeleton::GetBoneIndexByName(const std::string& BoneName) const
{
	for (size_t i = 0; i < m_Bones.size(); ++i)
		if (m_Bones[i].Name == BoneName)
			return i;
	throw CException("Bone '%s' not found.", BoneName.c_str());
}

const CSkeletonBone& CSkeleton::GetBoneByName(const std::string& BoneName) const
{
	for (size_t i = 0; i < m_Bones.size(); ++i)
		if (m_Bones[i].Name == BoneName)
			return m_Bones[i];
	throw CException("Bone '%s' not found.", BoneName.c_str());
}

const std::vector<CSkeletonBone>& CSkeleton::GetBones() const
{
	return m_Bones;
}
