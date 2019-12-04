#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
class SceneNode3D;
class CUIBaseNode;
ZN_INTERFACE IMesh;
class CLight3D;
// FORWARD END

ZN_INTERFACE OW_ENGINE_API IVisitor : public std::enable_shared_from_this<IVisitor>
{
	virtual bool Visit(SceneNode3D* node) = 0;
	virtual bool Visit(CUIBaseNode* node) = 0;
	virtual bool Visit(IMesh* Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) = 0;
	virtual bool Visit(std::shared_ptr<CLight3D> light) = 0;
};