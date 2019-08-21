#pragma once

// FORWARD BEGIN
class SceneNode;
class SceneNode3D;
class CUIBaseNode;
class IMesh;
class CLight3D;
// FORWARD END

class OW_ENGINE_API IVisitor: public std::enable_shared_from_this<IVisitor>
{
public:
	virtual bool Visit(std::shared_ptr<SceneNode3D> node) = 0;
	virtual bool Visit(std::shared_ptr<CUIBaseNode> node) = 0;
	virtual bool Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) = 0;
	virtual bool Visit(std::shared_ptr<CLight3D> light) = 0;
};