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
	: CObjectClassCreator(BaseManager)
{
	m_FBXManager = std::make_shared<CFBXManager>(GetBaseManager());

	AddKey(cSceneNode3D);
	AddKey(cSceneNode_FBXNode);
}

CSceneNode3DEngineCreator::~CSceneNode3DEngineCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNode3DEngineCreator::CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs)
{
	ISceneNode3DCreationArgs* sceneNodeCreationArgs = static_cast<ISceneNode3DCreationArgs*>(const_cast<IObjectCreationArgs*>(ObjectCreationArgs));

	if (Index == 0)
	{
		return sceneNodeCreationArgs->GetScene()->CreateSceneNode<SceneNode3D>(nullptr);
	}
#ifdef ZN_FBX_SDK_ENABLE
	else if (Index == 1)
	{
		return std::dynamic_pointer_cast<IObject>(m_FBXManager->CreateSceneNode(sceneNodeCreationArgs->GetScene(), "SomeSceneName"));
	}
#endif

	return nullptr;
}

CSceneNodeUIEngineCreator::CSceneNodeUIEngineCreator(IBaseManager & BaseManager)
	: CObjectClassCreator(BaseManager)
{
	AddKey(cSceneNodeUI);
	AddKey(cSceneNodeUI_Text);
}

CSceneNodeUIEngineCreator::~CSceneNodeUIEngineCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNodeUIEngineCreator::CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs)
{
	ISceneNodeUICreationArgs* sceneNodeCreationArgs = static_cast<ISceneNodeUICreationArgs*>(const_cast<IObjectCreationArgs*>(ObjectCreationArgs));

	if (Index == 0)
	{
		return sceneNodeCreationArgs->GetScene()->CreateSceneNodeUI<SceneNodeUI>(sceneNodeCreationArgs->GetParent());
	}
	else if (Index == 1)
	{
		return sceneNodeCreationArgs->GetScene()->CreateSceneNodeUI<CUITextNode>(sceneNodeCreationArgs->GetParent());
	}

	return nullptr;
}
