#include "stdafx.h"

// General
#include "FBXScene.h"

// Additional
#include "FBXStream.h"
#include "FBXDisplayCommon.h"

CFBXScene::CFBXScene(const IBaseManager* BaseManager, fbxsdk::FbxScene* NativeScene)
	: m_BaseManager(BaseManager)
	, m_NativeScene(NativeScene)
{
}

CFBXScene::~CFBXScene()
{
}

//------------------------------------------------------------------------------------------------------

bool CFBXScene::LoadFromFile(std::shared_ptr<IFile> File)
{
	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	fbxsdk::FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(GetNativeManager(), "");

	// Create wrapper for file
	CFBXStream fbxStream(File, GetNativeManager()->GetIOPluginRegistry());
	m_Path = File->Path();

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(&fbxStream, NULL, lFileFormat, GetNativeManager()->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		Log::Print("Call to FbxImporter::Initialize() failed.");
		Log::Print("Error returned: %s", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			Log::Print("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			Log::Print("FBX file format version for file '%s' is %d.%d.%d\n\n", File->Path_Name().c_str(), lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	Log::Print("FBX file format version for this FBX SDK is %d.%d.%d", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		Log::Print("FBX file format version for file '%s' is %d.%d.%d", File->Path_Name().c_str(), lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		Log::Print("Animation Stack Information");

		lAnimStackCount = lImporter->GetAnimStackCount();

		Log::Print("    Number of Animation Stacks: %d", lAnimStackCount);
		Log::Print("    Current Animation Stack: '%s'", lImporter->GetActiveAnimStackName().Buffer());
		Log::Print("\n");

		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			Log::Print("    Animation Stack %d", i);
			Log::Print("         Name: '%s'", lTakeInfo->mName.Buffer());
			Log::Print("         Description: '%s'", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			Log::Print("         Import Name: '%s'", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			Log::Print("         Import State: %s", lTakeInfo->mSelect ? "true" : "false");
			Log::Print("\n");
		}

		// Set the import states. By default, the import states are always set to true. The code below shows how to change these states.
		GetNativeManager()->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
		GetNativeManager()->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
		GetNativeManager()->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, true);
		GetNativeManager()->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
		GetNativeManager()->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, true);
		GetNativeManager()->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
		GetNativeManager()->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(m_NativeScene);
	if (!lStatus)
	{
		Log::Print("Call to FbxImporter::Import() failed.");
		Log::Print("Error returned: %s", lImporter->GetStatus().GetErrorString());
		return lStatus;
	}


#if 0
	if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
	{
		Log::Print("Please enter password: ");

		lPassword[0] = '\0';

		FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
			scanf("%s", lPassword);
		FBXSDK_CRT_SECURE_NO_WARNING_END

			FbxString lString(lPassword);

		m_SdkManager->GetIOSettings()->SetStringProp(IMP_FBX_PASSWORD, lString);
		m_SdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

		lStatus = lImporter->Import(pScene);

		if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			Log::Print("\nPassword is wrong, import aborted.\n");
		}
	}
#endif

	lImporter->Destroy();

	FbxGeometryConverter converter(GetNativeManager());

	if (!converter.Triangulate(fbxsdk::FbxCast<fbxsdk::FbxScene>(m_NativeScene), true))
	{
		Log::Error("CFBXManager: Error while FbxGeometryConverter::Triangulate.");
		return false;
	}

	if (!converter.SplitMeshesPerMaterial(fbxsdk::FbxCast<fbxsdk::FbxScene>(m_NativeScene), true))
	{
		Log::Error("CFBXManager: Error while FbxGeometryConverter::SplitMeshesPerMaterial.");
		return false;
	}

	return lStatus;
}

bool CFBXScene::SaveToFile(std::shared_ptr<IFile> File, int pFileFormat, bool pEmbedMedia)
{
	int lMajor, lMinor, lRevision;
	bool lStatus = true;

	// Create an exporter.
	fbxsdk::FbxExporter* lExporter = fbxsdk::FbxExporter::Create(GetNativeManager(), "");

	if (pFileFormat < 0 || pFileFormat >= GetNativeManager()->GetIOPluginRegistry()->GetWriterFormatCount())
	{
		// Write in fall back format in less no ASCII format found
		pFileFormat = GetNativeManager()->GetIOPluginRegistry()->GetNativeWriterFormat();

		//Try to export in ASCII if possible
		for (int lFormatIndex = 0; lFormatIndex < GetNativeManager()->GetIOPluginRegistry()->GetWriterFormatCount(); lFormatIndex++)
		{
			if (GetNativeManager()->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
			{
				FbxString lDesc = GetNativeManager()->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
				const char *lASCII = "ascii";
				if (lDesc.Find(lASCII) >= 0)
				{
					pFileFormat = lFormatIndex;
					break;
				}
			}
		}
	}

	// Set the export states. By default, the export states are always set to  true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below shows how to change these states.
	GetNativeManager()->GetIOSettings()->SetBoolProp(EXP_FBX_MATERIAL, true);
	GetNativeManager()->GetIOSettings()->SetBoolProp(EXP_FBX_TEXTURE, true);
	GetNativeManager()->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, pEmbedMedia);
	GetNativeManager()->GetIOSettings()->SetBoolProp(EXP_FBX_SHAPE, true);
	GetNativeManager()->GetIOSettings()->SetBoolProp(EXP_FBX_GOBO, true);
	GetNativeManager()->GetIOSettings()->SetBoolProp(EXP_FBX_ANIMATION, true);
	GetNativeManager()->GetIOSettings()->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	// Create wrapper for file
	CFBXStream fbxStream(File, GetNativeManager()->GetIOPluginRegistry());

	// Initialize the exporter by providing a filename.
	if (lExporter->Initialize(&fbxStream, NULL, pFileFormat, GetNativeManager()->GetIOSettings()) == false)
	{
		Log::Print("Call to FbxExporter::Initialize() failed.\n");
		Log::Print("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
		return false;
	}

	fbxsdk::FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
	Log::Print("FBX file format version %d.%d.%d\n\n", lMajor, lMinor, lRevision);

	// Export the scene.
	lStatus = lExporter->Export(m_NativeScene);

	// Destroy the exporter.
	lExporter->Destroy();

	return lStatus;
}

bool CFBXScene::Load()
{
	DisplayMetaData(GetNativeScene());
	DisplayHierarchy(GetNativeScene());

	LoadNodes();

	return true;
}

//------------------------------------------------------------------------------------------------------

bool CFBXScene::LoadNodes()
{
	m_RootNode = std::make_shared<CFBXSceneNode>(shared_from_this(), m_NativeScene->GetRootNode());

	return true;
}

//------------------------------------------------------------------------------------------------------

bool CFBXScene::LoadTextures()
{
	Log::Print("CFBXScene: Textures count '%d'.", m_NativeScene->GetTextureCount());
	for (int i = 0; i < m_NativeScene->GetTextureCount(); i++)
	{
		fbxsdk::FbxTexture* texture = m_NativeScene->GetTexture(i);
		Log::Print("CFBXScene: Loading texture '%s'.", texture->GetName());

		_ASSERT_EXPR(texture->Is<fbxsdk::FbxFileTexture>(), "FBX texture must be 'FbxFileTexture'.");
		fbxsdk::FbxFileTexture* fileTexture = fbxsdk::FbxCast<fbxsdk::FbxFileTexture>(texture);

		// For exporter
		fileTexture->SetFileName(fileTexture->GetRelativeFileName());

		m_TexturesArray.push_back(m_BaseManager->GetManager<IRenderDevice>()->CreateTexture2D(m_Path + fileTexture->GetRelativeFileName()));
	}

	return true;
}

const std::vector<std::shared_ptr<ITexture>>& CFBXScene::GetTextures() const
{
	return m_TexturesArray;
}

std::shared_ptr<ITexture> CFBXScene::GetTexture(int Index) const
{
	return m_TexturesArray.at(Index);
}

//------------------------------------------------------------------------------------------------------

bool CFBXScene::LoadMaterials()
{
	Log::Print("CFBXScene: Materials count '%d'.", m_NativeScene->GetMaterialCount());
	for (int i = 0; i < m_NativeScene->GetMaterialCount(); i++)
	{
		fbxsdk::FbxSurfaceMaterial* surfaceMaterial = m_NativeScene->GetMaterial(i);
		Log::Print("CFBXScene: Loading material '%s'.", surfaceMaterial->GetName());

		_ASSERT_EXPR(surfaceMaterial->Is<fbxsdk::FbxSurfacePhong>(), "FBX material must be 'FbxSurfacePhong'.");
		fbxsdk::FbxSurfacePhong* surfacePhong = fbxsdk::FbxCast<fbxsdk::FbxSurfacePhong>(surfaceMaterial);

		std::shared_ptr<CFBXMaterial> material = std::make_shared<CFBXMaterial>(m_BaseManager->GetManager<IRenderDevice>());
		material->SetAmbient(vec3(surfacePhong->Ambient.Get()[0], surfacePhong->Ambient.Get()[1], surfacePhong->Ambient.Get()[2]));
		material->SetDiffuse(vec3(surfacePhong->Diffuse.Get()[0], surfacePhong->Diffuse.Get()[1], surfacePhong->Diffuse.Get()[2]));
		material->SetSpecular(vec3(surfacePhong->Specular.Get()[0], surfacePhong->Specular.Get()[1], surfacePhong->Specular.Get()[2]));
		material->SetEmissive(vec3(surfacePhong->Emissive.Get()[0], surfacePhong->Emissive.Get()[1], surfacePhong->Emissive.Get()[2]));
		material->SetTransparencyFactor(surfacePhong->TransparencyFactor.Get());
		material->SetShininess(surfacePhong->Shininess.Get());
		material->SetReflectionFactor(surfacePhong->ReflectionFactor.Get());

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

				std::shared_ptr<ITexture> texture = m_Textures[std::string(lTexture->GetName())];
				fbxsdk::FbxLayerElement::EType texureType = FBXSDK_TEXTURE_TYPE(j);

				switch (texureType)
				{
				case fbxsdk::FbxLayerElement::EType::eTextureDiffuse:
					material->SetTexture(0, texture);
					break;

				case fbxsdk::FbxLayerElement::EType::eTextureEmissive:
					material->SetTexture(1, texture);
					break;


				case fbxsdk::FbxLayerElement::EType::eTextureAmbient:
					material->SetTexture(2, texture);
					break;


				case fbxsdk::FbxLayerElement::EType::eTextureSpecular:
					material->SetTexture(3, texture);
					break;


				case fbxsdk::FbxLayerElement::EType::eTextureShininess:
					break;


				case fbxsdk::FbxLayerElement::EType::eTextureNormalMap:
					break;


				case fbxsdk::FbxLayerElement::EType::eTextureBump:
					break;


				case fbxsdk::FbxLayerElement::EType::eTextureTransparency:
					break;

				case fbxsdk::FbxLayerElement::EType::eTextureReflection:
					break;


				case fbxsdk::FbxLayerElement::EType::eTextureReflectionFactor:
					break;


				case fbxsdk::FbxLayerElement::EType::eTextureDisplacement:
					break;


				case fbxsdk::FbxLayerElement::EType::eTextureDisplacementVector:
					break;
				}
			}
			else
			{
				_ASSERT_EXPR(false, L"CFBXScene: Material '%s' texture '%s' (%s) has more than one texture object.", surfaceMaterial->GetName(), lProperty.GetName().Buffer(), fbxsdk::FbxLayerElement::sTextureChannelNames[j]);
			}
		}

		m_Materials.insert(std::make_pair(surfacePhong->GetName(), material));
		m_MaterialsArray.push_back(material);
	}

	return true;
}

const std::vector<std::shared_ptr<CFBXMaterial>>& CFBXScene::GetMaterials() const
{
	return m_MaterialsArray;
}

std::shared_ptr<CFBXMaterial> CFBXScene::GetMaterial(int Index) const
{
	return m_MaterialsArray.at(Index);
}

//------------------------------------------------------------------------------------------------------

bool CFBXScene::LoadMeshes()
{
	Log::Print("CFBXScene: Geometry count '%d'.", m_NativeScene->GetGeometryCount());
	for (int i = 0; i < m_NativeScene->GetGeometryCount(); i++)
	{
		fbxsdk::FbxGeometry* geometry = m_NativeScene->GetGeometry(i);
		_ASSERT_EXPR(geometry->Is<fbxsdk::FbxMesh>(), L"FBX geometry must be 'FbxMesh'.");
		fbxsdk::FbxMesh* fbxMesh = fbxsdk::FbxCast<fbxsdk::FbxMesh>(geometry);
		Log::Print("CFBXScene: Loading mesh '%s'.", fbxMesh->GetName());

		std::shared_ptr<CFBXMesh> mesh = std::make_shared<CFBXMesh>(m_BaseManager, shared_from_this(), fbxMesh);
		mesh->Load();
		m_MeshesArray.push_back(mesh);
	}
	return true;
}

const std::vector<std::shared_ptr<CFBXMesh>>& CFBXScene::GetMeshes() const
{
	return m_MeshesArray;
}

std::shared_ptr<CFBXMesh> CFBXScene::GetMesh(int Index) const
{
	return m_MeshesArray.at(Index);
}

//------------------------------------------------------------------------------------------------------

fbxsdk::FbxScene * CFBXScene::GetNativeScene() const
{
	return m_NativeScene;
}

fbxsdk::FbxManager * CFBXScene::GetNativeManager() const
{
	return m_NativeScene->GetFbxManager();
}
