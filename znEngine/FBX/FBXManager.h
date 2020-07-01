#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

#include "FBXInterfaces.h"
#include "FBXScene.h"
#include "FBXSceneNode.h"

class CFBXManager
	: public IFBXManager
{
public:
	CFBXManager(const IBaseManager& BaseManager);
	virtual ~CFBXManager();

	// IFBXManager
	std::shared_ptr<IFBXSceneNode3D> CreateSceneNode(IScene* Scene, std::string SceneName) override;

private:
	fbxsdk::FbxManager* m_FBXManager;
	const IBaseManager& m_BaseManager;
};

#endif
