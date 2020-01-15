#pragma once

class CShadowPass
	: public Base3DPass
{
public:
	CShadowPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CShadowPass();

	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit3D(ISceneNode* node) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(ILightComponent3D* light) override;

	std::shared_ptr<ITexture> GetColorTexture() const;
	std::shared_ptr<ITexture> GetShadowTexture() const;
	std::shared_ptr<IConstantBuffer> GetPerLightBuffer() const;

protected:
	void CreatePipeline();

private:
	__declspec(align(16)) struct PerLight
	{
		glm::mat4 LightView;
		glm::mat4 LightProjection;
	};
	PerLight*										m_PerLightData;
	std::shared_ptr<IConstantBuffer>                m_PerLightConstantBuffer;

private:
	std::shared_ptr<ITexture> m_ColorTexture;
	std::shared_ptr<ITexture> m_ShadowTexture;
	std::shared_ptr<IPipelineState> m_ShadowPipeline;
	Viewport v;
};