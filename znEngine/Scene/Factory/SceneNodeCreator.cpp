#include "stdafx.h"

// General
#include "SceneNodeCreator.h"

// Additional
#include "Scene/Nodes/UIText.h"
#include "Scene/Nodes/UIColor.h"

CSceneNode3DEngineCreator::CSceneNode3DEngineCreator(IBaseManager& BaseManager)
	: CObjectClassCreator(BaseManager)
{
	//m_FBXManager = MakeShared(CFBXManager, GetBaseManager());

	AddKey("SceneNode3D", cSceneNode3D);
//#ifdef ZN_FBX_SDK_ENABLE
//	AddKey("SceneNodeFBX", cSceneNode_FBXNode);
//#endif
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
//#ifdef ZN_FBX_SDK_ENABLE
//	else if (Index == 1)
//	{
//		createdNode = scene->CreateSceneNode3DInternal<CFBXNode>(GetBaseManager(), m_FBXManager->GetFBXManager());
//	}
//#endif

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
	AddKey("SceneNodeUI", cSceneNodeUI);
	AddKey("SceneNodeUIText", cSceneNodeUI_Text);
	AddKey("SceneNodeUIColor", cSceneNodeUI_Color);
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
