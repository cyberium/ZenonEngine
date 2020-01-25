#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

void DisplayMetaData(fbxsdk::FbxScene* pScene);
void DisplayMetaDataConnections(fbxsdk::FbxObject* pNode);

void DisplayHierarchy(fbxsdk::FbxScene* pScene);
void DisplayHierarchy(fbxsdk::FbxNode* pNode, int pDepth);

void DisplayString(const char* pHeader, const char* pValue = "", const char* pSuffix = "");
void DisplayBool(const char* pHeader, bool pValue, const char* pSuffix = "");
void DisplayInt(const char* pHeader, int pValue, const char* pSuffix = "");
void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix = "");
void Display2DVector(const char* pHeader, FbxVector2 pValue, const char* pSuffix = "");
void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix = "");
void DisplayColor(const char* pHeader, FbxColor pValue, const char* pSuffix = "");
void Display4DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix = "");

#endif
