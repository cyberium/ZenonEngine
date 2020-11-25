#include "stdafx.h"

// General
#include "FBXMaterials.h"

// Additional
#include "FBXUtils.h"


namespace
{
	struct
	{
		fbxsdk::FbxLayerElement::EType FBXType;
		MaterialModel::ETextureType EngineType;
	} TextureTypeMapping[] =
	{
		{ fbxsdk::FbxLayerElement::EType::eTextureDiffuse, MaterialModel::ETextureType::TextureDiffuse },
		{ fbxsdk::FbxLayerElement::EType::eTextureEmissive, MaterialModel::ETextureType::TextureEmissive },
		{ fbxsdk::FbxLayerElement::EType::eTextureAmbient, MaterialModel::ETextureType::TextureAmbient },
		{ fbxsdk::FbxLayerElement::EType::eTextureSpecular, MaterialModel::ETextureType::TextureSpecular },
		{ fbxsdk::FbxLayerElement::EType::eTextureShininess, MaterialModel::ETextureType::TextureShininess },
		{ fbxsdk::FbxLayerElement::EType::eTextureNormalMap, MaterialModel::ETextureType::TextureNormalMap },
		{ fbxsdk::FbxLayerElement::EType::eTextureBump, MaterialModel::ETextureType::TextureBump },
		{ fbxsdk::FbxLayerElement::EType::eTextureReflection, MaterialModel::ETextureType::TextureReflection}
	};

	MaterialModel::ETextureType GetEngineTextureTypeByFBXLayerType(fbxsdk::FbxLayerElement::EType FBXType)
	{
		for (size_t i = 0; i < _countof(TextureTypeMapping); i++)
			if (FBXType == TextureTypeMapping[i].FBXType)
				return TextureTypeMapping[i].EngineType;
		throw CException("Unknown '%d' FBX texture type.", FBXType);
	}
}


CFBXMaterials::CFBXMaterials(const IBaseManager& BaseManager, const IFBXScene& FBXScene)
	: m_BaseManager(BaseManager)
	, m_FBXScene(FBXScene)
{
}

CFBXMaterials::~CFBXMaterials()
{
}



//
// CFBXMaterials
//
void CFBXMaterials::Load(fbxsdk::FbxScene * FBXScene)
{
	Log::Green("FBXMaterials: Loading...");

	int fbxMaterialsCount = FBXScene->GetMaterialCount();
	Log::Print("FBXMaterials: Materials count: '%d'.", fbxMaterialsCount);

	for (int s = 0; s < fbxMaterialsCount; s++)
	{
		fbxsdk::FbxSurfaceMaterial* fbxSurfaceMaterial = FBXScene->GetMaterial(s);
		Log::Print("FBXMaterials:    Process '%s' material.", fbxSurfaceMaterial->GetName());

		auto mat = LoadMaterial(fbxSurfaceMaterial);
		m_Materials.insert(std::make_pair(mat->GetName(), mat));
	}

	Log::Print("FBXMaterials: Loaded '%d' materials.", m_Materials.size());
	Log::Print("");
}



//
// IFBXMaterials
//
std::shared_ptr<IMaterial> CFBXMaterials::GetMaterial(const std::string & MaterialName) const
{
	return m_Materials.at(MaterialName);
}



//
// Protected
//
std::shared_ptr<MaterialModel> CFBXMaterials::LoadMaterial(fbxsdk::FbxSurfaceMaterial * FBXMaterial)
{
	if (false == FBXMaterial->Is<fbxsdk::FbxSurfacePhong>())
		throw CException("Material '%s' isn't 'FbxSurfacePhong'.", FBXMaterial->GetName());

	fbxsdk::FbxSurfacePhong* fbxPhongMaterial = fbxsdk::FbxCast<fbxsdk::FbxSurfacePhong>(FBXMaterial);
	_ASSERT(fbxPhongMaterial != nullptr);

	std::shared_ptr<MaterialModel> material = MakeShared(MaterialModel, m_BaseManager);
	material->SetName(fbxPhongMaterial->GetName());

	LoadPhongMaterialsProperties(fbxPhongMaterial, material);
	LoadMaterialsTextures(fbxPhongMaterial, material);

	return material;
}

void CFBXMaterials::LoadPhongMaterialsProperties(fbxsdk::FbxSurfacePhong * FBXSurfacePhong, std::shared_ptr<MaterialModel> Material)
{
	Material->SetAmbientColor(ToGLMVec3(FBXSurfacePhong->Ambient));
	Material->SetAmbientFactor(ToFloat(FBXSurfacePhong->AmbientFactor));

	Material->SetDiffuseColor(ToGLMVec3(FBXSurfacePhong->Diffuse));
	Material->SetDiffuseFactor(ToFloat(FBXSurfacePhong->DiffuseFactor));

	Material->SetSpecularColor(ToGLMVec3(FBXSurfacePhong->Specular));
	Material->SetSpecularFactor(ToFloat(FBXSurfacePhong->SpecularFactor)) /** 16.0f*/;

	//MaterialData().NormalMap = ToGLMVec3(FBXSurfacePhong->NormalMap);
	//MaterialData().Shininess = ToFloat(FBXSurfacePhong->Shininess);

	//MaterialData().TransparentColor = ToGLMVec3(FBXSurfacePhong->TransparentColor);
	//MaterialData().TransparencyFactor = ToFloat(FBXSurfacePhong->TransparencyFactor);

	//MaterialData().DisplacementColor = ToGLMVec3(FBXSurfacePhong->DisplacementColor);
	//MaterialData().DisplacementFactor = ToFloat(FBXSurfacePhong->DisplacementFactor);

	//MaterialData().VectorDisplacementColor = ToGLMVec3(FBXSurfacePhong->VectorDisplacementColor);
	//MaterialData().VectorDisplacementFactor = ToFloat(FBXSurfacePhong->VectorDisplacementFactor);

	//Material->SetEmissiveColor(ToGLMVec3(FBXSurfacePhong->Emissive));
	//MaterialData().EmissiveFactor = ToFloat(FBXSurfacePhong->EmissiveFactor);

	//Material->SetBump(ToGLMVec3(FBXSurfacePhong->Bump));
	Material->SetBumpFactor(ToFloat(FBXSurfacePhong->BumpFactor));

	//Material->SetReflectionColor(ToGLMVec3(FBXSurfacePhong->Reflection));
	//Material->SetReflectionFactor(ToFloat(FBXSurfacePhong->ReflectionFactor));
}

void CFBXMaterials::LoadMaterialsTextures(fbxsdk::FbxSurfacePhong * FBXSurfacePhong, std::shared_ptr<MaterialModel> Material)
{
	for (int j = 0; j < fbxsdk::FbxLayerElement::sTypeTextureCount; j++)
	{
		const char* channelName = fbxsdk::FbxLayerElement::sTextureChannelNames[j];

		fbxsdk::FbxProperty fbxProperty = FBXSurfacePhong->FindProperty(channelName);
		if (false == fbxProperty.IsValid())
			continue;

		/*
		FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
		if (lLayeredTexture)
		{
			DisplayInt("    Layered Texture: ", j);
			int lNbTextures = lLayeredTexture->GetSrcObjectCount<FbxTexture>();
			for (int k = 0; k < lNbTextures; ++k)
			{
				FbxTexture* lTexture = lLayeredTexture->GetSrcObject<FbxTexture>(k);
				if (lTexture)
				{

					//NOTE the blend mode is ALWAYS on the LayeredTexture and NOT the one on the texture.
					//Why is that?  because one texture can be shared on different layered textures and might
					//have different blend modes.

					FbxLayeredTexture::EBlendMode lBlendMode;
					lLayeredTexture->GetTextureBlendMode(k, lBlendMode);
					DisplayString("    Textures for ", pProperty.GetName());
					DisplayInt("        Texture ", k);



					const char* lBlendModes[] = { "Translucent", "Additive", "Modulate", "Modulate2", "Over", "Normal", "Dissolve", "Darken", "ColorBurn", "LinearBurn",
													"DarkerColor", "Lighten", "Screen", "ColorDodge", "LinearDodge", "LighterColor", "SoftLight", "HardLight", "VividLight",
													"LinearLight", "PinLight", "HardMix", "Difference", "Exclusion", "Substract", "Divide", "Hue", "Saturation", "Color",
													"Luminosity", "Overlay" };
					DisplayString("			Blend Mode: ", lBlendModes[lBlendMode]);

					return DisplayTextureInfo(lTexture);
				}

			}
		}
		*/


		if (fbxProperty.GetSrcObjectCount<fbxsdk::FbxTexture>() == 0)
		{
			continue;
		}
		else if (fbxProperty.GetSrcObjectCount<fbxsdk::FbxTexture>() == 1)
		{
			fbxsdk::FbxTexture* fbxTexture = fbxProperty.GetSrcObject<fbxsdk::FbxTexture>(0);
			_ASSERT(fbxTexture != NULL);

			MaterialModel::ETextureType textureType = GetEngineTextureTypeByFBXLayerType(FBXSDK_TEXTURE_TYPE(j));

			// Replace texture type
			auto loaderParams = m_FBXScene.GetLoaderParams();
			if (auto loaderFBXParams = dynamic_cast<const CznFBXLoaderParams*>(loaderParams))
			{
				const auto& it = loaderFBXParams->TexturesTypeChange.find((uint8)textureType);
				if (it != loaderFBXParams->TexturesTypeChange.end())
					textureType = (MaterialModel::ETextureType)it->second;
			}

			auto texture = LoadTexture(fbxTexture);

			Material->SetTexture(textureType, texture);
		}
		else
			throw CException("CFBXMaterial: Material '%s' texture '%s' (%s) has more than one texture object.", FBXSurfacePhong->GetName(), fbxProperty.GetName().Buffer(), fbxsdk::FbxLayerElement::sTextureChannelNames[j]);
	}
}

std::shared_ptr<ITexture> CFBXMaterials::LoadTexture(fbxsdk::FbxTexture * Texture)
{
	Log::Print("FBXMaterials:       Loading texture '%s'.", Texture->GetName());

	if (false == Texture->Is<fbxsdk::FbxFileTexture>())
		throw CException("Texture '%s' isn't 'FbxFileTexture'.", Texture->GetName());

	fbxsdk::FbxFileTexture* fileTexture = fbxsdk::FbxCast<fbxsdk::FbxFileTexture>(Texture);

	// For exporter
	//fileTexture->SetFileName(fileTexture->GetRelativeFileName());

	std::string fileName = Utils::FixFilePath(fileTexture->GetFileName());
	Log::Print("FBXMaterials:       Filename: '%s'", fileName.c_str());

	std::string relativeFileName = Utils::FixFilePath(fileTexture->GetRelativeFileName());
	Log::Print("FBXMaterials:       Relative filename '%s'.", relativeFileName.c_str());

	std::string texturesRootDirForRelative = m_FBXScene.GetFBXFilePath();

	auto loaderParams = m_FBXScene.GetLoaderParams();
	if (auto loaderFBXParams = dynamic_cast<const CznFBXLoaderParams*>(loaderParams))
	{
		std::string overrridenTexture = loaderFBXParams->OverrideTexture;
		if (overrridenTexture.size() > 0)
			relativeFileName = overrridenTexture;

		std::string texturesRoot = loaderFBXParams->TexturesPathRoot;
		if (texturesRoot.size() > 0)
			texturesRootDirForRelative = texturesRoot;
	}

	std::string resultTextureFullname = texturesRootDirForRelative + "/" + relativeFileName;

	Log::Print("FBXMaterials:       Result filename '%s'.", resultTextureFullname.c_str());
	return m_BaseManager.GetManager<IznTexturesFactory>()->LoadTexture2D(resultTextureFullname);
}
