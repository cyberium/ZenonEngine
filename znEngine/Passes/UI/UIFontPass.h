#pragma once

class ZN_API CUIFontPass
	: public BaseUIPass
{
public:
	CUIFontPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CUIFontPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	virtual EVisitResult Visit(const std::shared_ptr<IUIControl>& node) override;
	virtual EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;
};