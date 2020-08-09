#pragma once

class ZN_API CUIButtonPass
	: public BaseUIPass
{
public:
	CUIButtonPass(IRenderDevice& RenderDevice, std::weak_ptr<IScene> Scene);
	virtual ~CUIButtonPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	virtual EVisitResult Visit(const ISceneNodeUI* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;
private:

};