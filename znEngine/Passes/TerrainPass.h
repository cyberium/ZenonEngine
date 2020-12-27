#pragma once

class ZN_API CTerrainPass
	: public Base3DPass
{
public:
	CTerrainPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CTerrainPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override final;

	// IVisitor
	EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) override final;
	EVisitResult Visit(const std::shared_ptr<IModel>& Model) override final;
	EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
	EVisitResult Visit(const std::shared_ptr<ITerrain>& Terrain) override final;

private:
	std::shared_ptr<IMaterial> m_Material;
};