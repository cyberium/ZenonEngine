#pragma once

class CMaterialPassOpaque
	: public Base3DPass
{
public:
	CMaterialPassOpaque(IRenderDevice* RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CMaterialPassOpaque();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(IRenderTarget* RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
};