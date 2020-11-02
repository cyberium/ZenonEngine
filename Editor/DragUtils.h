#pragma once

enum class EDragDataSourceType : uint32
{
	Texture = 0,
	Material,
	Geometry,
	Model,
	SceneNode
};

void CreateDragDataFromSceneNode(const std::shared_ptr<ISceneNode>& Node, CByteBuffer * ByteBuffer);
void CreateDragDataFromModel(const IModelPtr& Model, CByteBuffer * ByteBuffer);

EDragDataSourceType           GetDragDataSourceType(const CByteBuffer& ByteBuffer);
std::shared_ptr<ISceneNode> GetSceneNodeFromDragData(IBaseManager& BaseManager, IScene& Scene, const CByteBuffer& ByteBuffer);
std::shared_ptr<IModel>       GetModelFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);
