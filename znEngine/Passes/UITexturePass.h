#pragma once

class ZN_API CUITexturePass
	: public BaseUIPass
{
public:
	CUITexturePass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CUITexturePass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	virtual EVisitResult Visit(const ISceneNodeUI* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;
private:

};