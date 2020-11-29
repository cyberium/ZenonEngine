#pragma once

class CSkyboxPass
	: public RenderPassPipelined
{
public:
	CSkyboxPass(IRenderDevice& RenderDevice);
	virtual ~CSkyboxPass();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override final;

private:
	std::shared_ptr<IGeometry> m_SkyGeometry;

	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectShaderParameter;
};