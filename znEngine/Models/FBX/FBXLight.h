#pragma once

#include <fbxsdk.h>

// FORWARD BEGIN
class CFBXSceneNode;
// FORWARD END

class CFBXLight
	: public CLight3D
{
public:
	CFBXLight(const IBaseManager* BaseManager, std::weak_ptr<CFBXSceneNode> OwnerFBXNode);
	virtual ~CFBXLight();

	void Load(fbxsdk::FbxLight* NativeLight);

private:
	std::weak_ptr<CFBXSceneNode> m_OwnerFBXNode;

private:
	const IBaseManager* m_BaseManager;
};