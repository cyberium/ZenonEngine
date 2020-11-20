#pragma once

#if 0 

class ZN_API CUIControlConsolePass
	: public RenderPassPipelined
{
public:
	CUIControlConsolePass(IRenderDevice& RenderDevice);
	virtual ~CUIControlConsolePass();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

protected:
	PerFrame GetPerFrameData() const override final;

private:
	std::string m_Text;
	std::shared_ptr<IznFont> m_Font;
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectParameter;

private:
	std::shared_ptr<IConstantBuffer>   m_FontBuffer;
	IShaderParameter*                  m_FontBufferParameter;
};

#endif
