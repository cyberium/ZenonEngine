#pragma once

class ZN_API CParticlesPass 
	: public Base3DPass
{
public:
	CParticlesPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CParticlesPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override final;

	// IVisitor
	EVisitResult Visit(const ISceneNode* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	EVisitResult Visit(const IParticleSystem* ParticlesSystem) override;

private:
	std::shared_ptr<IStructuredBuffer> m_GeomParticlesBuffer;
	IShaderParameter*                  m_GeomShaderParticlesBufferParameter;
	
private:
	std::shared_ptr<IGeometry>         m_Geometry;
};