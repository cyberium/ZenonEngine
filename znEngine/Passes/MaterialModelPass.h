#pragma once

class CMaterialModelPass
	: public Base3DPass
	, public IMaterialModelPass
{
public:
	CMaterialModelPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CMaterialModelPass();

	IShaderParameter* GetLightsShaderParameter() const;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	IShaderParameter* m_ShaderLightsBufferParameter;
};