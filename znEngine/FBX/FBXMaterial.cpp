#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXMaterial.h"

// Additional
#include "FBXUtils.h"

CFBXMaterial::CFBXMaterial(const IBaseManager& BaseManager, const IFBXNode & FBXNode)
	: MaterialModel(BaseManager)
	, m_BaseManager(BaseManager)
	, m_FBXNode(FBXNode)
{
}

CFBXMaterial::~CFBXMaterial()
{
}

void CFBXMaterial::Load(fbxsdk::FbxSurfaceMaterial* NativeMaterial)
{
	Log::Print("CFBXNode: Loading material '%s'.", NativeMaterial->GetName());

	SetName(NativeMaterial->GetName());

	_ASSERT_EXPR(NativeMaterial->Is<fbxsdk::FbxSurfacePhong>(), "FBX material must be 'FbxSurfacePhong'.");
	fbxsdk::FbxSurfacePhong* surfacePhong = fbxsdk::FbxCast<fbxsdk::FbxSurfacePhong>(NativeMaterial);

	MaterialData().Emissive = ToGLMVec3(surfacePhong->Emissive);
	MaterialData().EmissiveFactor = ToFloat(surfacePhong->EmissiveFactor);

	MaterialData().Ambient = ToGLMVec3(surfacePhong->Ambient);
	MaterialData().AmbientFactor = ToFloat(surfacePhong->AmbientFactor);

	MaterialData().Diffuse = ToGLMVec3(surfacePhong->Diffuse);
	MaterialData().DiffuseFactor = ToFloat(surfacePhong->DiffuseFactor);

	MaterialData().NormalMap = ToGLMVec3(surfacePhong->NormalMap);
	MaterialData().Shininess = ToFloat(surfacePhong->Shininess);

	MaterialData().Bump = ToGLMVec3(surfacePhong->Bump);
	MaterialData().BumpFactor = ToFloat(surfacePhong->BumpFactor);

	MaterialData().TransparentColor = ToGLMVec3(surfacePhong->TransparentColor);
	MaterialData().TransparencyFactor = ToFloat(surfacePhong->TransparencyFactor);

	MaterialData().DisplacementColor = ToGLMVec3(surfacePhong->DisplacementColor);
	MaterialData().DisplacementFactor = ToFloat(surfacePhong->DisplacementFactor);

	MaterialData().VectorDisplacementColor = ToGLMVec3(surfacePhong->VectorDisplacementColor);
	MaterialData().VectorDisplacementFactor = ToFloat(surfacePhong->VectorDisplacementFactor);

	MaterialData().Specular = ToGLMVec3(surfacePhong->Specular);
	MaterialData().SpecularFactor = ToFloat(surfacePhong->SpecularFactor) /** 16.0f*/;

	MaterialData().Reflection = ToGLMVec3(surfacePhong->Reflection);
	MaterialData().ReflectionFactor = ToFloat(surfacePhong->ReflectionFactor);

	MarkMaterialDataDirty();

	PrintInfo();

	for (int j = 0; j < fbxsdk::FbxLayerElement::sTypeTextureCount; j++)
	{
		volatile auto channelName = fbxsdk::FbxLayerElement::sTextureChannelNames[j];
		fbxsdk::FbxProperty lProperty = surfacePhong->FindProperty(channelName);
		if (!lProperty.IsValid())
		{
			continue;
		}


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


		if (lProperty.GetSrcObjectCount<fbxsdk::FbxTexture>() == 0)
		{
			continue;
		}
		else if (lProperty.GetSrcObjectCount<fbxsdk::FbxTexture>() == 1)
		{
			fbxsdk::FbxTexture* lTexture = lProperty.GetSrcObject<fbxsdk::FbxTexture>(0);

			const auto& texture = LoadTexture(lTexture);

			fbxsdk::FbxLayerElement::EType texureType = FBXSDK_TEXTURE_TYPE(j);
			switch (texureType)
			{
			case fbxsdk::FbxLayerElement::EType::eTextureDiffuse:
				SetTexture(MaterialModel::ETextureType::TextureDiffuse, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureEmissive:
				SetTexture(MaterialModel::ETextureType::TextureEmissive, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureAmbient:
				SetTexture(MaterialModel::ETextureType::TextureAmbient, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureSpecular:
				SetTexture(MaterialModel::ETextureType::TextureSpecular, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureShininess:
				SetTexture(MaterialModel::ETextureType::TextureShininess, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureNormalMap:
				SetTexture(MaterialModel::ETextureType::TextureNormalMap, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureBump:
				SetTexture(MaterialModel::ETextureType::TextureBump, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureTransparency:
				SetTexture(MaterialModel::ETextureType::TextureTransparency, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureReflection:
				SetTexture(MaterialModel::ETextureType::TextureReflection, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureDisplacement:
				SetTexture(MaterialModel::ETextureType::TextureDisplacement, texture);
				break;

			default:
				Log::Error("CFBXMaterial: Unsupported texture type '%d'.", texureType);
			}
		}
		else
		{
			_ASSERT_EXPR(false, L"CFBXMaterial: Material '%s' texture '%s' (%s) has more than one texture object.", surfaceMaterial->GetName(), lProperty.GetName().Buffer(), fbxsdk::FbxLayerElement::sTextureChannelNames[j]);
		}
	}

	MarkMaterialDataDirty();
}



//
// MaterialModel
//
void CFBXMaterial::SetTexture(ETextureType TextureType, std::shared_ptr<ITexture> texture)
{
	ETextureType textureType = TextureType;

	auto loaderParams = m_FBXNode.GetFBXScene().GetLoaderParams();
	if (auto loaderFBXParams = std::dynamic_pointer_cast<CznFBXLoaderParams>(loaderParams))
	{
		const auto& it = loaderFBXParams->TexturesTypeChange.find((uint8)textureType);
		if (it != loaderFBXParams->TexturesTypeChange.end())
			textureType = (ETextureType)it->second;
	}

	__super::SetTexture(textureType, texture);
}



//
// IFBXMaterial
//
std::shared_ptr<IMaterial> CFBXMaterial::GetMaterial()
{
	return shared_from_this();
}



//
// Protected
//
std::shared_ptr<ITexture> CFBXMaterial::LoadTexture(fbxsdk::FbxTexture * Texture)
{
	Log::Print("FBXMaterial: Loading '%s' texture '%s'.", Texture->GetTextureType().Buffer(), Texture->GetName());

	_ASSERT_EXPR(Texture->Is<fbxsdk::FbxFileTexture>(), "FBX texture must be 'FbxFileTexture'.");
	fbxsdk::FbxFileTexture* fileTexture = fbxsdk::FbxCast<fbxsdk::FbxFileTexture>(Texture);

	// For exporter
	//fileTexture->SetFileName(fileTexture->GetRelativeFileName());

	std::string fileName = fileTexture->GetRelativeFileName();

	auto loaderParams = m_FBXNode.GetFBXScene().GetLoaderParams();
	if (auto loaderFBXParams = std::dynamic_pointer_cast<CznFBXLoaderParams>(loaderParams))
	{
		std::string overrridenTexture = loaderFBXParams->OverrideTexture;
		if (overrridenTexture.size() > 0)
			fileName = overrridenTexture;

		std::string texturesRoot = loaderFBXParams->TexturesPathRoot;
		if (texturesRoot.size() > 0)
			fileName = texturesRoot + "/" + fileName;
	}

	return m_BaseManager.GetManager<IznTexturesFactory>()->LoadTexture2D(fileName);
}

#endif
