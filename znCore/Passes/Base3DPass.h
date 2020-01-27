#pragma once

#include "ScenePassPipelined.h"

__declspec(align(16)) struct ZN_API PerObject3D
{
	glm::mat4 Model;
};


class ZN_API Base3DPass 
	: public ScenePassPipelined
{
public:
	Base3DPass(IRenderDevice* RenderDevice, std::shared_ptr<IScene> scene);
	virtual ~Base3DPass();

	// IVisitor
	virtual bool Visit(ISceneNode3D* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

protected:
	PerObject3D* m_PerObjectData;
	IConstantBuffer* m_PerObjectConstantBuffer;
};