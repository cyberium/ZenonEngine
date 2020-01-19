#pragma once

class CDefferedRender
	: public RenderListProcessorPass
{
public:
	CDefferedRender(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass);
	virtual ~CDefferedRender();

	// CDefferedRender
	std::shared_ptr<ITexture> GetTexture0() const;
	std::shared_ptr<ITexture> GetTexture1() const;
	std::shared_ptr<ITexture> GetTexture2() const;
	std::shared_ptr<ITexture> GetTexture3() const;
	std::shared_ptr<ITexture> GetTextureDepthStencil() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;


	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	void UpdateViewport(const Viewport * _viewport) override;

	// IVisitor
	bool Visit3D(ISceneNode* node) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	bool Visit(ILightComponent3D* light) override;

private:
	PerObject3D* m_PerObjectData;
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;

private:
	std::shared_ptr<ITexture> m_Texture0;
	std::shared_ptr<ITexture> m_Texture1;
	std::shared_ptr<ITexture> m_Texture2;
	std::shared_ptr<ITexture> m_Texture3;
	std::shared_ptr<ITexture> m_DepthStencilTexture;
};