#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

#include "Materials/MaterialModel.h"

class ZN_API CFBXMaterial
	: public MaterialModel
	, public IFBXMaterial
{
public:
	CFBXMaterial(const IBaseManager& BaseManager, const IFBXNode& FBXNode);
	virtual ~CFBXMaterial();

	void Load(fbxsdk::FbxSurfaceMaterial* NativeMaterial);

	// MaterialModel
	void SetTexture(ETextureType TextureType, std::shared_ptr<ITexture> texture) override;

	// IFBXMaterial
	std::shared_ptr<IMaterial> GetMaterial() override;

private:
	std::shared_ptr<ITexture> LoadTexture(fbxsdk::FbxTexture* Texture);

private:
	const IBaseManager& m_BaseManager;
	const IFBXNode& m_FBXNode;
};

#endif
