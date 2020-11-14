#pragma once

#include "AnimatedValue.h"

class ZN_API CSkeletonBone
	: public ISkeletonBone
{
public:
	CSkeletonBone(const std::string& Name, int32 ParentIndex);
	virtual ~CSkeletonBone();

	// ISkeletonBone
	void MergeWithOther(std::shared_ptr<ISkeletonBone> OtherBone) override;
	std::string GetName() const override;
	glm::vec3 GetPivotPoint() const override;
	int32 GetParentIndex() const override;
	glm::mat4 CalcMatrix(const IModelsComponent3D* ModelsComponent) const override;
	glm::mat4 CalcRotateMatrix(const IModelsComponent3D* ModelsComponent) const override;

	std::string          Name;
	int32                ParentIndex;
	glm::mat4            GlobalTransform;
	glm::mat4            LocalTransform;

	CznAnimatedValue<float> pX, pY, pZ;
	CznAnimatedValue<float> rX, rY, rZ;
	CznAnimatedValue<float> sX, sY, sZ;
};