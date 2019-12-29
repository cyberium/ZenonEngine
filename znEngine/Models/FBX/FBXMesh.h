#pragma once

#include <fbxsdk.h>

// FORWARD BEGIN
class CFBXScene;
// FORWARD END

class CFBXMesh
	: public MeshProxie
{
public:
	CFBXMesh(const IBaseManager* BaseManager, std::shared_ptr<CFBXScene> OwnerScene, fbxsdk::FbxMesh* NativeMesh);
	virtual ~CFBXMesh();

	void Load();

private:
	std::shared_ptr<CFBXScene> m_OwnerScene;
	fbxsdk::FbxMesh* m_NativeMesh;

private:
	const IBaseManager* m_BaseManager;
};