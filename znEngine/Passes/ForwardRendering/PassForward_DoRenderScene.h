#pragma once

class CPassForward_DoRenderScene
	: public Base3DPass
{
public:
	CPassForward_DoRenderScene(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CPassForward_DoRenderScene();

	void SetEnviorementTexture(std::shared_ptr<ITexture> Texture);
	IShaderParameter* GetLightsShaderParameter() const;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	EVisitResult Visit(const ISceneNode* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	IShaderParameter* m_ShaderBonesBufferParameter;
	IShaderParameter* m_ShaderLightsBufferParameter;
};