#pragma once

#include <fbxsdk.h>

class CFBXManager
{
public:
	CFBXManager();
	virtual ~CFBXManager();

	fbxsdk::FbxScene* CreateScene(std::string SceneName);
	bool LoadScene(fbxsdk::FbxScene*& pScene, const char* pFilename);
	bool SaveScene(fbxsdk::FbxScene*& pScene, const char* pFilename, int pFileFormat = -1, bool pEmbedMedia = false);
	

private:
	fbxsdk::FbxManager* m_SdkManager;
};