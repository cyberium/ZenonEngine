#include "stdafx.h"

// General
#include "FBXModelsLoader.h"

// Additional
#include "FBX/FBXStream.h"
#include "FBX/FBXScene.h"
#include "FBX/FBXSceneNode.h"

#ifdef ZN_FBX_SDK_ENABLE

#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")


CznFBXModelsLoader::CznFBXModelsLoader(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
	m_FBXManager = FbxManager::Create();
	if (m_FBXManager == nullptr)
		throw CException("CFBXManager: Unable to create FBX Manager.");

	Log::Print("CFBXManager: Autodesk FBX SDK version %s", m_FBXManager->GetVersion());

	FbxIOSettings* ios = FbxIOSettings::Create(m_FBXManager, IOSROOT);
	m_FBXManager->SetIOSettings(ios);

	FbxString lPath = FbxGetApplicationDirectory();
	m_FBXManager->LoadPluginsDirectory(lPath.Buffer());
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

bool CznFBXModelsLoader::IsSupportedFormat(const std::shared_ptr<IFile>& ModelFile) const
{
	return ModelFile->Extension() == "fbx";
}

std::shared_ptr<IModel> CznFBXModelsLoader::LoadModel(const std::shared_ptr<IFile>& ModelFile, const std::shared_ptr<IznLoaderParams>& LoaderParams) const
{
	auto FBXScene = MakeShared(CFBXScene, m_BaseManager, m_FBXManager, LoaderParams);
	if (!FBXScene->LoadFromFile(ModelFile))
		throw CException("FBXModelsLoade: Unable to load '%s'", ModelFile->Name().c_str());

	//const auto& models = FBXScene->GetFBXModels();
	//if (models.empty())
	//	throw CException("FBXModelsLoader: Model '%s' doesn't contains models.", ModelFile->Name().c_str());

	//const auto& firstModel = models[0];
	//return firstModel->GetModel();
	return FBXScene->MergeModels();
}

std::shared_ptr<IFile> CznFBXModelsLoader::SaveModel(const std::shared_ptr<IModel>& Model, const std::string & FileName) const
{
	throw CException("Operation not supported.");
}

#endif
