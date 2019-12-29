#include "stdafx.h"

// General
#include "FBXHelpers.h"

void DisplayMetaData(FbxScene* pScene)
{
	FbxDocumentInfo* sceneInfo = pScene->GetSceneInfo();
	if (sceneInfo)
	{
		Log::Print("-------------------- Meta-Data --------------------");
		Log::Print("    Title: %s\n", sceneInfo->mTitle.Buffer());
		Log::Print("    Subject: %s\n", sceneInfo->mSubject.Buffer());
		Log::Print("    Author: %s\n", sceneInfo->mAuthor.Buffer());
		Log::Print("    Keywords: %s\n", sceneInfo->mKeywords.Buffer());
		Log::Print("    Revision: %s\n", sceneInfo->mRevision.Buffer());
		Log::Print("    Comment: %s\n", sceneInfo->mComment.Buffer());

		FbxThumbnail* thumbnail = sceneInfo->GetSceneThumbnail();
		if (thumbnail)
		{
			Log::Print("    Thumbnail:\n");

			switch (thumbnail->GetDataFormat())
			{
			case FbxThumbnail::eRGB_24:
				Log::Print("        Format: RGB\n");
				break;
			case FbxThumbnail::eRGBA_32:
				Log::Print("        Format: RGBA\n");
				break;
			}

			switch (thumbnail->GetSize())
			{
			default:
				break;
			case FbxThumbnail::eNotSet:
				Log::Print("        Size: no dimensions specified (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e64x64:
				Log::Print("        Size: 64 x 64 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
				break;
			case FbxThumbnail::e128x128:
				Log::Print("        Size: 128 x 128 pixels (%ld bytes)\n", thumbnail->GetSizeInBytes());
			}
		}
	}
}
