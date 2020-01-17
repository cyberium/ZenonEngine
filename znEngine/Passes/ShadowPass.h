#pragma once

class CShadowPass
	: public Base3DPass
{
public:
	CShadowPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CShadowPass();

	std::shared_ptr<ITexture> GetColorTexture() const;
	std::shared_ptr<ITexture> GetShadowTexture() const;
	std::shared_ptr<IConstantBuffer> GetPerLightBuffer() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;

	// IRenderPassPipelined
	void CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	void UpdateViewport(const Viewport * _viewport) override;

	// IVisitor
	virtual bool Visit3D(ISceneNode* node) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(ILightComponent3D* light) override;

protected:
	void FillPerFrameData() override;

private:
	std::shared_ptr<IRenderTarget> m_RenderTarget;
	std::shared_ptr<ITexture> m_ColorTexture;
	std::shared_ptr<ITexture> m_ShadowTexture;
	Viewport v;
};