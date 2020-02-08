#pragma once

class CMaterialPassOpaque
	: public Base3DPass
{
public:
	CMaterialPassOpaque(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CMaterialPassOpaque();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	bool Visit(const IModel* Model) override;
	bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
};