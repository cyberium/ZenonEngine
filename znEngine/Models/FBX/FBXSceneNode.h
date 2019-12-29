#pragma once

#include <fbxsdk.h>

// FORWARD BEGIN
class CFBXScene;
// FORWARD END

class ZN_API CFBXSceneNode
	: public SceneNode3D
{
public:
	CFBXSceneNode(std::shared_ptr<CFBXScene> OwnerScene, fbxsdk::FbxNode * NativeNode);
	virtual ~CFBXSceneNode();

	void LoadNode(fbxsdk::FbxNode * NativeNode);

	// ISceneNode
	void Initialize() override;

private:
	std::shared_ptr<CFBXScene> m_OwnerScene;
	fbxsdk::FbxNode* m_NativeNode;

private:
	std::string m_SceneFilename;
};