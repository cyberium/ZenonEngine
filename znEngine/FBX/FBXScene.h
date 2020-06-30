#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

#include "FBXSceneNode.h"

class CFBXScene 
	: public std::enable_shared_from_this<CFBXScene>
{
public:
	CFBXScene(const IBaseManager& BaseManager, fbxsdk::FbxManager* FBXManager);
	virtual ~CFBXScene();

	bool LoadFromFile(std::shared_ptr<IFile> File);

public:
	fbxsdk::FbxScene*   GetNativeScene() const;
	fbxsdk::FbxManager* GetNativeManager() const;
	std::string         GetPath() const;
	std::shared_ptr<CFBXSceneNode> GetRootNode() const;

private:
	std::string m_Path;
	fbxsdk::FbxScene* m_NativeScene;

	std::shared_ptr<CFBXSceneNode> m_RootNode;

private:
	const IBaseManager& m_BaseManager;
};

#endif
