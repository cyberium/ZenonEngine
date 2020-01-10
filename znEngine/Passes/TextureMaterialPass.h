#pragma once

class CTexturedMaterialPass : public Base3DPass
{
public:
	CTexturedMaterialPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline);
	virtual ~CTexturedMaterialPass();

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
};