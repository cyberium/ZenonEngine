#include "stdafx.h"

// General
#include "DragUtils.h"


//
// SceneBrowser
//
void CreateDragDataFromSceneNode(const std::shared_ptr<ISceneNode>& Node, CByteBuffer * ByteBuffer)
{
	if (Node == nullptr)
		throw CException("Unable create drag data from ISceneNode, because ISceneNode is nullptr.");

	if (ByteBuffer == nullptr || ByteBuffer->getSize() > 0 || ByteBuffer->getPos() > 0)
		throw CException("Unable create drag data from '%s' ISceneNode, because ByteBuffer is not empty.", Node->GetName().c_str());

	// 4 bytes - sourceType
	ETreeViewItemType sourceType = ETreeViewItemType::SceneNode;
	ByteBuffer->write(&sourceType);

	// 8 bytes - guid
	uint64 rawGuid = Node->GetGUID().GetRawValue();
	ByteBuffer->write(&rawGuid);
}



//
// ResourcesBrowser
//
void CreateDragDataFromSceneNodeProto(const std::shared_ptr<ISceneNode>& Node, CByteBuffer * ByteBuffer)
{
	if (Node == nullptr)
		throw CException("Unable create drag data from ISceneNode, because ISceneNode is nullptr.");

	if (ByteBuffer == nullptr || ByteBuffer->getSize() > 0 || ByteBuffer->getPos() > 0)
		throw CException("Unable create drag data from '%s' ISceneNode, because ByteBuffer is not empty.", Node->GetName().c_str());

	// 4 bytes - sourceType
	ETreeViewItemType sourceType = ETreeViewItemType::SceneNodeProto;
	ByteBuffer->write(&sourceType);

	// Такого говнокода я ещё в своей жизни не писал.
	intptr_t ptrToSharedPtr = (intptr_t)Node.get();
	ByteBuffer->write(&ptrToSharedPtr);

	// string - SceneNode guid
	//ByteBuffer->AppendPackedUInt64(Node->GetGUID());
}

void CreateDragDataFromModel(const std::shared_ptr<IModel>& Model, CByteBuffer * ByteBuffer)
{
	if (Model == nullptr)
		throw CException("Unable create drag data from IModel, because IModel is nullptr.");

	if (ByteBuffer == nullptr || ByteBuffer->getSize() > 0 || ByteBuffer->getPos() > 0)
		throw CException("Unable create drag data from '%s' IModel, because ByteBuffer is not empty.", Model->GetName().c_str());

	// 4 bytes - sourceType
	ETreeViewItemType sourceType = ETreeViewItemType::Model;
	ByteBuffer->write(&sourceType);

	// Model name
	ByteBuffer->writeString(Model->GetName());

	// Model filename
	ByteBuffer->writeString(Model->GetFileName());
}

void CreateDragDataFromTexture(const std::shared_ptr<ITexture>& Texture, CByteBuffer * ByteBuffer)
{
	if (Texture == nullptr)
		throw CException("Unable create drag data from ITexture, because ITexture is nullptr.");

	if (ByteBuffer == nullptr || ByteBuffer->getSize() > 0 || ByteBuffer->getPos() > 0)
		throw CException("Unable create drag data from '%s' ITexture, because ByteBuffer is not empty.", Texture->GetFilename().c_str());

	// 4 bytes - sourceType
	ETreeViewItemType sourceType = ETreeViewItemType::Texture;
	ByteBuffer->write(&sourceType);

	// Model filename
	ByteBuffer->writeString(Texture->GetFilename());
}

void CreateDragDataFromParticleSystem(const IBaseManager& BaseManager, const std::shared_ptr<IParticleSystem>& ParticleSystem, CByteBuffer * ByteBuffer)
{
	if (ParticleSystem == nullptr)
		throw CException("Unable create drag data from IParticleSystem, because IParticleSystem is nullptr.");

	if (ByteBuffer == nullptr || ByteBuffer->getSize() > 0 || ByteBuffer->getPos() > 0)
		throw CException("Unable create drag data from '%s' IParticleSystem, because ByteBuffer is not empty.", ParticleSystem->GetName());

	// 4 bytes - sourceType
	ETreeViewItemType sourceType = ETreeViewItemType::ParticleSystem;
	ByteBuffer->write(&sourceType);

	ByteBuffer->writeString(ParticleSystem->GetName());

	CXMLManager xml(BaseManager);
	
	auto xmlDocument = xml.CreateDocument();
	auto propertiesNode = xmlDocument->CreateChild("Props");
	ParticleSystem->Save(propertiesNode);

	std::string xmlDocumentAsString = xml.SaveWriterToString(xmlDocument);
	ByteBuffer->writeString(xmlDocumentAsString);
}



ETreeViewItemType GetDragDataSourceType(const CByteBuffer& ByteBuffer)
{
	if (ByteBuffer.getSize() == 0 || ByteBuffer.getPos() > 0)
		throw CException("Incorrect drag data ByteBuffer.");

	CByteBuffer byteBufferCopy(ByteBuffer);

	uint32 sourceTypeInt;
	if (false == byteBufferCopy.read(&sourceTypeInt))
		throw CException("Incorrect drag data ByteBuffer.");

	return static_cast<ETreeViewItemType>(sourceTypeInt);
}



//
// SceneBrowser
//
Guid GetSceneNodeFromDragData(const IBaseManager& BaseManager, const CByteBuffer& ByteBuffer)
{
	if (ByteBuffer.getSize() == 0 || ByteBuffer.getPos() > 0)
		throw CException("Incorrect drag data ByteBuffer.");

	CByteBuffer byteBufferCopy(ByteBuffer);

	uint32 sourceTypeInt;
	if (false == byteBufferCopy.read(&sourceTypeInt))
		throw CException("Incorrect drag data Buffer for ISceneNode.");

	ETreeViewItemType sourceType = static_cast<ETreeViewItemType>(sourceTypeInt);
	if (sourceType != ETreeViewItemType::SceneNode)
		throw CException("Drag data type '%d' is not ISceneNode drag data.", sourceTypeInt);

	uint64 guid;
	if (false == byteBufferCopy.read(&guid))
		throw CException("Incorrect drag data Buffer for ISceneNode.");

	return Guid(guid);
}



//
// ResourcesBrowser
//
std::shared_ptr<ISceneNode> GetSceneNodeProtoFromDragData(const IBaseManager& BaseManager, IScene& Scene, const CByteBuffer& ByteBuffer)
{
	if (ByteBuffer.getSize() == 0 || ByteBuffer.getPos() > 0)
		throw CException("Incorrect drag data ByteBuffer.");

	CByteBuffer byteBufferCopy(ByteBuffer);

	uint32 sourceTypeInt;
	if (false == byteBufferCopy.read(&sourceTypeInt))
		throw CException("Incorrect drag data Buffer for ISceneNode.");
	ETreeViewItemType sourceType = static_cast<ETreeViewItemType>(sourceTypeInt);
	if (sourceType != ETreeViewItemType::SceneNodeProto)
		throw CException("Drag data type '%d' is not ISceneNode drag data.", sourceTypeInt);

	intptr_t ptr;
	byteBufferCopy.read(&ptr);

	ISceneNode* ptrPtr = reinterpret_cast<ISceneNode*>(ptr);

	auto copy = BaseManager.GetManager<IObjectsFactory>()->GetClassFactoryCast<CSceneNodeFactory>()->CreateSceneNode3D(ptrPtr->GetClass(), Scene, Scene.GetRootSceneNode());
	ptrPtr->CopyTo(copy);
	return copy;
}

std::shared_ptr<IModel> GetModelFromDragData(const IBaseManager& BaseManager, const CByteBuffer& ByteBuffer)
{
	if (ByteBuffer.getSize() == 0 || ByteBuffer.getPos() > 0)
		throw CException("Incorrect drag data ByteBuffer.");

	CByteBuffer byteBufferCopy(ByteBuffer);

	uint32 sourceTypeInt;
	if (false == byteBufferCopy.read(&sourceTypeInt))
		throw CException("Incorrect drag data Buffer for IModel.");
	ETreeViewItemType sourceType = static_cast<ETreeViewItemType>(sourceTypeInt);
	if (sourceType != ETreeViewItemType::Model)
		throw CException("Drag data type '%d' is not IModel drag data.", sourceTypeInt);

	std::string modelName;
	if (false == byteBufferCopy.readString(&modelName))
		throw CException("Incorrect drag data ByteBuffer.");

	std::string modelFileName;
	if (false == byteBufferCopy.readString(&modelFileName))
		throw CException("Incorrect drag data ByteBuffer.");

	try
	{
		std::shared_ptr<IModel> model = BaseManager.GetManager<IznModelsFactory>()->LoadModel(modelFileName);
		model->SetName(modelName);
		return model;
	}
	catch (const CException& e)
	{
		Log::Error("Unable to create IModel with name '%s' and filename '%s' from drag data.", modelName.c_str(), modelFileName.c_str());
		Log::Error("--->%s", e.MessageCStr());
	}

	return nullptr;
}

std::shared_ptr<ITexture> GetTextureFromDragData(const IBaseManager& BaseManager, const CByteBuffer& ByteBuffer)
{
	if (ByteBuffer.getSize() == 0 || ByteBuffer.getPos() > 0)
		throw CException("Incorrect drag data ByteBuffer.");

	CByteBuffer byteBufferCopy(ByteBuffer);

	uint32 sourceTypeInt;
	if (false == byteBufferCopy.read(&sourceTypeInt))
		throw CException("Incorrect drag data Buffer for ITexture.");
	ETreeViewItemType sourceType = static_cast<ETreeViewItemType>(sourceTypeInt);
	if (sourceType != ETreeViewItemType::Texture)
		throw CException("Drag data type '%d' is not ITexture drag data.", sourceTypeInt);

	std::string textureFilename;
	if (false == byteBufferCopy.readString(&textureFilename))
		throw CException("Incorrect drag data ByteBuffer.");

	try
	{
		std::shared_ptr<ITexture> texture = BaseManager.GetManager<IznTexturesFactory>()->LoadTexture2D(textureFilename);
		texture->SetName(textureFilename);
		return texture;
	}
	catch (const CException& e)
	{
		Log::Error("Unable to create ITexture with filename '%s' from drag data.", textureFilename.c_str());
		Log::Error("--->%s", e.MessageCStr());
	}

	return nullptr;
}


std::shared_ptr<IParticleSystem> GetParticleSystemFromDragData(const IBaseManager& BaseManager, const CByteBuffer& ByteBuffer)
{
	if (ByteBuffer.getSize() == 0 || ByteBuffer.getPos() > 0)
		throw CException("Incorrect drag data ByteBuffer.");

	CByteBuffer byteBufferCopy(ByteBuffer);

	uint32 sourceTypeInt;
	if (false == byteBufferCopy.read(&sourceTypeInt))
		throw CException("Incorrect drag data Buffer for IParticleSystem.");
	ETreeViewItemType sourceType = static_cast<ETreeViewItemType>(sourceTypeInt);
	if (sourceType != ETreeViewItemType::ParticleSystem)
		throw CException("Drag data type '%d' is not IParticleSystem drag data.", sourceTypeInt);

	std::string particleSystemName;
	if (false == byteBufferCopy.readString(&particleSystemName))
		throw CException("Incorrect drag data ByteBuffer.");

	std::string particleSystemXMLString;
	if (false == byteBufferCopy.readString(&particleSystemXMLString))
		throw CException("Incorrect drag data ByteBuffer.");

	CXMLManager xml(BaseManager);
	auto xmlDocument = xml.CreateReaderFromString(particleSystemXMLString);
	auto propertiesNode = xmlDocument->GetChilds()[0];

	try
	{
		std::shared_ptr<IParticleSystem> particleSystem = MakeShared(CParticleSystem, BaseManager);
		particleSystem->SetName(particleSystemName);
		particleSystem->Load(propertiesNode);
		return particleSystem;
	}
	catch (const CException& e)
	{
		Log::Error("Unable to create IParticleSystem with name '%s' from drag data.", particleSystemName.c_str());
		Log::Error("--->%s", e.MessageCStr());
	}

	return nullptr;
}