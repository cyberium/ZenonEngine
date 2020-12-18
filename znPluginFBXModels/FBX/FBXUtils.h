#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

inline float ToFloat(const fbxsdk::FbxPropertyT<fbxsdk::FbxDouble>& FBXDouble)
{
	return FBXDouble.Get();
}

inline glm::vec3 ToGLMVec3(const fbxsdk::FbxPropertyT<fbxsdk::FbxDouble3>& FBXVec3)
{
	return glm::vec3(FBXVec3.Get()[0], FBXVec3.Get()[1], FBXVec3.Get()[2]);
}

inline glm::vec4 ToGLMVec4(const fbxsdk::FbxPropertyT<fbxsdk::FbxDouble4>& FBXVec4)
{
	return glm::vec4(FBXVec4.Get()[0], FBXVec4.Get()[1], FBXVec4.Get()[2], FBXVec4.Get()[3]);
}

inline ColorRGB ToColorRGB(const fbxsdk::FbxPropertyT<fbxsdk::FbxDouble3>& FBXVec3)
{
	return ColorRGB(FBXVec3.Get()[0], FBXVec3.Get()[1], FBXVec3.Get()[2]);
}

inline ColorRGBA ToColorRGBA(const fbxsdk::FbxPropertyT<fbxsdk::FbxDouble4>& FBXVec4)
{
	return ColorRGBA(FBXVec4.Get()[0], FBXVec4.Get()[1], FBXVec4.Get()[2], FBXVec4.Get()[3]);
}

inline glm::mat4 ToGLMMat4(const fbxsdk::FbxAMatrix& FBXMatrix)
{
	glm::mat4 glmMatrix;
	for (uint32 i = 0; i < 4; i++)
		for (uint32 j = 0; j < 4; j++)
			glmMatrix[i][j] = FBXMatrix[i][j];
	return glmMatrix;
}

#endif