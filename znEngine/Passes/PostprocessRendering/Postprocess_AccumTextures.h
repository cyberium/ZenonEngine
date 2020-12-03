#pragma once

#include "PostprocessBase.h"

class ZN_API CPassPostprocess_AccumTextures
	: public RenderPassPipelined
{
public:
	CPassPostprocess_AccumTextures(IRenderDevice& RenderDevice, std::vector<std::shared_ptr<ITexture>> InputTextures);
	virtual ~CPassPostprocess_AccumTextures();

	std::shared_ptr<ITexture> GetOutputTexture() const;

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

protected:
	std::shared_ptr<IRenderTarget> CreateRenderTarget(std::shared_ptr<IRenderTarget> RenderTarget);
	void SetOutputTexture(std::shared_ptr<ITexture> Texture);

private:
	std::vector<std::shared_ptr<ITexture>> m_InputTextures;
	std::shared_ptr<ITexture> m_OutputTexture;
	std::shared_ptr<IGeometry> m_QuadGeometry;
};