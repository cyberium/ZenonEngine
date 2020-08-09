#pragma once

#include "DefferedRender.h"
#include "DefferedRenderPrepareLights.h"

class ZN_API CDefferedRenderFinal
	: public RenderPassPipelined
{
public:
	CDefferedRenderFinal(IRenderDevice& RenderDevice, std::shared_ptr<CDefferedRender> DefferedRender, std::shared_ptr<CDefferedRenderPrepareLights> DefferedRenderPrepareLights);
	virtual ~CDefferedRenderFinal();

	// IRenderPass
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

protected:
	void BindLightParamsForCurrentIteration(const RenderEventArgs& e, const CDefferedRenderPrepareLights::SLightResult& LightResult);

private: // Pass light params
	struct __declspec(novtable, align(16)) SLightResult
	{
		SLight Light;
		glm::mat4 LightViewMatrix;
		glm::mat4 LightProjectionMatrix;
		uint32 IsShadowEnabled;
		// 12 bytes padding
	};
	SLightResult* m_LightResultData;
	std::shared_ptr<IConstantBuffer> m_LightResultConstantBuffer;

private:
	std::shared_ptr<CDefferedRender> m_DefferedRender;
	std::shared_ptr<CDefferedRenderPrepareLights> m_DefferedRenderPrepareLights;

	std::shared_ptr<IGeometry> m_QuadGeometry;
};