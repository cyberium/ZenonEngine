#pragma once

class ZN_API CUIColorPass
	: public BaseUIPass
{
public:
	CUIColorPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CUIColorPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	virtual EVisitResult Visit(const IUIControl* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;
};