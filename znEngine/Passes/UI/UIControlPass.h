#pragma once

class ZN_API CUIControlPass
	: public BaseUIPass
{
public:
	CUIControlPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CUIControlPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	virtual EVisitResult Visit(const IUIControl* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;
private:

};