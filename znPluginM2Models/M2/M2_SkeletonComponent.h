#pragma once

class CM2;
class CM2_Base_Instance;
#include "M2/M2_Part_Bone.h"


//
// CM2SkeletonBone3D
//
class ZN_API CM2SkeletonBone3D
	: public ISkeletonComponentBone3D
	, public ISkeletonComponentBoneInternal3D
	, public std::enable_shared_from_this<CM2SkeletonBone3D>
{
public:
	CM2SkeletonBone3D(const SM2_Part_Bone_Wrapper& M2Bone);
	virtual ~CM2SkeletonBone3D();

	// ISkeletonComponentBone3D
	std::string GetName() const override;
	const std::weak_ptr<ISkeletonComponentBone3D>& GetParentBone() const override;
	const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& GetChilds() const override;
	glm::vec3 GetPivotPoint() const override;

	const glm::mat4& GetMatrix() const override;
	const glm::mat4& GetRotateMatrix() const override;

	// ISkeletonComponentBoneInternal3D
	void AddChildInternal(const std::shared_ptr<ISkeletonComponentBone3D>& Child) override;
	void SetParentAndChildsInternals(const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& Bones) override;
	void Calculate(const ISceneNode& Instance, const ICameraComponent3D* Camera) override;
	void Reset() override;

private:
	const SM2_Part_Bone_Wrapper&                   m_M2Bone;
	std::weak_ptr<ISkeletonComponentBone3D>                 m_ParentBone;
	std::vector<std::shared_ptr<ISkeletonComponentBone3D>>  m_Childs;
	glm::vec3                                      m_PivotPoint;
	glm::mat4                                      m_Matrix;
	glm::mat4                                      m_RotateMatrix;
	bool                                           m_IsCalculated;
};



//
// CM2SkeletonComponent3D
//
class ZN_API CM2SkeletonComponent3D
	: public ISkeletonComponent3D
	, public CComponentBase
{
public:
	CM2SkeletonComponent3D(const CM2_Base_Instance& OwnerNode);
	virtual ~CM2SkeletonComponent3D();

	// CComponentBase
	void Update(const UpdateEventArgs& e) override;

	// ISkeletonComponent3D
	std::shared_ptr<ISkeletonComponentBone3D> GetBone(size_t Index) const override;
	virtual const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& GetBones() const override;
	virtual std::shared_ptr<IStructuredBuffer> GetBonesBuffer() const override;
	virtual std::vector<glm::mat4> CreatePose(size_t BoneStartIndex = 0, size_t BonesCount = 0) const override;

protected:
	const CM2_Base_Instance& GetM2OwnerNode() const;

private:
	std::vector<std::shared_ptr<ISkeletonComponentBone3D>> m_Bones;
};