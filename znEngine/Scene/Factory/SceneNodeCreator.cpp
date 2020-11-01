#include "stdafx.h"

// General
#include "SceneNodeCreator.h"

// Additional
#include "Scene/Nodes/UIText.h"
#include "Scene/Nodes/UIColor.h"
#include "Scene/RTSSceneNodeGround.h"

CSceneNode3DEngineCreator::CSceneNode3DEngineCreator(IBaseManager& BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{
	AddKey("SceneNode3D", cSceneNode3D);
	AddKey("RTSSceneNodeGround", cCRTSSceneNodeGround);
}

CSceneNode3DEngineCreator::~CSceneNode3DEngineCreator()
{}



//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNode3DEngineCreator::CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	auto sceneNodeCreationArgs = static_cast<const ISceneNode3DCreationArgs*>(ObjectCreationArgs);
	auto scene = sceneNodeCreationArgs->GetScene();
	auto parent = sceneNodeCreationArgs->GetParent();
	std::shared_ptr<ISceneNode3D> createdNode = nullptr;

	// 1. Create object
	if (Index == 0)
	{
		createdNode = sceneNodeCreationArgs->GetScene()->CreateSceneNode3DInternal<SceneNode3D>();
	}
	else if (Index == 1)
	{
		createdNode = sceneNodeCreationArgs->GetScene()->CreateSceneNode3DInternal<CRTSSceneNodeGround>();
	}

	// 2. Check
	if (createdNode == nullptr)
		throw CException("CSceneNode3DEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	// 3. Assign GUID
	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNode))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("SceneNode3DEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	// 4. SceneNode specific
	if (parent)
		scene->AddChild(parent, createdNode);
	else if (scene->GetRootNode3D())
		scene->AddChild(scene->GetRootNode3D(), createdNode);

	return createdNode;
}



//
// CSceneNodeUIEngineCreator
//
CSceneNodeUIEngineCreator::CSceneNodeUIEngineCreator(IBaseManager & BaseManager)
	: CObjectClassCreatorBase(BaseManager)
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
std::shared_ptr<IObject> CSceneNodeUIEngineCreator::CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	auto sceneNodeCreationArgs = static_cast<const ISceneNodeUICreationArgs*>(ObjectCreationArgs);
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
		throw CException("SceneNodeUIEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNode))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("SceneNodeUIEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	if (parent)
		parent->AddChild(createdNode);
	else if (scene->GetRootNodeUI())
		scene->GetRootNodeUI()->AddChild(createdNode);

	return createdNode;
}
