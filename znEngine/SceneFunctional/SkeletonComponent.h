#pragma once

#include "3D//Skeleton.h"
#include "SkeletonComponentBone.h"

class __declspec(UUID_SkeletonComponent) ZN_API CSkeletonComponent3D
	: public ISkeletonComponent3D
	, public CComponentBase
{
public:
	CSkeletonComponent3D(const SceneNode3D& OwnerNode, const CSkeleton& Skeleton);
	virtual ~CSkeletonComponent3D();

	std::vector<glm::mat4> CreatePose(size_t BoneStartIndex = 0, size_t BonesCount = 0) const;

	// ISkeletonComponent3D
	std::shared_ptr<ISkeletonComponentBone3D> GetBone(size_t Index) const override;

	// CComponentBase
	void Update(const UpdateEventArgs& e) override;

protected:
	virtual void AddBone(std::shared_ptr<ISkeletonComponentBone3D> Bone);

private:
	std::vector<std::shared_ptr<ISkeletonComponentBone3D>> m_Bones;
};