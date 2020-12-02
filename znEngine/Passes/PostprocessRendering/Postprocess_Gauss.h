#pragma once

#include "PostprocessBase.h"

class ZN_API CPassPostprocess_Gauss
	: public CPassPostprocessBase
{
public:
	CPassPostprocess_Gauss(IRenderDevice& RenderDevice, std::shared_ptr<ITexture> InputTexture, bool IsHorizontal);
	virtual ~CPassPostprocess_Gauss();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

protected:
	std::shared_ptr<IRenderTarget> LoadRenderTarget(std::shared_ptr<IRenderTarget> OriginalRenderTarget) override;
	std::shared_ptr<IShader> LoadVertexShader() override;
	std::shared_ptr<IShader> LoadPixelShader() override;

private:
	bool m_IsHorizontal;

	std::shared_ptr<IConstantBuffer> m_GaussConstantBuffer;
	IShaderParameter* m_GaussShaderParameter;
};