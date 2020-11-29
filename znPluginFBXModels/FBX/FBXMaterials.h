#pragma once

#ifdef ZN_FBX_SDK_ENABLE
#include <fbxsdk.h>
#include "FBXInterfaces.h"

class CFBXMaterials
	: public IFBXMaterials
{
public:
	CFBXMaterials(const IBaseManager& BaseManager, const IFBXScene& FBXScene);
	virtual ~CFBXMaterials();

	// CFBXMaterials
	void Load(fbxsdk::FbxScene* FBXScene);

	// IFBXMaterials
	std::shared_ptr<IMaterial>      GetMaterial(const std::string& MaterialName) const override;

protected:
	std::shared_ptr<MaterialModel>  LoadMaterial(fbxsdk::FbxSurfaceMaterial* FBXMaterial);
	void                            LoadPhongMaterialsProperties(fbxsdk::FbxSurfacePhong* FBXSurfacePhong, std::shared_ptr<MaterialModel> Material);
	void                            LoadMaterialsTextures(fbxsdk::FbxSurfacePhong* FBXSurfacePhong, std::shared_ptr<MaterialModel> Material);
	std::shared_ptr<ITexture>       LoadTexture(std::shared_ptr<MaterialModel> OwnerMaterial, fbxsdk::FbxTexture* Texture);

private:
	std::unordered_map<std::string, std::shared_ptr<IMaterial>> m_Materials;

private:
	const IBaseManager& m_BaseManager;
	const IFBXScene& m_FBXScene;
};

#endif
