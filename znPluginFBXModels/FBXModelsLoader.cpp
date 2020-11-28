#include "stdafx.h"

// General
#include "FBXModelsLoader.h"

// Additional
#include "FBX/FBXStream.h"
#include "FBX/FBXScene.h"
#include "FBX/FBXNode.h"

#ifdef ZN_FBX_SDK_ENABLE

#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")

namespace
{
	const char * cLoaderSupportedExtension = "fbx";
}

CznFBXModelsLoader::CznFBXModelsLoader(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
	m_FBXManager = fbxsdk::FbxManager::Create();
	if (m_FBXManager == nullptr)
		throw CException("FBXManager: Unable to create FBX Manager.");

	Log::Print("FBXManager: FBX SDK version %s.", m_FBXManager->GetVersion());

	int sdkVersionMajor, sdkVersionMinor, sdkVersionRevision;
	fbxsdk::FbxManager::GetFileFormatVersion(sdkVersionMajor, sdkVersionMinor, sdkVersionRevision);
	Log::Print("FBXManager: FBX file format version is %d.%d.%d.", sdkVersionMajor, sdkVersionMinor, sdkVersionRevision);

	fbxsdk::FbxIOSettings* ios = fbxsdk::FbxIOSettings::Create(m_FBXManager, IOSROOT);
	m_FBXManager->SetIOSettings(ios);

	//FbxString lPath = FbxGetApplicationDirectory();
	//m_FBXManager->LoadPluginsDirectory(lPath.Buffer());
}

CznFBXModelsLoader::~CznFBXModelsLoader()
{
	m_FBXManager->Destroy();
}



//
// IznModelsLoader
//
std::string CznFBXModelsLoader::GetName() const
{
	return "FBXModelsLoader";
}

bool CznFBXModelsLoader::IsSupportedFormat(const std::string& ModelFileName) const
{
	std::string lowerFileName = Utils::ToLower(ModelFileName);
	std::string extension = lowerFileName;
	if (ModelFileName.length() > strlen(cLoaderSupportedExtension))
		extension = extension.substr(extension.length() - strlen(cLoaderSupportedExtension));
	return extension == cLoaderSupportedExtension;
}

bool CznFBXModelsLoader::IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const
{
	return ModelFile->Extension() == cLoaderSupportedExtension;
}

std::shared_ptr<IModel> CznFBXModelsLoader::LoadModel(const std::string& ModelFileName, const IznLoaderParams* LoaderParams) const
{
	auto modelFile = m_BaseManager.GetManager<IFilesManager>()->Open(ModelFileName);
	if (modelFile == nullptr)
		throw CException("Model file '%s' not found.", ModelFileName.c_str());
	return LoadModel(modelFile, LoaderParams);
}

std::shared_ptr<IModel> CznFBXModelsLoader::LoadModel(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams) const
{
	return LoadScene(ModelFile, LoaderParams)->MergeModels();
}

std::shared_ptr<IFile> CznFBXModelsLoader::SaveModel(const std::shared_ptr<IModel>& Model, const std::string & FileName) const
{
	throw CException("Operation not supported.");
}



//
// IFBXSceneLoader
//
std::shared_ptr<IFBXScene> CznFBXModelsLoader::LoadScene(const std::string& ModelFileName, const IznLoaderParams* LoaderParams) const
{
	auto fbxSceneFile = m_BaseManager.GetManager<IFilesManager>()->Open(ModelFileName);
	if (fbxSceneFile == nullptr)
		throw CException("FBXScene file '%s' not found.", ModelFileName.c_str());
	return LoadScene(fbxSceneFile, LoaderParams);
}

std::shared_ptr<IFBXScene> CznFBXModelsLoader::LoadScene(const std::shared_ptr<IFile>& ModelFile, const IznLoaderParams* LoaderParams) const
{
	std::lock_guard<std::mutex> fbxLock(m_FBXManagerLock);
	std::shared_ptr<CFBXScene> fbxScene = MakeShared(CFBXScene, m_BaseManager, m_FBXManager, LoaderParams);
	ModelFile->seek(0);
	fbxScene->LoadFromFile(ModelFile);
	return fbxScene;
}

#endif
