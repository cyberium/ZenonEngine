#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

// FORWARD BEGIN
class CFBXSceneNode;
// FORWARD END

class CFBXMesh
	: public ModelProxie
{
public:
	CFBXMesh(const IBaseManager& BaseManager, std::weak_ptr<CFBXSceneNode> OwnerFBXNode);
	virtual ~CFBXMesh();

	void Load(fbxsdk::FbxMesh* NativeMesh);

protected:
	void DisplayMaterialConnections(fbxsdk::FbxMesh* NativeMesh);
	void DisplayMaterialMapping(fbxsdk::FbxGeometryElementMaterial* materialElement);

private:
	std::weak_ptr<CFBXSceneNode> m_OwnerFBXNode;

private:
	const IBaseManager& m_BaseManager;
};

#endif
