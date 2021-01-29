#pragma once

#include "PassDeffered_ShadowMaps.h"
#include "PassDeffered_MergeShadowMaps.h"

struct __declspec(align(16)) SGPULightVSDeffered
{
	SGPULightVS  LightVS;
	// 64 bytes
	glm::mat4 LightViewMatrix;
	// 64 bytes
	glm::mat4 LightProjectionMatrix;
	// 64 bytes

};

class ZN_API CPassDeffered_RenderUIQuad
	: public RenderPassPipelined
{
public:
	CPassDeffered_RenderUIQuad(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass, std::shared_ptr<IRenderTarget> GBufferRenderTarget, std::shared_ptr<IPassDeffered_ShadowMaps> DefferedRenderShadowMaps);
	virtual ~CPassDeffered_RenderUIQuad();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;


protected:
	void FillLightParamsForCurrentIteration(const RenderEventArgs& e, const IRenderPassCreateTypelessList::SLightElement& LightElement);


private: 
	// GPUDefferedLightVS
	std::shared_ptr<IConstantBuffer> m_GPULightVSDefferedBuffer;
	IShaderParameter* m_GPUDefferedLightVSParameter;

	// ShadowMapTexture
	IShaderParameter* m_ShadowMapTextureParameter;

private:
	std::shared_ptr<IRenderPassCreateTypelessList> m_SceneCreateTypelessListPass;
	std::shared_ptr<IRenderTarget> m_GBufferRenderTarget;

	std::shared_ptr<IPassDeffered_ShadowMaps> m_ShadowMaps;
	std::shared_ptr<IGeometry> m_QuadGeometry;
};