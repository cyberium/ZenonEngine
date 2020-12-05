#pragma once

#include "PassDeffered_DoRenderScene.h"
#include "PassDeffered_ShadowMaps.h"

class ZN_API CPassDeffered_RenderUIQuad
	: public RenderPassPipelined
{
public:
	CPassDeffered_RenderUIQuad(IRenderDevice& RenderDevice, std::shared_ptr<CPassDeffered_DoRenderScene> DefferedRender, std::shared_ptr<CPassDeffered_ShadowMaps> DefferedRenderPrepareLights);
	virtual ~CPassDeffered_RenderUIQuad();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;


protected:
	void FillLightParamsForCurrentIteration(const RenderEventArgs& e, const CPassDeffered_ShadowMaps::SLightResult& LightResult);


private: 
	// GPUDefferedLightVS
	std::shared_ptr<IConstantBuffer> m_GPUDefferedLightVSBuffer;
	IShaderParameter* m_GPUDefferedLightVSParameter;

	// ShadowMapTexture
	IShaderParameter* m_ShadowMapTextureParameter;

private:
	std::shared_ptr<CPassDeffered_DoRenderScene> m_DefferedRender;
	std::shared_ptr<CPassDeffered_ShadowMaps> m_Deffered_Lights;

	std::shared_ptr<IGeometry> m_QuadGeometry;
};