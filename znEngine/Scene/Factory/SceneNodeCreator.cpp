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
	AddKey("SceneNode", cSceneNode3D, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNode, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});

	AddKey("SceneNodeRTSGround", cSceneNodeRTSGround, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNode, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});

	AddKey("SceneNodeRTSPath", cSceneNodeRTSPath, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNodeRTSPath, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});

	AddKey("SceneNodeRTSPoint", cSceneNodeRTSPoint, [this] (const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNodeRTSPoint, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<IColliderComponent3D>(cSceneNodeColliderComponent, *node));
		return node;
	});

	AddKey("SceneNodeRTSUnit", cSceneNodeRTSUnit, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNodeRTSUnit, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});

	AddKey("SceneNodeRTSBullet", cSceneNodeRTSBullet, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNodeRTSBullet, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});

	AddKey("SceneNodeRTSTower", cSceneNodeRTSTower, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNodeRTSTower, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});
}

CSceneNodeEngineCreator::~CSceneNodeEngineCreator()
{}



//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNodeEngineCreator::CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	std::shared_ptr<IObject> createdNodeAsObject = GetSupportedClassFunction(Index)(ObjectCreationArgs);
	if (createdNodeAsObject == nullptr)
		throw CException("CSceneNodeEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNodeAsObject))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("SceneNode3DEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	std::shared_ptr<ISceneNode> createdNode = std::dynamic_pointer_cast<ISceneNode>(createdNodeAsObject);
	createdNode->Initialize();

	auto sceneNodeCreationArgs = static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs);
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
	AddKey("CUIControl", cSceneNodeUI, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIControl, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
	AddKey("SceneNodeUIText", cSceneNodeUI_Text, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUITextNode, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
	AddKey("SceneNodeUIColor", cSceneNodeUI_Color, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIColorNode, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
}

CSceneNodeUIEngineCreator::~CSceneNodeUIEngineCreator()
{}



//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNodeUIEngineCreator::CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	std::shared_ptr<IObject> createdNodeAsObject = GetSupportedClassFunction(Index)(ObjectCreationArgs);
	if (createdNodeAsObject == nullptr)
		throw CException("SceneNodeUIEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNodeAsObject))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("SceneNodeUIEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	std::shared_ptr<IUIControl> createdNode = std::dynamic_pointer_cast<IUIControl>(createdNodeAsObject);
	createdNode->Initialize();

	auto sceneNodeCreationArgs = static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs);
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
