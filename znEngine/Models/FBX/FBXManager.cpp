#include "stdafx.h"

// General
#include "FBXManager.h"

// Additional
#include "FBXStream.h"

using namespace fbxsdk;

CFBXManager::CFBXManager(const IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
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

std::shared_ptr<CFBXScene> CFBXManager::CreateScene(std::string SceneName)
{
	FbxScene* fbxScene = FbxScene::Create(m_SdkManager, SceneName.c_str());
	if (!fbxScene)
		throw CException("CFBXManager: Unable to create FBX scene.");

	return std::make_shared<CFBXScene>(m_BaseManager, fbxScene);
}
