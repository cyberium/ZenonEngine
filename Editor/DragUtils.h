#pragma once

enum class EDragDataSourceType : uint32
{
	Texture = 0,
	Material,
	Geometry,
	Model,
	SceneNodeProto
};

void                          CreateDragDataFromSceneNode(const std::shared_ptr<ISceneNode>& Node, CByteBuffer * ByteBuffer);
void                          CreateDragDataFromModel(const std::shared_ptr<IModel>& Model, CByteBuffer * ByteBuffer);
void                          CreateDragDataFromTexture(const std::shared_ptr<ITexture>& Texture, CByteBuffer * ByteBuffer);

EDragDataSourceType           GetDragDataSourceType(const CByteBuffer& ByteBuffer);

std::shared_ptr<ISceneNode>   GetSceneNodeFromDragData(IBaseManager& BaseManager, IScene& Scene, const CByteBuffer& ByteBuffer);
std::shared_ptr<IModel>       GetModelFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);
std::shared_ptr<ITexture>     GetTextureFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);
