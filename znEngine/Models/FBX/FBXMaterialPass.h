#pragma once

class CFBXMaterialPass
	: public Base3DPass
{
public:
	CFBXMaterialPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline);
	virtual ~CFBXMaterialPass();

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	bool Visit(IGeometry* Geometry, const IShader* VertexShader, const SRenderGeometryArgs& RenderGeometryArgs) override;

private:

};