#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

#include "FBXInterfaces.h"
#include "FBXMaterial.h"

// FORWARD BEGIN
class CFBXScene;
// FORWARD END

class ZN_API CFBXSceneNode
	: public SceneNode3D
	, public IFBXSceneNode3D
{
public:
	CFBXSceneNode(const IBaseManager& BaseManager, fbxsdk::FbxManager* FBXManager);
	virtual ~CFBXSceneNode();

	void InitializeFromFile(const std::string& FileName);
	void LoadNode(fbxsdk::FbxNode * NativeNode);
	
	const std::vector<std::shared_ptr<CFBXMaterial>>& GetMaterials() const;
	std::shared_ptr<CFBXMaterial> GetMaterial(int Index) const;
	std::shared_ptr<IModel> GetModel() const;

protected:
	void LoadChilds(fbxsdk::FbxNode * NativeNode);
	void LoadMaterials(fbxsdk::FbxNode * NativeNode);
	void LoadModel(fbxsdk::FbxNode * NativeNode);
	void LoadLight(fbxsdk::FbxNode * NativeNode);

private:
	std::vector<std::shared_ptr<CFBXMaterial>> m_MaterialsArray;
	std::shared_ptr<IModel> m_Model;

private:
	const IBaseManager& m_BaseManager;
	fbxsdk::FbxManager* m_FBXManager;
};

#endif
