#pragma once

class ZN_API CUIControlConsolePass
	: public RenderPassPipelined
{
public:
	CUIControlConsolePass(IRenderDevice& RenderDevice);
	virtual ~CUIControlConsolePass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

};