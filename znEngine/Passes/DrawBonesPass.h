#pragma once

class ZN_API CDrawBonesPass
	: public Base3DPass
{
public:
	CDrawBonesPass(IScene& scene);
	virtual ~CDrawBonesPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	EVisitResult Visit(const ISceneNode * CSceneNode) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	std::shared_ptr<IGeometry> m_ConeGeometry;
	std::shared_ptr<IGeometry> m_SphereGeometry;
	std::shared_ptr<IMaterial> m_Material;
};