#pragma once

class CDrawToolsPass
	: public Base3DPass
{
public:
	CDrawToolsPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CDrawToolsPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override final;

	// IVisitor
	EVisitResult Visit(const std::shared_ptr<IModel>& Model) override final;
	EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
};