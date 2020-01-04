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

	m_pProperties->Emissive = ToGLMVec3(surfacePhong->Emissive);
	m_pProperties->EmissiveFactor = ToFloat(surfacePhong->EmissiveFactor);

	m_pProperties->Ambient = ToGLMVec3(surfacePhong->Ambient);
	m_pProperties->AmbientFactor = ToFloat(surfacePhong->AmbientFactor);

	m_pProperties->Diffuse = ToGLMVec3(surfacePhong->Diffuse);
	m_pProperties->DiffuseFactor = ToFloat(surfacePhong->DiffuseFactor);

	m_pProperties->NormalMap = ToGLMVec3(surfacePhong->NormalMap);
	m_pProperties->Shininess = ToFloat(surfacePhong->Shininess);

	m_pProperties->Bump = ToGLMVec3(surfacePhong->Bump);
	m_pProperties->BumpFactor = ToFloat(surfacePhong->BumpFactor);

	m_pProperties->TransparentColor = ToGLMVec3(surfacePhong->TransparentColor);
	m_pProperties->TransparencyFactor = ToFloat(surfacePhong->TransparencyFactor);

	m_pProperties->DisplacementColor = ToGLMVec3(surfacePhong->DisplacementColor);
	m_pProperties->DisplacementFactor = ToFloat(surfacePhong->DisplacementFactor);

	m_pProperties->VectorDisplacementColor = ToGLMVec3(surfacePhong->VectorDisplacementColor);
	m_pProperties->VectorDisplacementFactor = ToFloat(surfacePhong->VectorDisplacementFactor);

	m_pProperties->Specular = ToGLMVec3(surfacePhong->Specular);
	m_pProperties->SpecularFactor = ToFloat(surfacePhong->SpecularFactor);

	m_pProperties->Reflection = ToGLMVec3(surfacePhong->Reflection);
	m_pProperties->ReflectionFactor = ToFloat(surfacePhong->ReflectionFactor);

	MarkConstantBufferDirty();

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

			std::shared_ptr<ITexture> texture = LoadTexture(lTexture);

			fbxsdk::FbxLayerElement::EType texureType = FBXSDK_TEXTURE_TYPE(j);
			switch (texureType)
			{
			case fbxsdk::FbxLayerElement::EType::eTextureDiffuse:
				SetTexture(0, texture);
				m_pProperties->HasTextureDiffuse = true;
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureEmissive:
				SetTexture(1, texture);
				m_pProperties->HasTextureEmissive = true;
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureAmbient:
				SetTexture(2, texture);
				m_pProperties->HasTextureAmbient = true;
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureSpecular:
				SetTexture(3, texture);
				m_pProperties->HasTextureSpecular = true;
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureShininess:
				SetTexture(4, texture);
				m_pProperties->HasTextureShininess = true;
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureNormalMap:
				SetTexture(5, texture);
				m_pProperties->HasTextureNormalMap = true;
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureBump:
				SetTexture(6, texture);
				m_pProperties->HasTextureBump = true;
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureTransparency:
				SetTexture(7, texture);
				m_pProperties->HasTextureTransparency = true;
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureReflection:
				SetTexture(8, texture);
				m_pProperties->HasTextureReflection = true;
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureDisplacement:
				SetTexture(9, texture);
				m_pProperties->HasTextureDisplacement = true;
				break;

			default:
				Log::Error("CFBXMaterial: Unsupported texture type '%d'.", texureType);
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



//
// Protected
//
void CFBXMaterial::UpdateConstantBuffer() const
{
	MaterialProxie::UpdateConstantBuffer(m_pProperties, sizeof(MaterialProperties));
}
