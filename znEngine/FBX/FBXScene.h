#pragma once

#include <fbxsdk.h>

#include "FBXSceneNode.h"



class CFBXScene : public std::enable_shared_from_this<CFBXScene>
{
public:
	CFBXScene(const IBaseManager* BaseManager, fbxsdk::FbxScene* NativeScene);
	virtual ~CFBXScene();

	bool LoadFromFile(std::shared_ptr<IFile> File);
	bool SaveToFile(std::shared_ptr<IFile> File, int pFileFormat = -1, bool pEmbedMedia = false);

	bool LoadNodes(std::shared_ptr<ISceneNode> ParentNode);

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
	const IBaseManager* m_BaseManager;
};
