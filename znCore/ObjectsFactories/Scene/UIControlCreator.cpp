#include "stdafx.h"

// General
#include "UIControlCreator.h"

CUIControlCreator::CUIControlCreator(IBaseManager & BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{}

CUIControlCreator::~CUIControlCreator()
{}



//
// IObjectClassCreator
//
std::shared_ptr<IObject> CUIControlCreator::CreateObject(ObjectClass ObjectClass, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	std::shared_ptr<IObject> createdNodeAsObject = GetObjectClassFunctorCretor(ObjectClass)(ObjectCreationArgs);
	if (createdNodeAsObject == nullptr)
		throw CException("ComponentsEngineCreator: CreateObject: Unable to create '%s'.", GetObjectClassName(ObjectClass).c_str());

	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdNodeAsObject))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("UIControlCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

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
