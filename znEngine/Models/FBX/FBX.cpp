#include "stdafx.h"

// General
#include "FBX.h"

// Additional
#include "FBXManager.h"
#include "FBXScene.h"

#include "FBXDisplayMesh.h"

CFBX::CFBX(const std::string& SceneName, std::shared_ptr<ISceneNode> ParentNode)
{
	m_BaseManager = std::dynamic_pointer_cast<IBaseManagerHolder>(ParentNode->GetScene())->GetBaseManager();
	
	CFBXManager fbxManager(m_BaseManager);

	m_FBXScene = fbxManager.CreateScene("SomeSceneName");
	m_FBXScene->LoadFromFile(m_BaseManager->GetManager<IFilesManager>()->Open(SceneName));
	m_FBXScene->LoadNodes(ParentNode);

	//std::shared_ptr<IFile> savedFile = std::make_shared<CFile>("Sponza\\SponzaConv.fbx");
	//fbxScene->SaveToFile(savedFile);
	//m_BaseManager->GetManager<IFilesManager>()->GetFilesStorage("ZenonGamedata")->SaveFile(savedFile);

}

CFBX::~CFBX()
{
}

const IBaseManager * CFBX::GetBaseManager() const
{
	return m_BaseManager;
}

std::shared_ptr<ISceneNode> CFBX::GetSceneNode()
{
	return m_FBXScene->GetRootNode();
}


