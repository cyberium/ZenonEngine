#pragma once

#include "SkeletonBone.h"

class ZN_API CSkeletonComponentBone3D
	: public ISkeletonComponentBone3D
	, public ISkeletonComponentBoneInternal3D
	, public std::enable_shared_from_this<CSkeletonComponentBone3D>
{
public:
	CSkeletonComponentBone3D(const CSkeletonBone& Bone);
	virtual ~CSkeletonComponentBone3D();

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
	CSkeletonBone                                  m_Bone;
	std::weak_ptr<ISkeletonComponentBone3D>        m_ParentBone;
	std::vector<std::shared_ptr<ISkeletonComponentBone3D>> m_Childs;
	glm::vec3                                      m_PivotPoint;
	glm::mat4                                      m_Matrix;
	glm::mat4                                      m_RotateMatrix;
	bool                                           m_IsCalculated;
};
