#include "stdafx.h"

// General
#include "SceneNodeDefaultCreator.h"

// Additional
#include "UI/UIText.h"

#ifdef ZN_FBX_SDK_ENABLE
#include "FBX/FBXManager.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#endif

CSceneNodeDefaultCreator::CSceneNodeDefaultCreator(const IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{}

CSceneNodeDefaultCreator::~CSceneNodeDefaultCreator()
{}



//
// ISceneNodeCreator
//
size_t CSceneNodeDefaultCreator::GetSceneNodesCount() const
{
	size_t cnt = 3;
#ifdef ZN_FBX_SDK_ENABLE
	cnt += 1;
#endif
	return cnt;
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

#ifdef ZN_FBX_SDK_ENABLE
	else if (Index == 3)
	{
		return "FBXSceneNode";
	}
#endif

	throw CException("CSceneNodeDefaultCreator: GetSceneNodeTypeName(%d) is out of bounds. Count = %d", Index, GetSceneNodesCount());
}

std::shared_ptr<ISceneNode3D> CSceneNodeDefaultCreator::CreateSceneNode3D(ISceneNode3D* Parent, size_t Index) const
{
	if (Index == 0)
	{
		return Parent->CreateSceneNode<SceneNode3D>();
	}

#ifdef ZN_FBX_SDK_ENABLE
	else if (Index == 3)
	{
		CFBXManager fbxManager(m_BaseManager);

		std::shared_ptr<CFBXScene> m_FBXScene = fbxManager.CreateScene("SomeSceneName");
		//m_FBXScene->LoadFromFile(m_BaseManager.GetManager<IFilesManager>()->Open("Bistro_v4\\Bistro v4 Update\\Bistro_v4\\Bistro_Exterior.fbx"));
		m_FBXScene->LoadFromFile(m_BaseManager.GetManager<IFilesManager>()->Open("Sponza\\sponza.fbx"));
		//m_FBXScene->LoadFromFile(m_BaseManager.GetManager<IFilesManager>()->Open("tower\\tower.fbx"));
		m_FBXScene->LoadNodes(Parent);

		return m_FBXScene->GetRootNode();
	}
#endif

	throw CException("CSceneNodeDefaultCreator: CreateSceneNode(%d) is out of bounds. Count = %d", Index, GetSceneNodesCount());
}

std::shared_ptr<ISceneNodeUI> CSceneNodeDefaultCreator::CreateSceneNodeUI(ISceneNodeUI * Parent, size_t Index) const
{
	if (Index == 1)
	{
		return Parent->CreateSceneNode<SceneNodeUI>();
	}
	else if (Index == 2)
	{
		return Parent->CreateSceneNode<CUITextNode>();
	}

	throw CException("CSceneNodeDefaultCreator: CreateSceneNode(%d) is out of bounds. Count = %d", Index, GetSceneNodesCount());
}
