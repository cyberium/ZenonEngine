#include "stdafx.h"

// General
#include "SceneNodeDefaultCreator.h"

// Additional
#include "UI/UIText.h"
#include "FBX/FBXManager.h"

CSceneNodeDefaultCreator::CSceneNodeDefaultCreator(const IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
{}

CSceneNodeDefaultCreator::~CSceneNodeDefaultCreator()
{}



//
// ISceneNodeCreator
//
size_t CSceneNodeDefaultCreator::GetSceneNodesCount() const
{
	return 4;
}

std::string CSceneNodeDefaultCreator::GetSceneNodeTypeName(size_t Index) const
{
	if (Index == 0)
	{
		return "SceneNode3D";
	}
	else if (Index == 1)
	{
		return "SceneNodeUI";
	}
	else if (Index == 2)
	{
		return "TextUI";
	}
	else if (Index == 3)
	{
		return "FBXSceneNode";
	}

	throw CException("CSceneNodeDefaultCreator: GetSceneNodeTypeName(%d) is out of bounds. Count = %d", Index, GetSceneNodesCount());
}

std::shared_ptr<ISceneNode3D> CSceneNodeDefaultCreator::CreateSceneNode(std::weak_ptr<ISceneNode3D> Parent, size_t Index) const
{
	if (Index == 0)
	{
		return Parent.lock()->CreateSceneNode<SceneNode3D>();
	}
	else if (Index == 1)
	{
		return Parent.lock()->CreateSceneNode<CUIBaseNode>();
	}
	else if (Index == 2)
	{
		return Parent.lock()->CreateSceneNode<CUITextNode>();
	}
	else if (Index == 3)
	{
		CFBXManager fbxManager(m_BaseManager);

		std::shared_ptr<CFBXScene> m_FBXScene = fbxManager.CreateScene("SomeSceneName");
		//m_FBXScene->LoadFromFile(m_BaseManager->GetManager<IFilesManager>()->Open("Bistro_v4\\Bistro v4 Update\\Bistro_v4\\Bistro_Exterior.fbx"));
		m_FBXScene->LoadFromFile(m_BaseManager->GetManager<IFilesManager>()->Open("Sponza\\sponza.fbx"));
		//m_FBXScene->LoadFromFile(m_BaseManager->GetManager<IFilesManager>()->Open("tower\\tower.fbx"));
		m_FBXScene->LoadNodes(Parent.lock());

		return m_FBXScene->GetRootNode();
	}

	throw CException("CSceneNodeDefaultCreator: CreateSceneNode(%d) is out of bounds. Count = %d", Index, GetSceneNodesCount());
}
