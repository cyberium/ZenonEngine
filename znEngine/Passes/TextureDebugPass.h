#pragma once

class CDebugMaterialPass : public Base3DPass
{
public:
	CDebugMaterialPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline);
	virtual ~CDebugMaterialPass();

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
};