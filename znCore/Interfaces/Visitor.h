#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
class SceneNodeBase;
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IMesh;
class CLight3D;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API IVisitor : public std::enable_shared_from_this<IVisitor>
{
	virtual ~IVisitor() {}

	virtual bool Visit(SceneNodeBase* SceneNode) = 0;
	virtual bool Visit(ISceneNode3D* node) = 0;
	virtual bool Visit(ISceneNodeUI* node) = 0;
	virtual bool Visit(IMesh* Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) = 0;
	virtual bool Visit(std::shared_ptr<CLight3D> light) = 0;
};