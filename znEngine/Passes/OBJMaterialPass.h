#pragma once

class COBJMaterialPass : public Base3DPass
{
public:
	COBJMaterialPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline);
	virtual ~COBJMaterialPass();

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	bool Visit(IGeometry* Geometry, const IShader* VertexShader, const SRenderGeometryArgs& RenderGeometryArgs) override;
};