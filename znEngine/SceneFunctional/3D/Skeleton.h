#pragma once

#include "SkeletonBone.h"

class CSkeleton
{
public:
	void AddBone(const CSkeletonBone& Bone);
	const CSkeletonBone& GetBone(size_t Index) const;
	size_t GetBoneIndexByName(const std::string& BoneName) const;
	const CSkeletonBone& GetBoneByName(const std::string& BoneName) const;
	const std::vector<CSkeletonBone>& GetBones() const;

private:
	std::vector<CSkeletonBone> m_Bones;
};