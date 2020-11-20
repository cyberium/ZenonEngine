#pragma once

class CDebugPass 
	: public Base3DPass
{
public:
	CDebugPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CDebugPass();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override final;

	// IVisitor
	EVisitResult Visit(const ISceneNode* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override final;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override final;
};