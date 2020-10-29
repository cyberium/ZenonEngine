#pragma once

class ZN_API CDrawBoundingBoxPass
	: public Base3DPass
{
public:
	CDrawBoundingBoxPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> scene);
	virtual ~CDrawBoundingBoxPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	EVisitResult Visit(const ISceneNode3D * SceneNode3D) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	std::shared_ptr<IGeometry> m_BBoxGeometry;
};