#pragma once

class CMaterial_Textured_Pass 
	: public Base3DPass
{
public:
	CMaterial_Textured_Pass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CMaterial_Textured_Pass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override final;

	// IVisitor
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
};