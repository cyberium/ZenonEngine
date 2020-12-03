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
	EVisitResult Visit(const std::shared_ptr<ISceneNode>& SceneNode) override;
	EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
	EVisitResult Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	IShaderParameter* m_ShaderBonesBufferParameter;
	IShaderParameter* m_ShaderLightsBufferParameter;
};