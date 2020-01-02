#include "stdafx.h"

// Include
#include "FBXScene.h"
#include "FBXSceneNode.h"

// General
#include "FBXMaterial.h"

CFBXMaterial::CFBXMaterial(const IBaseManager* BaseManager, std::weak_ptr<CFBXSceneNode> OwnerFBXNode, fbxsdk::FbxSurfaceMaterial* NativeMaterial)
	: MaterialProxie(BaseManager->GetManager<IRenderDevice>()->CreateMaterial(sizeof(MaterialProperties)))
	, m_BaseManager(BaseManager)
	, m_RenderDevice(BaseManager->GetManager<IRenderDevice>())
	, m_OwnerFBXNode(OwnerFBXNode)
	, m_NativeMaterial(NativeMaterial)
{
	// Constant buffer
	m_pProperties = (MaterialProperties*)_aligned_malloc(sizeof(MaterialProperties), 16);
	*m_pProperties = MaterialProperties();

	std::shared_ptr<ISamplerState> g_Sampler = m_RenderDevice->CreateSamplerState();
	g_Sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	g_Sampler->SetWrapMode(ISamplerState::WrapMode::Repeat, ISamplerState::WrapMode::Repeat);
	SetSampler(0, g_Sampler);
}

CFBXMaterial::~CFBXMaterial()
{
	if (m_pProperties)
	{
		_aligned_free(m_pProperties);
		m_pProperties = nullptr;
	}
}

void CFBXMaterial::Load()
{
	//Log::Print("CFBXSceneNode: Loading material '%s'.", m_NativeMaterial->GetName());

	_ASSERT_EXPR(m_NativeMaterial->Is<fbxsdk::FbxSurfacePhong>(), "FBX material must be 'FbxSurfacePhong'.");
	fbxsdk::FbxSurfacePhong* surfacePhong = fbxsdk::FbxCast<fbxsdk::FbxSurfacePhong>(m_NativeMaterial);

	SetAmbient(vec3(surfacePhong->Ambient.Get()[0], surfacePhong->Ambient.Get()[1], surfacePhong->Ambient.Get()[2]));
	SetDiffuse(vec3(surfacePhong->Diffuse.Get()[0], surfacePhong->Diffuse.Get()[1], surfacePhong->Diffuse.Get()[2]));
	SetSpecular(vec3(surfacePhong->Specular.Get()[0], surfacePhong->Specular.Get()[1], surfacePhong->Specular.Get()[2]));
	SetEmissive(vec3(surfacePhong->Emissive.Get()[0], surfacePhong->Emissive.Get()[1], surfacePhong->Emissive.Get()[2]));
	SetTransparencyFactor(surfacePhong->TransparencyFactor.Get());
	SetShininess(surfacePhong->Shininess.Get());
	SetReflectionFactor(surfacePhong->ReflectionFactor.Get());

	for (int j = 0; j < fbxsdk::FbxLayerElement::sTypeTextureCount; j++)
	{
		fbxsdk::FbxProperty lProperty = surfacePhong->FindProperty(fbxsdk::FbxLayerElement::sTextureChannelNames[j]);
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

			if (FBXSDK_TEXTURE_TYPE(j) == fbxsdk::FbxLayerElement::EType::eTextureNormalMap)
				continue;

			std::shared_ptr<ITexture> texture = LoadTexture(lTexture);

			fbxsdk::FbxLayerElement::EType texureType = FBXSDK_TEXTURE_TYPE(j);
			switch (texureType)
			{
			case fbxsdk::FbxLayerElement::EType::eTextureDiffuse:
				SetTexture(0, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureEmissive:
				SetTexture(1, texture);
				break;


			case fbxsdk::FbxLayerElement::EType::eTextureAmbient:
				SetTexture(2, texture);
				break;


			case fbxsdk::FbxLayerElement::EType::eTextureSpecular:
				SetTexture(3, texture);
				break;


			case fbxsdk::FbxLayerElement::EType::eTextureShininess:
			case fbxsdk::FbxLayerElement::EType::eTextureNormalMap:
			case fbxsdk::FbxLayerElement::EType::eTextureBump:
			case fbxsdk::FbxLayerElement::EType::eTextureTransparency:
			case fbxsdk::FbxLayerElement::EType::eTextureReflection:
			case fbxsdk::FbxLayerElement::EType::eTextureReflectionFactor:
			case fbxsdk::FbxLayerElement::EType::eTextureDisplacement:
			case fbxsdk::FbxLayerElement::EType::eTextureDisplacementVector:
			{
			}
			break;
			}
		}
		else
		{
			_ASSERT_EXPR(false, L"CFBXMaterial: Material '%s' texture '%s' (%s) has more than one texture object.", surfaceMaterial->GetName(), lProperty.GetName().Buffer(), fbxsdk::FbxLayerElement::sTextureChannelNames[j]);
		}
	}
}

std::shared_ptr<ITexture> CFBXMaterial::LoadTexture(fbxsdk::FbxTexture * Texture)
{
	//Log::Print("CFBXMaterial: Loading texture '%s'.", Texture->GetName());

	_ASSERT_EXPR(Texture->Is<fbxsdk::FbxFileTexture>(), "FBX texture must be 'FbxFileTexture'.");
	fbxsdk::FbxFileTexture* fileTexture = fbxsdk::FbxCast<fbxsdk::FbxFileTexture>(Texture);

	// For exporter
	fileTexture->SetFileName(fileTexture->GetRelativeFileName());

	return m_BaseManager->GetManager<IRenderDevice>()->CreateTexture2D(m_OwnerFBXNode.lock()->GetOwnerScene().lock()->GetPath() + fileTexture->GetRelativeFileName());
}
