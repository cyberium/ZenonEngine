#include "stdafx.h"

#ifdef ZN_FBX_SDK_ENABLE

// General
#include "FBXManager.h"

// Additional
#include "FBXStream.h"

using namespace fbxsdk;

CFBXManager::CFBXManager(const IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
	m_FBXManager = FbxManager::Create();
	if (!m_FBXManager)
		throw CException("CFBXManager: Unable to create FBX Manager.");

	Log::Print("CFBXManager: Autodesk FBX SDK version %s", m_FBXManager->GetVersion());

	// Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(m_FBXManager, IOSROOT);
	m_FBXManager->SetIOSettings(ios);

	// Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	m_FBXManager->LoadPluginsDirectory(lPath.Buffer());
}

CFBXManager::~CFBXManager()
{
	m_FBXManager->Destroy();
}

std::shared_ptr<CFBXSceneNode> CFBXManager::CreateSceneNode(IScene* Scene, std::string SceneName)
{
	auto rootNode = Scene->CreateSceneNode<CFBXSceneNode>(nullptr, m_BaseManager, m_FBXManager);
	return rootNode;
}

#endif
