#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IMesh;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IShader;
class CLight3D;
// FORWARD END

ZN_INTERFACE ZN_API IVisitor : public std::enable_shared_from_this<IVisitor>
{
	virtual ~IVisitor() {}

	virtual bool VisitBase(ISceneNode* SceneNode) = 0;
	virtual bool Visit3D(ISceneNode* node) = 0;
	virtual bool VisitUI(ISceneNode* node) = 0;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) = 0;
	virtual bool Visit(IGeometry* Geometry, const IShader* VertexShader, const SRenderGeometryArgs& RenderGeometryArgs) = 0;
	virtual bool Visit(std::shared_ptr<CLight3D> light) = 0;
};