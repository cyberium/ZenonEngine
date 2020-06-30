#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

#include "FBXInterfaces.h"
#include "FBXScene.h"
#include "FBXSceneNode.h"

class CFBXManager
{
public:
	CFBXManager(const IBaseManager& BaseManager);
	virtual ~CFBXManager();

	std::shared_ptr<CFBXSceneNode> CreateSceneNode(IScene* Scene, std::string SceneName);

private:
	fbxsdk::FbxManager* m_FBXManager;
	const IBaseManager& m_BaseManager;
};

#endif
