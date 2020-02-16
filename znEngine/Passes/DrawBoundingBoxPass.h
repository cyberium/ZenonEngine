#pragma once

class ZN_API CDrawBoundingBoxPass
	: public Base3DPass
{
public:
	CDrawBoundingBoxPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> scene);
	virtual ~CDrawBoundingBoxPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	bool Visit(const ISceneNode3D * SceneNode3D) override;
	bool Visit(const IModel* Model) override;
	bool Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	std::shared_ptr<IGeometry> m_QuadGeometry;
};