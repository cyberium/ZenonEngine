#include "stdafx.h"

// General
#include "SceneNodeEngineCreator.h"

// Additional
#include "UI/UIText.h"
#include "UI/UIColor.h"

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
		return sceneNodeCreationArgs->GetScene()->CreateSceneNode<SceneNode3D>(sceneNodeCreationArgs->GetParent());
	}
#ifdef ZN_FBX_SDK_ENABLE
	else if (Index == 1)
	{
		auto node = m_FBXManager->CreateSceneNode(sceneNodeCreationArgs->GetScene(), "SomeSceneName");
		if (sceneNodeCreationArgs->GetParent() != nullptr)
			sceneNodeCreationArgs->GetParent()->AddChild(std::dynamic_pointer_cast<ISceneNode3D>(node));
		return std::dynamic_pointer_cast<IObject>(node);
	}
#endif

	return nullptr;
}

CSceneNodeUIEngineCreator::CSceneNodeUIEngineCreator(IBaseManager & BaseManager)
	: CObjectClassCreator(BaseManager)
{
	AddKey(cSceneNodeUI);
	AddKey(cSceneNodeUI_Text);
	AddKey(cSceneNodeUI_Color);
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
	else if (Index == 2)
	{
		return sceneNodeCreationArgs->GetScene()->CreateSceneNodeUI<CUIColorNode>(sceneNodeCreationArgs->GetParent());
	}

	return nullptr;
}
