#include "stdafx.h"

// General
#include "SceneNodeEngineCreator.h"

// Additional
#include "UI/UIText.h"

#ifdef ZN_FBX_SDK_ENABLE
#include "FBX/FBXManager.h"
#pragma comment(lib, "libfbxsdk-md.lib")
#pragma comment(lib, "libxml2-md.lib")
#pragma comment(lib, "zlib-md.lib")
#endif

CSceneNode3DEngineCreator::CSceneNode3DEngineCreator(IBaseManager& BaseManager)
	: CSceneNode3DCreator(BaseManager)
{
	AddKey(cSceneNode3D);
	AddKey(cSceneNode_FBXNode);
}

CSceneNode3DEngineCreator::~CSceneNode3DEngineCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNode3DEngineCreator::CreateObject(ObjectClassType ObjectClassKey)
{
	if (ObjectClassKey == cSceneNode3D)
	{
		return m_Parent->CreateSceneNode<SceneNode3D>();
	}
#ifdef ZN_FBX_SDK_ENABLE
	else if (ObjectClassKey == cSceneNode_FBXNode)
	{
		CFBXManager fbxManager(GetBaseManager());

		std::shared_ptr<CFBXScene> m_FBXScene = fbxManager.CreateScene("SomeSceneName");
		//m_FBXScene->LoadFromFile(m_BaseManager.GetManager<IFilesManager>()->Open("Bistro_v4\\Bistro v4 Update\\Bistro_v4\\Bistro_Exterior.fbx"));
		//m_FBXScene->LoadFromFile(m_BaseManager.GetManager<IFilesManager>()->Open("Sponza\\sponza.fbx"));
		//m_FBXScene->LoadFromFile(m_BaseManager.GetManager<IFilesManager>()->Open("tower\\tower.fbx"));
		m_FBXScene->LoadFromFile(GetBaseManager().GetManager<IFilesManager>()->Open("Nature Kit (2.1)\\Models\\FBX format\\cliff_blockSlopeHalfWalls_rock.fbx"));
		m_FBXScene->LoadNodes(m_Parent);

		return m_FBXScene->GetRootNode();
	}
#endif
}

CSceneNodeUIEngineCreator::CSceneNodeUIEngineCreator(IBaseManager & BaseManager)
	: CSceneNodeUICreator(BaseManager)
{
	AddKey(cSceneNodeUI);
	AddKey(cSceneNodeUI_Text);
}

CSceneNodeUIEngineCreator::~CSceneNodeUIEngineCreator()
{
}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNodeUIEngineCreator::CreateObject(ObjectClassType ObjectClassKey)
{
	if (ObjectClassKey == cSceneNodeUI)
	{
		return m_Parent->CreateSceneNode<SceneNodeUI>();
	}
	else if (ObjectClassKey == cSceneNodeUI_Text)
	{
		return m_Parent->CreateSceneNode<CUITextNode>();
	}

	return nullptr;
}
