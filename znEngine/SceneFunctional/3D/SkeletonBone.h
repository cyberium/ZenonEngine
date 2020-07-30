#pragma once

#include "AnimatedValue.h"

struct CSkeletonBone
{
	CSkeletonBone(const std::string& Name, int32 ParentIndex);
	virtual ~CSkeletonBone();

	glm::mat4 CalcMatrix(const ISceneNode3D& Instance) const;

	std::string          Name;
	int32                ParentIndex;

	AnimatedValue<float> pX, pY, pZ;
	AnimatedValue<float> rX, rY, rZ;
	AnimatedValue<float> sX, sY, sZ;
};