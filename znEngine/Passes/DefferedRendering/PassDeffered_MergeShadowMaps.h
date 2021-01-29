#pragma once

#include "PassDeffered_ShadowMaps.h"

class ZN_API CPassDeffered_MergeShadowMaps
	: public RenderPassPipelined
{
public:
	CPassDeffered_MergeShadowMaps(IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> GBufferRenderTarget, std::shared_ptr<CPassDeffered_ShadowMaps> DefferedRenderShadowMaps);
	virtual ~CPassDeffered_MergeShadowMaps();

	std::shared_ptr<ITexture> GetMergedShadowMapTexture() const;

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

protected:
	void BindShadowMapToShader(const RenderEventArgs& e, const SShadowMap& ShadowMap);

private: 
	std::shared_ptr<IGeometry> m_QuadGeometry;

	// ShadowMapBuffer
	std::shared_ptr<IConstantBuffer> m_ShadowMapBuffer;
	IShaderParameter* m_ShadowMapParameter;

	// ShadowMapTexture
	IShaderParameter* m_ShadowMapTextureParameter;

	// Merged shadows texture
	std::shared_ptr<ITexture> m_MergedShadowMapTexture;

private:
	std::shared_ptr<IRenderTarget> m_GBufferRenderTarget;
	std::shared_ptr<CPassDeffered_ShadowMaps> m_DefferedRenderShadowMaps;
};