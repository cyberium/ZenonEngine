#include "stdafx.h"

// General
#include "FBXDisplayMaterial.h"

// Additional
#include "FBXMaterial.h"
#include "FBXDisplayTexture.h"

// Additonals
#include <fbxsdk.h>

std::shared_ptr<IMaterial> DisplayMaterial(std::shared_ptr<IRenderDevice> RenderDevice, FbxGeometry* pGeometry)
{
	int lMaterialCount = 0;
	FbxNode* lNode = NULL;
	if (pGeometry) 
	{
		lNode = pGeometry->GetNode();
		if (lNode)
			lMaterialCount = lNode->GetMaterialCount();
	}

	if (lMaterialCount == 0)
	{
		_ASSERT_EXPR(false, "FBX: Geometry must contain only one material!");
		return nullptr;
	}

	
	for (int iii = 0; iii < lMaterialCount; iii++)
	{
		std::shared_ptr<CFBXMaterial> material = std::make_shared<CFBXMaterial>(RenderDevice);

		DisplayInt("        Material ", iii);

		FbxSurfaceMaterial* lMaterial = lNode->GetMaterial(iii);

		DisplayString("            Name: \"", (char *)lMaterial->GetName(), "\"");

		_ASSERT_EXPR(lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId), "FBX material must be 'FbxSurfacePhong'");
		if (lMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
		{
			FbxPropertyT<FbxDouble3> lKFbxDouble3;
			FbxPropertyT<FbxDouble> lKFbxDouble1;
			FbxColor theColor;

			// We found a Phong material.  Display its properties.

			// Display the Ambient Color
			lKFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Ambient;
			theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
			material->SetAmbient(vec3(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]));

			// Display the Diffuse Color
			lKFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Diffuse;
			theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
			DisplayColor("            Diffuse: ", theColor);
			material->SetDiffuse(vec3(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]));

			// Display the Specular Color (unique to Phong materials)
			lKFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Specular;
			theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
			DisplayColor("            Specular: ", theColor);
			material->SetSpecular(vec3(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]));

			// Display the Emissive Color
			lKFbxDouble3 = ((FbxSurfacePhong *)lMaterial)->Emissive;
			theColor.Set(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]);
			DisplayColor("            Emissive: ", theColor);
			material->SetEmissive(vec3(lKFbxDouble3.Get()[0], lKFbxDouble3.Get()[1], lKFbxDouble3.Get()[2]));

			//Opacity is Transparency factor now
			lKFbxDouble1 = ((FbxSurfacePhong *)lMaterial)->TransparencyFactor;
			DisplayDouble("            TransparencyFactor: ", lKFbxDouble1.Get());
			DisplayDouble("            Opacity: ", (1.0 - lKFbxDouble1.Get()));
			material->SetTransparencyFactor(lKFbxDouble1.Get());

			// Display the Shininess
			lKFbxDouble1 = ((FbxSurfacePhong *)lMaterial)->Shininess;
			DisplayDouble("            Shininess: ", lKFbxDouble1.Get());
			material->SetShininess(lKFbxDouble1.Get());

			// Display the Reflectivity
			lKFbxDouble1 = ((FbxSurfacePhong *)lMaterial)->ReflectionFactor;
			DisplayDouble("            ReflectionFactor: ", lKFbxDouble1.Get());
			material->SetReflectionFactor(lKFbxDouble1.Get());
		}


		for (int lTextureIndex = 0; lTextureIndex < fbxsdk::FbxLayerElement::sTypeTextureCount; lTextureIndex++)
		{
			fbxsdk::FbxProperty lProperty = lMaterial->FindProperty(fbxsdk::FbxLayerElement::sTextureChannelNames[lTextureIndex]);
			if (!lProperty.IsValid())
			{
				continue;
			}

			FbxTexture* lTexture = lProperty.GetSrcObject<FbxTexture>(0);
			if (!lTexture)
			{
				continue;
			}

			std::shared_ptr<ITexture> texture = DisplayTextureInfo(RenderDevice, lTexture);
				

			fbxsdk::FbxLayerElement::EType texureType = FBXSDK_TEXTURE_TYPE(lTextureIndex);
			switch (texureType)
			{
			case fbxsdk::FbxLayerElement::EType::eTextureDiffuse:
				material->SetTexture(0, texture);
				break;

			case fbxsdk::FbxLayerElement::EType::eTextureEmissive:
				material->SetTexture(1, texture);
				break;


			case fbxsdk::FbxLayerElement::EType::eTextureAmbient :
				material->SetTexture(2, texture);
				break;


			case fbxsdk::FbxLayerElement::EType::eTextureSpecular :
				material->SetTexture(3, texture);
				break;


			case fbxsdk::FbxLayerElement::EType::eTextureShininess :

				break;


			case fbxsdk::FbxLayerElement::EType::eTextureNormalMap :

				break;


			case fbxsdk::FbxLayerElement::EType::eTextureBump :

				break;


			case fbxsdk::FbxLayerElement::EType::eTextureTransparency :

				break;


			case fbxsdk::FbxLayerElement::EType::eTextureReflection :

				break;


			case fbxsdk::FbxLayerElement::EType::eTextureReflectionFactor :

				break;


			case fbxsdk::FbxLayerElement::EType::eTextureDisplacement :

				break;


			case fbxsdk::FbxLayerElement::EType::eTextureDisplacementVector :

				break;
			}

			
		}

		return material;
	}

	return nullptr;
}

