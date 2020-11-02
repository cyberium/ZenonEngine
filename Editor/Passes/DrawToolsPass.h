#pragma once

class CDrawToolsPass
	: public Base3DPass
{
public:
	CDrawToolsPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CDrawToolsPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override final;

	// IVisitor
	EVisitResult Visit(const IModel* Model) override final;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
};