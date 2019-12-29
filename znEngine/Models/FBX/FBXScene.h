#pragma once

#include <fbxsdk.h>

#include "FBXSceneNode.h"
#include "FBXMaterial.h"
#include "FBXMesh.h"

class CFBXScene : public std::enable_shared_from_this<CFBXScene>
{
public:
	CFBXScene(const IBaseManager* BaseManager, fbxsdk::FbxScene* NativeScene);
	virtual ~CFBXScene();

	bool LoadFromFile(std::shared_ptr<IFile> File);
	bool SaveToFile(std::shared_ptr<IFile> File, int pFileFormat = -1, bool pEmbedMedia = false);

	bool Load();

	bool LoadNodes();

	bool LoadTextures();
	const std::vector<std::shared_ptr<ITexture>>& GetTextures() const;
	std::shared_ptr<ITexture> GetTexture(int Index) const;

	bool LoadMaterials();
	const std::vector<std::shared_ptr<CFBXMaterial>>& GetMaterials() const;
	std::shared_ptr<CFBXMaterial> GetMaterial(int Index) const;

	bool LoadMeshes();
	const std::vector<std::shared_ptr<CFBXMesh>>& GetMeshes() const;
	std::shared_ptr<CFBXMesh> GetMesh(int Index) const;

public:
	fbxsdk::FbxScene*   GetNativeScene() const;
	fbxsdk::FbxManager* GetNativeManager() const;

private:
	std::string m_Path;
	fbxsdk::FbxScene* m_NativeScene;

	std::shared_ptr<CFBXSceneNode> m_RootNode;
	std::vector<std::shared_ptr<ITexture>> m_TexturesArray;
	std::vector<std::shared_ptr<CFBXMaterial>> m_MaterialsArray;
	std::vector<std::shared_ptr<CFBXMesh>> m_MeshesArray;

private:
	const IBaseManager* m_BaseManager;
};