#pragma once

class CDefferedRender
	: public RenderListProcessorPass
{
public:
	CDefferedRender(IRenderDevice* RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass);
	virtual ~CDefferedRender();

	// CDefferedRender
	ITexture* GetTexture0() const;
	ITexture* GetTexture1() const;
	ITexture* GetTexture2() const;
	ITexture* GetTexture3() const;
	ITexture* GetTextureDepthStencil() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;


	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(IRenderTarget* RenderTarget, const Viewport* Viewport) override;
	void UpdateViewport(const Viewport * _viewport) override;

	// IVisitor
	bool Visit3D(ISceneNode3D* node) override;
	bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	bool Visit(ILightComponent3D* light) override;

private:
	PerObject3D* m_PerObjectData;
	IConstantBuffer* m_PerObjectConstantBuffer;

private:
	ITexture* m_Texture0;
	ITexture* m_Texture1;
	ITexture* m_Texture2;
	ITexture* m_Texture3;
	ITexture* m_DepthStencilTexture;
};