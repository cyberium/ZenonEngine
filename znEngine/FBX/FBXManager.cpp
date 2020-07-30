#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXManager.h"

// Additional
#include "FBXStream.h"
#include "FBXScene.h"
#include "FBXSceneNode.h"

CFBXManager::CFBXManager(const IBaseManager& BaseManager)
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

CFBXManager::~CFBXManager()
{
	m_FBXManager->Destroy();
}

//
// IFBXManager
//
std::shared_ptr<IFBXScene> CFBXManager::LoadFBX(const std::string & FileName)
{
	auto file = m_BaseManager.GetManager<IFilesManager>()->Open(FileName);
	if (file == nullptr)
		throw CException("FBXManager: File '%s' not found.", FileName.c_str());

	auto FBXScene = std::make_shared<CFBXScene>(m_BaseManager, m_FBXManager);
	if (!FBXScene->LoadFromFile(file))
		throw CException("FBXManager: Unable to load '%s'.", FileName.c_str());

	return FBXScene;
}

#endif
