#include "stdafx.h"

// General
#include "SceneNodeCreator.h"



CSceneNodeCreator::CSceneNodeCreator(IBaseManager& BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{}

CSceneNodeCreator::~CSceneNodeCreator()
{}



//
// IObjectClassCreator
//
std::shared_ptr<IObject> CSceneNodeCreator::CreateObject(ObjectClass ObjectClass, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	std::shared_ptr<IObject> createdNodeAsObject = GetObjectClassFunctorCretor(ObjectClass)(ObjectCreationArgs);
	if (createdNodeAsObject == nullptr)
		throw CException("ComponentsEngineCreator: CreateObject: Unable to create '%s'.", GetObjectClassName(ObjectClass).c_str());

	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNodeAsObject))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("SceneNodeEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

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
