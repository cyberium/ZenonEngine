#pragma once

#include "AnimatedValue.h"

struct ZN_API CSkeletonBone
{
	CSkeletonBone(const std::string& Name, int32 ParentIndex);
	virtual ~CSkeletonBone();

	void MergeWithOther(const CSkeletonBone & other);

	bool operator==(const CSkeletonBone& other) const;

	glm::mat4 CalcMatrix(const ISceneNode3D& Instance) const;
	glm::vec3 CalcTranslate(const ISceneNode3D& Instance) const;
	glm::mat4 CalcRotate(const ISceneNode3D& Instance) const;

	std::string          Name;
	int32                ParentIndex;
	glm::mat4            GlobalTransform;
	glm::mat4            LocalTransform;

	AnimatedValue<float> pX, pY, pZ;
	AnimatedValue<float> rX, rY, rZ;
	AnimatedValue<float> sX, sY, sZ;
	AnimatedValue<glm::mat4> mM;
};