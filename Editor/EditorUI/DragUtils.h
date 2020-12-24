#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewIntfs.h"

// SceneBrowser
void                          CreateDragDataFromSceneNode(const std::shared_ptr<ISceneNode>& Node, CByteBuffer * ByteBuffer);

// ResourcesBrowser
void                          CreateDragDataFromSceneNodeProto(const std::shared_ptr<ISceneNode>& Node, CByteBuffer * ByteBuffer);
void                          CreateDragDataFromModel(const std::shared_ptr<IModel>& Model, CByteBuffer * ByteBuffer);
void                          CreateDragDataFromTexture(const std::shared_ptr<ITexture>& Texture, CByteBuffer * ByteBuffer);
void                          CreateDragDataFromParticleSystem(const IBaseManager& BaseManager, const std::shared_ptr<IParticleSystem>& ParticleSystem, CByteBuffer * ByteBuffer);

ETreeViewItemType             GetDragDataSourceType(const CByteBuffer& ByteBuffer);

// SceneBrowser
Guid                         GetSceneNodeFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);

// ResourceBrowser
std::shared_ptr<ISceneNode>   GetSceneNodeProtoFromDragData(IBaseManager& BaseManager, IScene& Scene, const CByteBuffer& ByteBuffer);
std::shared_ptr<IModel>       GetModelFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);
std::shared_ptr<ITexture>     GetTextureFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);
std::shared_ptr<IParticleSystem> GetParticleSystemFromDragData(IBaseManager& BaseManager, const CByteBuffer& ByteBuffer);