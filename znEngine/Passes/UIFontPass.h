#pragma once

class ZN_API CUIFontPass
	: public BaseUIPass
{
public:
	CUIFontPass(IRenderDevice& RenderDevice, std::weak_ptr<IScene> Scene);
	virtual ~CUIFontPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	virtual EVisitResult Visit(const ISceneNodeUI* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;

private:

};