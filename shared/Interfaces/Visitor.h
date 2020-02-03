#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"
#include "Interfaces/Render/Types/ShaderType.h"

// FORWARD BEGIN
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IMesh;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IMaterial;
ZN_INTERFACE ILightComponent3D;
// FORWARD END

ZN_INTERFACE ZN_API IVisitor : public std::enable_shared_from_this<IVisitor>
{
	virtual ~IVisitor() {}

	virtual bool Visit(ISceneNode3D* node) = 0;
	virtual bool Visit(ISceneNodeUI* node) = 0;
	virtual bool Visit(IMesh* Mesh, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) = 0;
	virtual bool Visit(ILightComponent3D* Light) = 0;
};