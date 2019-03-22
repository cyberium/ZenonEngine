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
	virtual bool Visit(IMesh& mesh, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) = 0;
	virtual bool Visit(CLight3D& light) = 0;
};