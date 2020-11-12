#include "stdafx.h"

// General
#include "ComponentsFactory.h"

// Additional
#include "Files/File.h"
#include "XML/XMLManager.h"

CComponentsFactory::CComponentsFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type)
	: CObjectClassFactory(BaseManager, TypeName, Type)
{}

CComponentsFactory::~CComponentsFactory()
{}



//
// IComponentFactory
//
std::shared_ptr<ISceneNodeComponent> CComponentsFactory::CreateComponent(ObjectClass ObjectClassKey, ISceneNode & SceneNode)
{
	CComponentCreationArgs creationArgs(SceneNode);
	return std::dynamic_pointer_cast<ISceneNodeComponent>(CreateObject(ObjectClassKey, &creationArgs));
}

std::shared_ptr<ISceneNodeComponent> CComponentsFactory::LoadComponentXML(const std::shared_ptr<IXMLReader>& Reader, ISceneNode & SceneNode)
{
	Guid guid = ReadGUIDXML(Reader);

	CComponentCreationArgs creationArgs(SceneNode);
	auto sceneNodeComponent = std::dynamic_pointer_cast<ISceneNodeComponent>(CreateObject(guid.GetObjectClass(), &creationArgs));
	sceneNodeComponent->Load(Reader);
	return sceneNodeComponent;
}

std::shared_ptr<IXMLWriter> CComponentsFactory::SaveComponentXML(std::shared_ptr<ISceneNodeComponent> SceneNodeComponent)
{
	auto writer = WriteGUIDXML(SceneNodeComponent->GetGUID());
	SceneNodeComponent->Save(writer);
	return writer;
}
