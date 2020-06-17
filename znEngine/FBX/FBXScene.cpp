#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXScene.h"

// Additional
#include "FBXStream.h"
#include "FBXDisplayCommon.h"

CFBXScene::CFBXScene(const IBaseManager& BaseManager, fbxsdk::FbxScene* NativeScene)
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

	Log::Info("CFBXScene: Triangulation started.");
	Timer t;
	if (!converter.Triangulate(fbxsdk::FbxCast<fbxsdk::FbxScene>(m_NativeScene), true))
	{
		Log::Error("CFBXScene: Error while FbxGeometryConverter::Triangulate.");
		return false;
	}
	Log::Info("CFBXScene: Triangulation finished in '%f' ms.", t.GetElapsedTime());

	//if (!converter.SplitMeshesPerMaterial(fbxsdk::FbxCast<fbxsdk::FbxScene>(m_NativeScene), true))
	//{
	//	Log::Error("CFBXManager: Error while FbxGeometryConverter::SplitMeshesPerMaterial.");
	//	return false;
	//}

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

//------------------------------------------------------------------------------------------------------

bool CFBXScene::LoadNodes(ISceneNode3D* ParentNode)
{
	DisplayMetaData(GetNativeScene());
	DisplayHierarchy(GetNativeScene());

	Log::Print("GEOMETRIES CNT = '%d'", m_NativeScene->GetGeometryCount());
	for (int i = 0; i < m_NativeScene->GetGeometryCount(); i++)
	{
		fbxsdk::FbxGeometry* geom = m_NativeScene->GetGeometry(i);
		Log::Print("%d GEOM NAME = '%s'", i, geom->GetNameOnly().Buffer());
	}

	m_RootNode = ParentNode->CreateSceneNode<CFBXSceneNode>(m_BaseManager, weak_from_this(), m_NativeScene->GetRootNode());
	m_RootNode->LoadNode();
	//m_RootNode->SetScale(glm::vec3(10));



	return true;
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

std::string CFBXScene::GetPath() const
{
	return m_Path;
}

std::shared_ptr<CFBXSceneNode> CFBXScene::GetRootNode() const
{
	return m_RootNode;
}

#endif
