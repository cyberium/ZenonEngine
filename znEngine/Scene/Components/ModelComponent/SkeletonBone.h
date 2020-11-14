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
	int32 GetParentIndex() const override;

	void SetLocalMatrix(const glm::mat4& Matrix) override;
	glm::mat4 GetLocalMatrix() const override;

	void SetPivotMatrix(const glm::mat4& Matrix) override;
	glm::mat4 GetPivotMatrix() const override;

	void SetFuckingMatrix(const glm::mat4& Matrix) override;
	glm::mat4 GetFuckingMatrix() const override;

	glm::mat4 CalcMatrix(const IModelsComponent3D* ModelsComponent) const override;
	glm::mat4 CalcRotateMatrix(const IModelsComponent3D* ModelsComponent) const override;

	std::string          Name;
	int32                ParentIndex;

	glm::mat4            LocalTransform;
	glm::mat4            PivotMatrix;
	glm::mat4            FuckingMatrix;
	

	CznAnimatedValue<float> pX, pY, pZ;
	CznAnimatedValue<float> rX, rY, rZ;
	CznAnimatedValue<float> sX, sY, sZ;

	CznAnimatedValue<glm::mat4> m_CalculatedMatrixes;
};