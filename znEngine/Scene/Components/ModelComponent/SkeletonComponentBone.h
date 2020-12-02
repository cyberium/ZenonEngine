#pragma once

#include "SkeletonBone.h"

class ZN_API CSkeletonComponentBone3D
	: public ISkeletonComponentBone3D
	, public ISkeletonComponentBoneInternal3D
	, public std::enable_shared_from_this<CSkeletonComponentBone3D>
{
public:
	CSkeletonComponentBone3D(const std::shared_ptr<ISkeletonBone>& Bone);
	virtual ~CSkeletonComponentBone3D();

	// ISkeletonComponentBone3D
	std::string GetName() const override;
	const std::weak_ptr<ISkeletonComponentBone3D>& GetParentBone() const override;
	const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& GetChilds() const override;
	const ISkeletonBone& GetProtoBone() const override;
	
	const glm::mat4& GetCalculatedMatrix() const override;

	// ISkeletonComponentBoneInternal3D
	void AddChildInternal(const std::shared_ptr<ISkeletonComponentBone3D>& Child) override;
	void SetParentAndChildsInternals(const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& Bones) override;
	void Calculate(const IModelsComponent3D* ModelsComponent, const ICameraComponent3D* Camera) override;
	void Reset() override;

protected:
	std::shared_ptr<ISkeletonBone>                         m_ProtoBone;
	std::weak_ptr<ISkeletonComponentBone3D>                m_ParentBone;
	std::vector<std::shared_ptr<ISkeletonComponentBone3D>> m_Childs;

	// Dynamic
	glm::mat4                                              m_CalculatedMatrix;
	bool                                                   m_IsCalculated;
};
