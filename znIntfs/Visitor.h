#pragma once

// FORWARD BEGIN
class SceneNode3D;
class CUIBaseNode;
class IMesh;
class CLight3D;
// FORWARD END

struct IVisitor
{
	virtual bool Visit(SceneNode3D& node) = 0;
	virtual bool Visit(CUIBaseNode& node) = 0;
	virtual bool Visit(IMesh& Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) = 0;
	virtual bool Visit(CLight3D& light) = 0;
};