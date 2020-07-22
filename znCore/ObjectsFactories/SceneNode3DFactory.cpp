#include "stdafx.h"

// General
#include "SceneNode3DFactory.h"

// Additional
#include "Scene/3D/SceneNode3D.h"
#include "Scene/UI/SceneNodeUI.h"
#include "Files/File.h"
#include "XML/XMLManager.h"

CSceneNode3DFactory::CSceneNode3DFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type)
	: CObjectClassFactory(BaseManager, TypeName, Type)
{}

CSceneNode3DFactory::~CSceneNode3DFactory()
{}



//
// ISceneNode3DFactory
//
std::shared_ptr<ISceneNode3D> CSceneNode3DFactory::CreateSceneNode3D(ObjectClass ObjectClassKey, IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent)
{
	CSceneNode3DCreationArgs creationArgs(Scene, Parent);
	return std::dynamic_pointer_cast<ISceneNode3D>(CreateObject(ObjectClassKey, &creationArgs));
}
std::shared_ptr<ISceneNode3D> CSceneNode3DFactory::LoadSceneNode3DXML(const std::shared_ptr<IXMLReader>& Reader, IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent)
{
	Guid guid = ReadGUIDXML(Reader);

	CSceneNode3DCreationArgs creationArgs(Scene, Parent);
	auto createdbject = CreateObject(guid.GetObjectClass(), &creationArgs);
	if (auto objectLoadSave = std::dynamic_pointer_cast<IObjectLoadSave>(createdbject))
		objectLoadSave->Load(Reader);

	return std::dynamic_pointer_cast<ISceneNode3D>(createdbject);
}

std::shared_ptr<IXMLWriter> CSceneNode3DFactory::SaveSceneNode3DXML(std::shared_ptr<ISceneNode3D> Object)
{
	auto writer = WriteGUIDXML(Object->GetGUID());
	if (auto objectLoadSave = std::dynamic_pointer_cast<IObjectLoadSave>(Object))
		objectLoadSave->Save(writer);

	return writer;
}

std::shared_ptr<ISceneNode3D> CSceneNode3DFactory::LoadSceneNode3D(const std::shared_ptr<IByteBuffer>& Bytes, IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent)
{
	Guid guid = ReadGUID(Bytes);

	CSceneNode3DCreationArgs creationArgs(Scene, Parent);
	std::shared_ptr<IObject> createdbject = CreateObject(guid.GetObjectClass(), &creationArgs);
	if (auto objectLoadSave = std::dynamic_pointer_cast<IObjectLoadSave>(createdbject))
		objectLoadSave->Load(Bytes);

	return std::dynamic_pointer_cast<ISceneNode3D>(createdbject);
}

std::shared_ptr<IByteBuffer> CSceneNode3DFactory::SaveSceneNode3D(std::shared_ptr<ISceneNode3D> Object)
{
	std::shared_ptr<IByteBuffer> byteBuffer = WriteGUID(Object->GetGUID());

	if (auto objectLoadSave = std::dynamic_pointer_cast<IObjectLoadSave>(Object))
		objectLoadSave->Save(byteBuffer);

	return byteBuffer;
}
