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
	void PostRender(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

protected:
	void BindLightParamsForCurrentIteration(const RenderEventArgs& e, const CPassDeffered_ProcessLights::SLightResult& LightResult);

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
	std::shared_ptr<CPassDeffered_DoRenderScene> m_DefferedRender;
	std::shared_ptr<CPassDeffered_ProcessLights> m_DefferedRenderPrepareLights;

	std::shared_ptr<IGeometry> m_QuadGeometry;
};