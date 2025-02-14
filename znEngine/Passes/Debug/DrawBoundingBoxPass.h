#pragma once

class ZN_API CDrawBoundingBoxPass
	: public Base3DPass
{
public:
	CDrawBoundingBoxPass(IRenderDevice& RenderDevice, IScene& scene);
	virtual ~CDrawBoundingBoxPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	EVisitResult Visit(const std::shared_ptr<ISceneNode>& CSceneNode) override;
	EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
	EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	std::shared_ptr<IGeometry> m_BBoxGeometry;
	std::shared_ptr<IMaterial> m_Material;
};