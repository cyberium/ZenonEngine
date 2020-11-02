#pragma once

#include "../Animation/AnimatedValue.h"

struct ZN_API CSkeletonBone
{
	CSkeletonBone(const std::string& Name, int32 ParentIndex);
	virtual ~CSkeletonBone();

	void MergeWithOther(const CSkeletonBone & other);

	bool operator==(const CSkeletonBone& other) const;

	glm::mat4 CalcMatrix(const ISceneNode& Instance) const;
	glm::vec3 CalcTranslate(const ISceneNode& Instance) const;
	glm::vec3 CalcRotate(const ISceneNode& Instance) const;

	std::string          Name;
	int32                ParentIndex;
	glm::mat4            GlobalTransform;
	glm::mat4            LocalTransform;
	glm::mat4            LocalTransform222;

	CznAnimatedValue<float> pX, pY, pZ;
	CznAnimatedValue<float> rX, rY, rZ;
	CznAnimatedValue<float> sX, sY, sZ;
	CznAnimatedValue<glm::mat4> mM;
};