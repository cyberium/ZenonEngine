#pragma once

#include <fbxsdk.h>
#include "Materials/MaterialModel.h"

// FORWARD BEGIN
class CFBXSceneNode;
// FORWARD END

class ZN_API CFBXMaterial
	: public MaterialModel
{
public:
	CFBXMaterial(const IBaseManager* BaseManager, std::weak_ptr<CFBXSceneNode> OwnerFBXNode);
	virtual ~CFBXMaterial();

	void Load(fbxsdk::FbxSurfaceMaterial* NativeMaterial);

protected:
	ITexture* LoadTexture(fbxsdk::FbxTexture* Texture);

private:
	std::weak_ptr<CFBXSceneNode> m_OwnerFBXNode;
};