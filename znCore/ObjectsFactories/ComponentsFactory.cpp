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

std::shared_ptr<ISceneNodeComponent> CComponentsFactory::CreateComponent(ObjectClass ObjectClassKey, ISceneNode & SceneNode)
{
	CComponentCreationArgs creationArgs(SceneNode);
	return std::dynamic_pointer_cast<ISceneNodeComponent>(CreateObject(ObjectClassKey, &creationArgs));
}

std::shared_ptr<ISceneNodeComponent> CComponentsFactory::LoadComponentXML(const std::shared_ptr<IXMLReader>& Reader, ISceneNode & SceneNode)
{
	Guid guid = ReadGUIDXML(Reader);

	CComponentCreationArgs creationArgs(SceneNode);
	auto createdbject = CreateObject(guid.GetObjectClass(), &creationArgs);
	if (auto objectLoadSave = std::dynamic_pointer_cast<IObjectLoadSave>(createdbject))
		objectLoadSave->Load(Reader);

	return std::dynamic_pointer_cast<ISceneNodeComponent>(createdbject);
}

std::shared_ptr<IXMLWriter> CComponentsFactory::SaveComponentXML(std::shared_ptr<ISceneNodeComponent> Object)
{
	auto writer = WriteGUIDXML(Object->GetGUID());
	if (auto objectLoadSave = std::dynamic_pointer_cast<IObjectLoadSave>(Object))
		objectLoadSave->Save(writer);

	return writer;
}
