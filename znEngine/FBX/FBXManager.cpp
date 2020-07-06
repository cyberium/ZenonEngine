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
fbxsdk::FbxManager* CFBXManager::GetFBXManager() const
{
	return m_FBXManager;
}

#endif
