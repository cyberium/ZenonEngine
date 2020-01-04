#pragma once

#include "Interfaces/Render/Types/RenderGeometryParams.h"
#include "Interfaces/Render/Types/ShaderType.h"

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
ZN_INTERFACE ISceneNode3D;
ZN_INTERFACE ISceneNodeUI;
ZN_INTERFACE IMesh;
ZN_INTERFACE IGeometry;
ZN_INTERFACE IConstantBuffer;
ZN_INTERFACE IShader;
ZN_INTERFACE IMaterial;
class CLight3D;
// FORWARD END

ZN_INTERFACE ZN_API IVisitor : public std::enable_shared_from_this<IVisitor>
{
	virtual ~IVisitor() {}

	virtual bool VisitBase(ISceneNode* SceneNode) = 0;
	virtual bool Visit3D(ISceneNode* node) = 0;
	virtual bool VisitUI(ISceneNode* node) = 0;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) = 0;
	virtual bool Visit(IGeometry* Geometry, const IConstantBuffer* PerObject, const std::unordered_map<SShaderType, std::shared_ptr<IShader>>& ShadersMap, const IMaterial* Material, const SGeometryPartParams& GeometryPartParams) = 0;
	virtual bool Visit(std::shared_ptr<CLight3D> light) = 0;
};