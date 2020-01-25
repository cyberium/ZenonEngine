#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXDisplayCommon.h"

void DisplayMetaData(FbxScene* pScene)
{
	FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
	if (!sceneInfo)
	{
		return;
	}

	Log::Print("-------------------- Meta-Data --------------------");
	Log::Print("    Title: %s", sceneInfo->mTitle.Buffer());
	Log::Print("    Subject: %s", sceneInfo->mSubject.Buffer());
	Log::Print("    Author: %s", sceneInfo->mAuthor.Buffer());
	Log::Print("    Keywords: %s", sceneInfo->mKeywords.Buffer());
	Log::Print("    Revision: %s", sceneInfo->mRevision.Buffer());
	Log::Print("    Comment: %s", sceneInfo->mComment.Buffer());

	FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
	if (thumbnail)
	{
		Log::Print("    Thumbnail:");

		switch (thumbnail->GetDataFormat())
		{
			case FbxThumbnail::eRGB_24:
				Log::Print("        Format: RGB");
				break;
			case FbxThumbnail::eRGBA_32:
				Log::Print("        Format: RGBA");
				break;
		}

		switch (thumbnail->GetSize())
		{
			case FbxThumbnail::eNotSet:
				Log::Print("        Size: no dimensions specified (%ld bytes)", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e64x64:
				Log::Print("        Size: 64 x 64 pixels (%ld bytes)", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e128x128:
				Log::Print("        Size: 128 x 128 pixels (%ld bytes)", thumbnail->GetSizeInBytes());
			default:
				break;
		}
	}

	Log::Print("---------------------------------------------------");
}

void DisplayMetaDataConnections(FbxObject* pObject)
{
	int nbMetaData = pObject->GetSrcObjectCount<FbxObjectMetaData>();
	if (nbMetaData > 0)
		DisplayString("    MetaData connections ");

	for (int i = 0; i < nbMetaData; i++)
	{
		FbxObjectMetaData* metaData = pObject->GetSrcObject<FbxObjectMetaData>(i);
		DisplayString("        Name: ", (char*)metaData->GetName());
	}
}



void DisplayHierarchy(FbxScene* pScene)
{
	Log::Info("Nodes hierarchy:");
	fbxsdk::FbxNode* lRootNode = pScene->GetRootNode();

	for (int i = 0; i < lRootNode->GetChildCount(); i++)
	{
		DisplayHierarchy(lRootNode->GetChild(i), 1);
	}
}

void DisplayHierarchy(fbxsdk::FbxNode* pNode, int pDepth)
{
	FbxString lString;

	for (int i = 0; i < pDepth; i++)
		lString += "     ";

	lString += pNode->GetName();
	Log::Print(lString.Buffer());

	for (int i = 0; i < pNode->GetChildCount(); i++)
		DisplayHierarchy(pNode->GetChild(i), pDepth + 1);
}



void DisplayString(const char* pHeader, const char* pValue /* = "" */, const char* pSuffix /* = "" */)
{
	FbxString lString;

	lString = pHeader;
	lString += pValue;
	lString += pSuffix;
	lString += "\n";
	Log::Print(lString);
}


void DisplayBool(const char* pHeader, bool pValue, const char* pSuffix /* = "" */)
{
	FbxString lString;

	lString = pHeader;
	lString += pValue ? "true" : "false";
	lString += pSuffix;
	Log::Print(lString);
}


void DisplayInt(const char* pHeader, int pValue, const char* pSuffix /* = "" */)
{
	FbxString lString;

	lString = pHeader;
	lString += pValue;
	lString += pSuffix;
	Log::Print(lString);
}


void DisplayDouble(const char* pHeader, double pValue, const char* pSuffix /* = "" */)
{
	FbxString lString;
	FbxString lFloatValue = (float)pValue;

	lFloatValue = pValue <= -HUGE_VAL ? "-INFINITY" : lFloatValue.Buffer();
	lFloatValue = pValue >= HUGE_VAL ? "INFINITY" : lFloatValue.Buffer();

	lString = pHeader;
	lString += lFloatValue;
	lString += pSuffix;
	Log::Print(lString);
}


void Display2DVector(const char* pHeader, FbxVector2 pValue, const char* pSuffix  /* = "" */)
{
	FbxString lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += pSuffix;
	Log::Print(lString);
}


void Display3DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix /* = "" */)
{
	FbxString lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];
	FbxString lFloatValue3 = (float)pValue[2];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();
	lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
	lFloatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : lFloatValue3.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += ", ";
	lString += lFloatValue3;
	lString += pSuffix;
	Log::Print(lString);
}

void Display4DVector(const char* pHeader, FbxVector4 pValue, const char* pSuffix /* = "" */)
{
	FbxString lString;
	FbxString lFloatValue1 = (float)pValue[0];
	FbxString lFloatValue2 = (float)pValue[1];
	FbxString lFloatValue3 = (float)pValue[2];
	FbxString lFloatValue4 = (float)pValue[3];

	lFloatValue1 = pValue[0] <= -HUGE_VAL ? "-INFINITY" : lFloatValue1.Buffer();
	lFloatValue1 = pValue[0] >= HUGE_VAL ? "INFINITY" : lFloatValue1.Buffer();
	lFloatValue2 = pValue[1] <= -HUGE_VAL ? "-INFINITY" : lFloatValue2.Buffer();
	lFloatValue2 = pValue[1] >= HUGE_VAL ? "INFINITY" : lFloatValue2.Buffer();
	lFloatValue3 = pValue[2] <= -HUGE_VAL ? "-INFINITY" : lFloatValue3.Buffer();
	lFloatValue3 = pValue[2] >= HUGE_VAL ? "INFINITY" : lFloatValue3.Buffer();
	lFloatValue4 = pValue[3] <= -HUGE_VAL ? "-INFINITY" : lFloatValue4.Buffer();
	lFloatValue4 = pValue[3] >= HUGE_VAL ? "INFINITY" : lFloatValue4.Buffer();

	lString = pHeader;
	lString += lFloatValue1;
	lString += ", ";
	lString += lFloatValue2;
	lString += ", ";
	lString += lFloatValue3;
	lString += ", ";
	lString += lFloatValue4;
	lString += pSuffix;
	Log::Print(lString);
}


void DisplayColor(const char* pHeader, FbxColor pValue, const char* pSuffix /* = "" */)
{
	FbxString lString;

	lString = pHeader;
	lString += (float)pValue.mRed;

	lString += " (red), ";
	lString += (float)pValue.mGreen;

	lString += " (green), ";
	lString += (float)pValue.mBlue;

	lString += " (blue)";
	lString += pSuffix;
	Log::Print(lString);
}

#endif
