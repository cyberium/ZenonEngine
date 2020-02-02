#pragma once

class CUIFontPass
	: public BaseUIPass
{
public:
	CUIFontPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CUIFontPass();

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	virtual bool Visit(ISceneNodeUI* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;

private:

};