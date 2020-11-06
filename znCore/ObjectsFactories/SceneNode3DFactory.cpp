#include "stdafx.h"

// General
#include "SceneNode3DFactory.h"

// Additional
#include "Files/File.h"
#include "XML/XMLManager.h"

CSceneNode3DFactory::CSceneNode3DFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type)
	: CObjectClassFactory(BaseManager, TypeName, Type)
{}

CSceneNode3DFactory::~CSceneNode3DFactory()
{}



//
// ISceneNodeFactory
//
std::shared_ptr<ISceneNode> CSceneNode3DFactory::CreateSceneNode3D(ObjectClass ObjectClassKey, IScene& Scene, const std::shared_ptr<ISceneNode>& Parent)
{
	CSceneNode3DCreationArgs creationArgs(Scene, Parent);
	return std::dynamic_pointer_cast<ISceneNode>(CreateObject(ObjectClassKey, &creationArgs));
}

std::shared_ptr<ISceneNode> CSceneNode3DFactory::LoadSceneNode3DXML(const std::shared_ptr<IXMLReader>& Reader, IScene& Scene, const std::shared_ptr<ISceneNode>& Parent)
{
	Guid guid = ReadGUIDXML(Reader);
	ObjectClass objectClass = guid.GetObjectClass();

	CSceneNode3DCreationArgs creationArgs(Scene, Parent);
	std::shared_ptr<ISceneNode> sceneNode = std::dynamic_pointer_cast<ISceneNode>(CreateObject(objectClass, &creationArgs));
	sceneNode->Load(Reader);
	return sceneNode;
}

std::shared_ptr<IXMLWriter> CSceneNode3DFactory::SaveSceneNode3DXML(std::shared_ptr<ISceneNode> SceneNode)
{
	auto xmlWriter = WriteGUIDXML(SceneNode->GetGUID());
	SceneNode->Save(xmlWriter);
	return xmlWriter;
}

std::shared_ptr<ISceneNode> CSceneNode3DFactory::LoadSceneNode3D(const std::shared_ptr<IByteBuffer>& Bytes, IScene& Scene, const std::shared_ptr<ISceneNode>& Parent)
{
	Guid guid = ReadGUID(Bytes);
	ObjectClass objectClass = guid.GetObjectClass();

	CSceneNode3DCreationArgs creationArgs(Scene, Parent);
	std::shared_ptr<ISceneNode> sceneNode = std::dynamic_pointer_cast<ISceneNode>(CreateObject(objectClass, &creationArgs));
	sceneNode->Load(Bytes);
	return sceneNode;
}

std::shared_ptr<IByteBuffer> CSceneNode3DFactory::SaveSceneNode3D(std::shared_ptr<ISceneNode> SceneNode)
{
	auto byteBuffer = WriteGUID(SceneNode->GetGUID());
	SceneNode->Save(byteBuffer);
	return byteBuffer;
}
