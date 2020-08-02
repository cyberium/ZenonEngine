#pragma once

#include "AnimatedValue.h"

struct CSkeletonBone
{
	CSkeletonBone(const std::string& Name, int32 ParentIndex);
	virtual ~CSkeletonBone();

	void MergeWithOther(const CSkeletonBone & other);

	bool operator==(const CSkeletonBone& other) const;

	glm::vec3 CalcTranslate(const ISceneNode3D& Instance) const;
	glm::mat4 CalcMatrix(const ISceneNode3D& Instance) const;

	std::string          Name;
	int32                ParentIndex;
	glm::mat4            GlobalInverse;

	AnimatedValue<glm::mat4> mM;
	AnimatedValue<float> pX, pY, pZ;
	AnimatedValue<float> rX, rY, rZ;
	AnimatedValue<float> sX, sY, sZ;
};