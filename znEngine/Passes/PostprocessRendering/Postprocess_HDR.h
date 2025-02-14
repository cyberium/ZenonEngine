#pragma once

#include "PostprocessBase.h"

class ZN_API CPassPostprocess_HDR
	: public CPassPostprocessBase
{
public:
	CPassPostprocess_HDR(IRenderDevice& RenderDevice, std::shared_ptr<ITexture> InputTexture);
	virtual ~CPassPostprocess_HDR();

protected:
	std::shared_ptr<IRenderTarget> LoadRenderTarget(std::shared_ptr<IRenderTarget> OriginalRenderTarget) override;
	std::shared_ptr<IShader> LoadVertexShader() override;
	std::shared_ptr<IShader> LoadPixelShader() override;
};