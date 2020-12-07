#pragma once

class CPassPostprocess_RenderGlowSceneObjects
	: public Base3DPass
{
public:
	CPassPostprocess_RenderGlowSceneObjects(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CPassPostprocess_RenderGlowSceneObjects();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) override;
	EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
	EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	IShaderParameter* m_ShaderBonesBufferParameter;
};