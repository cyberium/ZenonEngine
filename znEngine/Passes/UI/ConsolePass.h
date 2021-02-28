#pragma once

#include "UIControls/Common/UICommonModel.h"
#include "UIControls/Text/UITextModel.h"

class ZN_API CUIConsolePass
	: public RenderPassMultipipelined
{
public:
	CUIConsolePass(IRenderDevice& RenderDevice);
	virtual ~CUIConsolePass();

	// IRenderPass
	void Render(size_t PipelineIndex, IPipelineState& Pipeline, RenderEventArgs & e) override;

	// IRenderPassPipelined
	virtual void Configure(std::shared_ptr<IRenderTarget> RenderTarget) override;

protected:
	PerFrame GetPerFrameData(IPipelineState& Pipeline) const override final;

private:
	size_t m_ColorPipelineIndex;
	size_t m_FontPipelineIndex;

	std::shared_ptr<CUICommonModel> m_ColorModel;
	std::shared_ptr<CUITextModel> m_FontModel;

	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	//IShaderParameter* m_PerObjectParameter;

};
