#include "stdafx.h"

// General
#include "FBXManager.h"

using namespace fbxsdk;

CFBXManager::CFBXManager()
{
	m_SdkManager = FbxManager::Create();
	if (!m_SdkManager)
		throw CException("CFBXManager: Unable to create FBX Manager.");

	Log::Print("CFBXManager: Autodesk FBX SDK version %s", m_SdkManager->GetVersion());

	// Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(m_SdkManager, IOSROOT);
	m_SdkManager->SetIOSettings(ios);

	// Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	m_SdkManager->LoadPluginsDirectory(lPath.Buffer());
}

CFBXManager::~CFBXManager()
{
	m_SdkManager->Destroy();
}

FbxScene* CFBXManager::CreateScene(std::string SceneName)
{
	FbxScene* fbxScene = FbxScene::Create(m_SdkManager, SceneName.c_str());
	if (!fbxScene)
		throw CException("CFBXManager: Unable to create FBX scene.");

	return fbxScene;
}

bool CFBXManager::LoadScene(FbxScene*& pScene, const char* pFilename)
{
	if (pScene == nullptr)
	{
		Log::Warn("CFBXManager: Passed scene is null. We will create scene.");
		pScene = CreateScene("DefaultFBXSceneName");
	}

	int lFileMajor, lFileMinor, lFileRevision;
	int lSDKMajor, lSDKMinor, lSDKRevision;
	int lFileFormat = -1;
	int i, lAnimStackCount;
	bool lStatus;
	char lPassword[1024];

	// Get the file version number generate by the FBX SDK.
	FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

	// Create an importer.
	FbxImporter* lImporter = FbxImporter::Create(m_SdkManager, "");

	// Initialize the importer by providing a filename.
	const bool lImportStatus = lImporter->Initialize(pFilename, lFileFormat, m_SdkManager->GetIOSettings());
	lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

	if (!lImportStatus)
	{
		FbxString error = lImporter->GetStatus().GetErrorString();
		Log::Print("Call to FbxImporter::Initialize() failed.");
		Log::Print("Error returned: %s", error.Buffer());

		if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
		{
			Log::Print("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
			Log::Print("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
		}

		return false;
	}

	Log::Print("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

	if (lImporter->IsFBX())
	{
		Log::Print("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

		// From this point, it is possible to access animation stack information without
		// the expense of loading the entire file.

		Log::Print("Animation Stack Information\n");

		lAnimStackCount = lImporter->GetAnimStackCount();

		Log::Print("    Number of Animation Stacks: %d\n", lAnimStackCount);
		Log::Print("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
		Log::Print("\n");

		for (i = 0; i < lAnimStackCount; i++)
		{
			FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

			Log::Print("    Animation Stack %d\n", i);
			Log::Print("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
			Log::Print("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

			// Change the value of the import name if the animation stack should be imported 
			// under a different name.
			Log::Print("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

			// Set the value of the import state to false if the animation stack should be not
			// be imported. 
			Log::Print("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
			Log::Print("\n");
		}

		// Set the import states. By default, the import states are always set to true. The code below shows how to change these states.
		m_SdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_MATERIAL, true);
		m_SdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_TEXTURE, true);
		m_SdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_LINK, true);
		m_SdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_SHAPE, true);
		m_SdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_GOBO, true);
		m_SdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_ANIMATION, true);
		m_SdkManager->GetIOSettings()->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
	}

	// Import the scene.
	lStatus = lImporter->Import(pScene);
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

	FbxGeometryConverter converter(m_SdkManager);
	if (!converter.Triangulate(FbxCast<fbxsdk::FbxScene>(pScene), true))
	{
		Log::Error("CFBXManager: Error while triangulate.");
		return false;
	}

	return lStatus;
}

bool CFBXManager::SaveScene(FbxScene*& pScene, const char* pFilename, int pFileFormat, bool pEmbedMedia)
{
	int lMajor, lMinor, lRevision;
	bool lStatus = true;

	// Create an exporter.
	FbxExporter* lExporter = FbxExporter::Create(m_SdkManager, "");

	if (pFileFormat < 0 || pFileFormat >= m_SdkManager->GetIOPluginRegistry()->GetWriterFormatCount())
	{
		// Write in fall back format in less no ASCII format found
		pFileFormat = m_SdkManager->GetIOPluginRegistry()->GetNativeWriterFormat();

		//Try to export in ASCII if possible
		int lFormatIndex, lFormatCount = m_SdkManager->GetIOPluginRegistry()->GetWriterFormatCount();

		for (lFormatIndex = 0; lFormatIndex < lFormatCount; lFormatIndex++)
		{
			if (m_SdkManager->GetIOPluginRegistry()->WriterIsFBX(lFormatIndex))
			{
				FbxString lDesc = m_SdkManager->GetIOPluginRegistry()->GetWriterFormatDescription(lFormatIndex);
				const char *lASCII = "ascii";
				if (lDesc.Find(lASCII) >= 0)
				{
					pFileFormat = lFormatIndex;
					break;
				}
			}
		}
	}

	// Set the export states. By default, the export states are always set to 
	// true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below 
	// shows how to change these states.
	m_SdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_MATERIAL, true);
	m_SdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_TEXTURE, true);
	m_SdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_EMBEDDED, pEmbedMedia);
	m_SdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_SHAPE, true);
	m_SdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_GOBO, true);
	m_SdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_ANIMATION, true);
	m_SdkManager->GetIOSettings()->SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

	// Initialize the exporter by providing a filename.
	if (lExporter->Initialize(pFilename, pFileFormat, m_SdkManager->GetIOSettings()) == false)
	{
		Log::Print("Call to FbxExporter::Initialize() failed.\n");
		Log::Print("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
		return false;
	}

	FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
	Log::Print("FBX file format version %d.%d.%d\n\n", lMajor, lMinor, lRevision);

	// Export the scene.
	lStatus = lExporter->Export(pScene);

	// Destroy the exporter.
	lExporter->Destroy();

	return lStatus;
}