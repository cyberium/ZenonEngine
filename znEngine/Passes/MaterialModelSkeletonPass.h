#pragma once

class CMaterialModelSkeletonPass
	: public Base3DPass
	, public IMaterialModelPass
{
public:
	CMaterialModelSkeletonPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CMaterialModelSkeletonPass();

	IShaderParameter* GetLightsShaderParameter() const;
	IShaderParameter* GetBonesShaderParameter() const;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	EVisitResult Visit(const ISceneNode3D* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	IShaderParameter* m_ShaderLightsBufferParameter;
	IShaderParameter* m_ShaderBonesBufferParameter;
};