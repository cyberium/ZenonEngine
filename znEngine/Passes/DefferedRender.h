#pragma once

class CDefferedRender
	: public RenderListProcessorPass
{
public:
	CDefferedRender(IRenderDevice& RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass);
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
	void UpdateViewport(const Viewport& _viewport) override;

	// IVisitor
	bool Visit(ISceneNode3D* node) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	bool Visit(ILightComponent3D* light) override;

private:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectShaderParameter;

private:
	std::shared_ptr<ITexture> m_Texture0;
	std::shared_ptr<ITexture> m_Texture1;
	std::shared_ptr<ITexture> m_Texture2;
	std::shared_ptr<ITexture> m_Texture3;
	std::shared_ptr<ITexture> m_DepthStencilTexture;
};