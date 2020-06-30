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
	CFBXMesh(const IBaseManager& BaseManager);
	virtual ~CFBXMesh();

	void Load(const CFBXSceneNode& FBXNode, fbxsdk::FbxMesh* NativeMesh);

protected:
	void DisplayMaterialConnections(const CFBXSceneNode& FBXNode, fbxsdk::FbxMesh* NativeMesh);
	void DisplayMaterialMapping(fbxsdk::FbxGeometryElementMaterial* materialElement);

private:
	std::shared_ptr<IGeometry> m_Geometry;

private:
	const IBaseManager& m_BaseManager;
};

#endif
