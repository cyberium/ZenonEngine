#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IMesh;
class CLight3D;
// FORWARD END

ZN_INTERFACE ZN_API IVisitor : public std::enable_shared_from_this<IVisitor>
{
	virtual ~IVisitor() {}

	virtual bool VisitBase(ISceneNode* SceneNode) = 0;
	virtual bool Visit3D(ISceneNode* node) = 0;
	virtual bool VisitUI(ISceneNode* node) = 0;
	virtual bool Visit(IMesh* Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) = 0;
	virtual bool Visit(std::shared_ptr<CLight3D> light) = 0;
};