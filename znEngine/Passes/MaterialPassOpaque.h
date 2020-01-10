#pragma once

class CMaterialPassOpaque
	: public BuildRenderListPass
{
public:
	CMaterialPassOpaque(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene, std::shared_ptr<IPipelineState> Pipeline);
	virtual ~CMaterialPassOpaque();

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
};