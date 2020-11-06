#include "stdafx.h"

// General
#include "SceneNodeCreator.h"

// Additional
#include "Scene/Nodes/UIText.h"
#include "Scene/Nodes/UIColor.h"

#include "Scene/SceneNodeRTSGround.h"
#include "Scene/SceneNodeRTSPathAndPoint.h"
#include "Scene/SceneNodeRTSUnit.h"
#include "Scene/SceneNodeRTSTower.h"

CSceneNodeEngineCreator::CSceneNodeEngineCreator(IBaseManager& BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{
	AddKey("SceneNode", cSceneNode3D);
	AddKey("SceneNodeRTSGround", cSceneNodeRTSGround);
	AddKey("SceneNodeRTSPath", cSceneNodeRTSPath);
	AddKey("SceneNodeRTSPoint", cSceneNodeRTSPoint);
	AddKey("SceneNodeRTSUnit", cSceneNodeRTSUnit);
	AddKey("SceneNodeRTSBullet", cSceneNodeRTSBullet);
	AddKey("SceneNodeRTSTower", cSceneNodeRTSTower);
}

CSceneNodeEngineCreator::~CSceneNodeEngineCreator()
{}



//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNodeEngineCreator::CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	auto sceneNodeCreationArgs = static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs);
	
	// 1. Create object
	std::shared_ptr<ISceneNode> createdNode = nullptr;
	if (Index == 0)
	{
		std::shared_ptr<CSceneNode> node = MakeShared(CSceneNode, sceneNodeCreationArgs->GetScene());
		node->RegisterComponents();
		node->Initialize();
		createdNode = node;
	}
	else if (Index == 1)
	{
		std::shared_ptr<CSceneNodeRTSGround> node = MakeShared(CSceneNodeRTSGround, sceneNodeCreationArgs->GetScene());
		node->RegisterComponents();
		node->Initialize();
		createdNode = node;
	}
	else if (Index == 2)
	{
		std::shared_ptr<CSceneNodeRTSPath> node = MakeShared(CSceneNodeRTSPath, sceneNodeCreationArgs->GetScene());
		node->Initialize();
		createdNode = node;
	}
	else if (Index == 3)
	{
		std::shared_ptr<CSceneNodeRTSPoint> node = MakeShared(CSceneNodeRTSPoint, sceneNodeCreationArgs->GetScene());
		node->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<IColliderComponent3D>(cSceneNodeColliderComponent, *node));
		node->Initialize();
		createdNode = node;
	}
	else if (Index == 4)
	{
		std::shared_ptr<CSceneNodeRTSUnit> node = MakeShared(CSceneNodeRTSUnit, sceneNodeCreationArgs->GetScene());
		node->RegisterComponents();
		node->Initialize();
		createdNode = node;
	}
	else if (Index == 5)
	{
		std::shared_ptr<CSceneNodeRTSBullet> node = MakeShared(CSceneNodeRTSBullet, sceneNodeCreationArgs->GetScene());
		node->RegisterComponents();
		node->Initialize();
		createdNode = node;
	}
	else if (Index == 6)
	{
		std::shared_ptr<CSceneNodeRTSTower> node = MakeShared(CSceneNodeRTSTower, sceneNodeCreationArgs->GetScene());
		node->RegisterComponents();
		node->Initialize();
		createdNode = node;
	}

	// 2. Check
	if (createdNode == nullptr)
		throw CException("CSceneNodeEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	// 3. Assign GUID
	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNode))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("SceneNode3DEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	// 4. SceneNode specific
	if (auto parent = sceneNodeCreationArgs->GetParent())
	{
		parent->AddChild(createdNode);
	}
	else if (auto sceneRoot = sceneNodeCreationArgs->GetScene().GetRootSceneNode())
	{
		sceneRoot->AddChild(createdNode);
	}

	return createdNode;
}



//
// CSceneNodeUIEngineCreator
//
CSceneNodeUIEngineCreator::CSceneNodeUIEngineCreator(IBaseManager & BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{
	AddKey("CUIControl", cSceneNodeUI);
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
	auto sceneNodeCreationArgs = static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs);

	// 1. Create object
	std::shared_ptr<IUIControl> createdNode = nullptr;
	if (Index == 0)
	{
		std::shared_ptr<CUIControl> newNode = MakeShared(CUIControl, sceneNodeCreationArgs->GetScene());
		newNode->Initialize();
		createdNode = newNode;
	}
	else if (Index == 1)
	{
		std::shared_ptr<CUIControl> newNode = MakeShared(CUITextNode, sceneNodeCreationArgs->GetScene());
		newNode->Initialize();
		createdNode = newNode;
	}
	else if (Index == 2)
	{
		std::shared_ptr<CUIControl> newNode = MakeShared(CUIColorNode, sceneNodeCreationArgs->GetScene());
		newNode->Initialize();
		createdNode = newNode;
	}

	// 2. Check
	if (createdNode == nullptr)
		throw CException("SceneNodeUIEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	// 3. Assign GUID
	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNode))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("SceneNodeUIEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	// 4. Control specific
	if (auto parent = sceneNodeCreationArgs->GetParent())
	{
		parent->AddChild(createdNode);
	}
	else if (auto sceneRoot = sceneNodeCreationArgs->GetScene().GetRootUIControl())
	{
		sceneRoot->AddChild(createdNode);
	}

	return createdNode;
}
