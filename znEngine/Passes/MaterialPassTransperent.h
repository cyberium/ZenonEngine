#pragma once

class CMaterialPassTransperent
	: public Base3DPass
{
public:
	CMaterialPassTransperent(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CMaterialPassTransperent();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	bool Visit(IMesh* Mesh, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
};