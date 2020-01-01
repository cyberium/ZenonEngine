#pragma once

#include <fbxsdk.h>

// FORWARD BEGIN
class CFBXSceneNode;
// FORWARD END

class CFBXMesh
	: public MeshProxie
{
public:
	CFBXMesh(const IBaseManager* BaseManager, std::weak_ptr<CFBXSceneNode> OwnerFBXNode, fbxsdk::FbxMesh* NativeMesh);
	virtual ~CFBXMesh();

	void Load();

protected:
	void DisplayMaterialMapping(fbxsdk::FbxGeometryElementMaterial* materialElement);
	void DisplayMaterialConnections();

private:
	std::weak_ptr<CFBXSceneNode> m_OwnerFBXNode;
	fbxsdk::FbxMesh* m_NativeMesh;

private:
	const IBaseManager* m_BaseManager;
};