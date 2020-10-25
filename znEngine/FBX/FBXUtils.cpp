#include "stdafx.h"

// General
#include "FBXUtils.h"

float ToFloat(const fbxsdk::FbxPropertyT<fbxsdk::FbxDouble>& FBXDouble)
{
	return FBXDouble.Get();
}

glm::vec3 ToGLMVec3(const fbxsdk::FbxPropertyT<fbxsdk::FbxDouble3>& FBXVec3)
{
	return glm::vec3(FBXVec3.Get()[0], FBXVec3.Get()[1], FBXVec3.Get()[2]);
}

glm::mat4 ToGLMMat4(const fbxsdk::FbxAMatrix& FBXMatrix)
{
	glm::mat4 glmMatrix;
	for (uint32 i = 0; i < 4; i++)
		for (uint32 j = 0; j < 4; j++)
			glmMatrix[i][j] = FBXMatrix[i][j];

	return glmMatrix;
}
