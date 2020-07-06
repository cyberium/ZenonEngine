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
#ifdef ZN_FBX_SDK_ENABLE
	AddKey(cSceneNode_FBXNode);
#endif
}

CSceneNode3DEngineCreator::~CSceneNode3DEngineCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNode3DEngineCreator::CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs)
{
	auto sceneNodeCreationArgs = static_cast<ISceneNode3DCreationArgs*>(const_cast<IObjectCreationArgs*>(ObjectCreationArgs));
	auto scene = sceneNodeCreationArgs->GetScene();
	auto parent = sceneNodeCreationArgs->GetParent();
	std::shared_ptr<ISceneNode3D> createdNode = nullptr;

	if (Index == 0)
	{
		createdNode = sceneNodeCreationArgs->GetScene()->CreateSceneNode3DInternal<SceneNode3D>();
	}
#ifdef ZN_FBX_SDK_ENABLE
	else if (Index == 1)
	{
		createdNode = scene->CreateSceneNode3DInternal<CFBXSceneNode>(GetBaseManager(), m_FBXManager->GetFBXManager());
	}
#endif

	if (createdNode == nullptr)
		throw CException("CSceneNode3DEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	if (parent)
		scene->AddChild(parent, createdNode);
	else if (scene->GetRootNode3D())
		scene->AddChild(scene->GetRootNode3D(), createdNode);

	return createdNode;
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
	auto sceneNodeCreationArgs = static_cast<ISceneNodeUICreationArgs*>(const_cast<IObjectCreationArgs*>(ObjectCreationArgs));
	auto scene = sceneNodeCreationArgs->GetScene();
	auto parent = sceneNodeCreationArgs->GetParent();
	std::shared_ptr<ISceneNodeUI> createdNode = nullptr;

	if (Index == 0)
	{
		createdNode = sceneNodeCreationArgs->GetScene()->CreateSceneNodeUIInternal<SceneNodeUI>();
	}
	else if (Index == 1)
	{
		createdNode = sceneNodeCreationArgs->GetScene()->CreateSceneNodeUIInternal<CUITextNode>();
	}
	else if (Index == 2)
	{
		createdNode = sceneNodeCreationArgs->GetScene()->CreateSceneNodeUIInternal<CUIColorNode>();
	}

	if (createdNode == nullptr)
		throw CException("CSceneNodeUIEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	if (parent)
		parent->AddChild(createdNode);
	else if (scene->GetRootNodeUI())
		scene->GetRootNodeUI()->AddChild(createdNode);

	return createdNode;
}
