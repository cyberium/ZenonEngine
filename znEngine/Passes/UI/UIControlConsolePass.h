#pragma once

#include "Materials/UI_Font_Material.h"

class ZN_API CUIControlConsolePass
	: public RenderPassPipelined
{
public:
	CUIControlConsolePass(IRenderDevice& RenderDevice);
	virtual ~CUIControlConsolePass();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

protected:
	PerFrame GetPerFrameData() const override final;

private:
	std::string m_Text;
	std::shared_ptr<IznFont> m_Font;
	std::shared_ptr<UI_Font_Material> m_Material;
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectParameter;
};