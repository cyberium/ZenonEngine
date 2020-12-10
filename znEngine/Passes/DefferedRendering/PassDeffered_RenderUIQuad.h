#pragma once

#include "PassDeffered_MergeShadowMaps.h"

class ZN_API CPassDeffered_RenderUIQuad
	: public RenderPassPipelined
{
public:
	CPassDeffered_RenderUIQuad(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass, std::shared_ptr<IRenderTarget> GBufferRenderTarget, std::shared_ptr<ITexture> MergedShadowTexture);
	virtual ~CPassDeffered_RenderUIQuad();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;


protected:
	void FillLightParamsForCurrentIteration(const RenderEventArgs& e, const IRenderPassCreateTypelessList::SLightElement& LightElement);


private: 
	// GPUDefferedLightVS
	std::shared_ptr<IConstantBuffer> m_GPUDefferedLightVSBuffer;
	IShaderParameter* m_GPUDefferedLightVSParameter;

	// ShadowMapTexture
	IShaderParameter* m_ShadowMapTextureParameter;

private:
	std::shared_ptr<IRenderPassCreateTypelessList> m_SceneCreateTypelessListPass;
	std::shared_ptr<IRenderTarget> m_GBufferRenderTarget;
	std::shared_ptr<ITexture> m_MergedShadowTexture;

	std::shared_ptr<IGeometry> m_QuadGeometry;
};