#pragma once

class ZN_API CMaterialParticlePass 
	: public Base3DPass
{
public:
	CMaterialParticlePass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CMaterialParticlePass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override final;

	// IVisitor
	EVisitResult Visit(const ISceneNode3D* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	EVisitResult Visit(const IParticleSystem* ParticlesSystem) override;

private:
	IShaderParameter*                  m_GeomShaderPerObjectParameter;

	std::shared_ptr<IStructuredBuffer> m_GeomParticlesBuffer;
	IShaderParameter*                  m_GeomShaderParticlesBufferParameter;
	
private:
	std::shared_ptr<IGeometry>         m_Geometry;
};