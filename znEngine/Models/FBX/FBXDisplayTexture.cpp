#include "stdafx.h"

// General
#include "FBXDisplayTexture.h"

// Additional
#include <fbxsdk.h>

std::shared_ptr<ITexture> DisplayTextureInfo(std::shared_ptr<IRenderDevice> RenderDevice, FbxTexture* pTexture)
{
	FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(pTexture);
	_ASSERT_EXPR(lFileTexture, "FBX texture must be file texture.");
		
	DisplayString("            Name: \"", (char *)pTexture->GetName(), "\"");
	DisplayString("            File Name: \"", (char *)lFileTexture->GetFileName(), "\"");

	DisplayDouble("            Scale U: ", pTexture->GetScaleU());
	DisplayDouble("            Scale V: ", pTexture->GetScaleV());
	DisplayDouble("            Translation U: ", pTexture->GetTranslationU());
	DisplayDouble("            Translation V: ", pTexture->GetTranslationV());
	DisplayBool("            Swap UV: ", pTexture->GetSwapUV());
	DisplayDouble("            Rotation U: ", pTexture->GetRotationU());
	DisplayDouble("            Rotation V: ", pTexture->GetRotationV());
	DisplayDouble("            Rotation W: ", pTexture->GetRotationW());

	const char* lAlphaSources[] = { "None", "RGB Intensity", "Black" };

	DisplayString("            Alpha Source: ", lAlphaSources[pTexture->GetAlphaSource()]);
	DisplayDouble("            Cropping Left: ", pTexture->GetCroppingLeft());
	DisplayDouble("            Cropping Top: ", pTexture->GetCroppingTop());
	DisplayDouble("            Cropping Right: ", pTexture->GetCroppingRight());
	DisplayDouble("            Cropping Bottom: ", pTexture->GetCroppingBottom());

	const char* lMappingTypes[] = { "Null", "Planar", "Spherical", "Cylindrical",
		"Box", "Face", "UV", "Environment" };

	DisplayString("            Mapping Type: ", lMappingTypes[pTexture->GetMappingType()]);

	if (pTexture->GetMappingType() == FbxTexture::ePlanar)
	{
		const char* lPlanarMappingNormals[] = { "X", "Y", "Z" };

		DisplayString("            Planar Mapping Normal: ", lPlanarMappingNormals[pTexture->GetPlanarMappingNormal()]);
	}

	DisplayDouble("            Alpha: ", pTexture->GetDefaultAlpha());

	if (lFileTexture)
	{
		const char* lMaterialUses[] = { "Model Material", "Default Material" };
		DisplayString("            Material Use: ", lMaterialUses[lFileTexture->GetMaterialUse()]);
	}

	const char* pTextureUses[] = { "Standard", "Shadow Map", "Light Map",
		"Spherical Reflexion Map", "Sphere Reflexion Map", "Bump Normal Map" };

	DisplayString("            Texture Use: ", pTextureUses[pTexture->GetTextureUse()]);
	DisplayString("");
	
	return RenderDevice->CreateTexture2D(lFileTexture->GetFileName());
}

std::shared_ptr<ITexture> FindAndDisplayTextureInfoByProperty(std::shared_ptr<IRenderDevice> RenderDevice, FbxProperty pProperty)
{
	int lTextureCount = pProperty.GetSrcObjectCount<FbxTexture>();
	_ASSERT_EXPR(lTextureCount == 1, "FBX: Textures count must be 1.");

	/*FbxLayeredTexture *lLayeredTexture = pProperty.GetSrcObject<FbxLayeredTexture>(j);
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
	else*/
	{
		//no layered texture simply get on the property
		FbxTexture* lTexture = pProperty.GetSrcObject<FbxTexture>(0);
		if (lTexture)
		{
			DisplayString("    Textures for ", pProperty.GetName());
			DisplayInt("        Texture ", 0);
			return DisplayTextureInfo(RenderDevice, lTexture);
		}
	}
}
