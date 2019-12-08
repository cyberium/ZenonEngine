#pragma once

class CRenderPassDefaultCreator
	: public IRenderPassCreator
{
public:
	CRenderPassDefaultCreator();
	virtual ~CRenderPassDefaultCreator();

	// IRenderPassCreator
	size_t GetRenderPassCount() const;
	std::string GetRenderPassName(size_t Index) const;
	std::shared_ptr<IRenderPass> CreateRenderPass(size_t Index, std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const;

private:

};