#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

#include "FBXMaterial.h"

// FORWARD BEGIN
class CFBXScene;
// FORWARD END

class ZN_API CFBXSceneNode
	: public SceneNode3D
{
public:
	CFBXSceneNode(const IBaseManager& BaseManager, std::weak_ptr<CFBXScene> OwnerScene, fbxsdk::FbxNode * NativeNode);
	virtual ~CFBXSceneNode();

	void LoadNode();

	bool LoadMaterials();
	const std::vector<std::shared_ptr<CFBXMaterial>>& GetMaterials() const;
	std::shared_ptr<CFBXMaterial> GetMaterial(int Index) const;

	std::weak_ptr<CFBXScene> GetOwnerScene() const;
	fbxsdk::FbxNode* GetNativeNode() const;

private:
	std::vector<std::shared_ptr<CFBXMaterial>> m_MaterialsArray;

private:
	std::weak_ptr<CFBXScene> m_OwnerScene;
	fbxsdk::FbxNode* m_NativeNode;

private:
	const IBaseManager& m_BaseManager;
};

#endif
