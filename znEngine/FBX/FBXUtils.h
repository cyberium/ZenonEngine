#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

float ToFloat(const fbxsdk::FbxPropertyT<fbxsdk::FbxDouble>& FBXDouble);
glm::vec3 ToGLMVec3(const fbxsdk::FbxPropertyT<fbxsdk::FbxDouble3>& FBXVec3);

glm::mat4 ToGLMMat4(const fbxsdk::FbxAMatrix& FBXMatrix);

#endif