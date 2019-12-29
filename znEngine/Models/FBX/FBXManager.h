#pragma once

#include <fbxsdk.h>

#include "FBXScene.h"

class CFBXManager
{
public:
	CFBXManager(const IBaseManager* BaseManager);
	virtual ~CFBXManager();

	std::shared_ptr<CFBXScene> CreateScene(std::string SceneName);

private:
	fbxsdk::FbxManager* m_SdkManager;
	const IBaseManager* m_BaseManager;
};