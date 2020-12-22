#pragma once

enum class EDragDataSourceType : uint32
{
	Texture = 0,
	Material,
	Geometry,
	Model,
	ParticleSytem,
	SceneNodeProto
};

void                          CreateDragDataFromSceneNode(const std::shared_ptr<ISceneNode>& Node, CByteBuffer * ByteBuffer);
void                          CreateDragDataFromModel(const std::shared_ptr<IModel>& Model, CByteBuffer * ByteBuffer);
void                          CreateDragDataFromTexture(const std::shared_ptr<ITexture>& Texture, CByteBuffer * ByteBuffer);
void                          CreateDragDataFromParticleSystem(const IBaseManager& BaseManager, const std::shared_ptr<IParticleSystem>& ParticleSystem, CByteBuffer * ByteBuffer);

EDragDataSourceType           GetDragDataSourceType(const CByteBuffer& ByteBuffer);

std::shared_ptr<ISceneNode>   GetSceneNodeFromDragData(IBaseManager& BaseManager, IScene& Scene, const CByteBuffer& ByteBuffer);
std::shared_ptr<IModel>       GetModelFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);
std::shared_ptr<ITexture>     GetTextureFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);
std::shared_ptr<IParticleSystem> GetParticleSystemFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);