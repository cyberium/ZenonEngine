#pragma once

#include "PassDeffered_DoRenderScene.h"
#include "PassDeffered_ProcessLights.h"

class ZN_API CPassDeffered_RenderUIQuad
	: public RenderPassPipelined
{
public:
	CPassDeffered_RenderUIQuad(IRenderDevice& RenderDevice, std::shared_ptr<CPassDeffered_DoRenderScene> DefferedRender, std::shared_ptr<CPassDeffered_ProcessLights> DefferedRenderPrepareLights);
	virtual ~CPassDeffered_RenderUIQuad();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;


protected:
	void BindLightParamsForCurrentIteration(const RenderEventArgs& e, const CPassDeffered_ProcessLights::SLightResult& LightResult);


private: // Pass light params
	std::shared_ptr<IConstantBuffer> m_LightResultConstantBuffer;


private:
	std::shared_ptr<CPassDeffered_DoRenderScene> m_DefferedRender;
	std::shared_ptr<CPassDeffered_ProcessLights> m_Deffered_Lights;

	std::shared_ptr<IGeometry> m_QuadGeometry;
};