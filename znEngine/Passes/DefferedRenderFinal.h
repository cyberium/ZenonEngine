#pragma once

#include "DefferedRender.h"
#include "DefferedRenderPrepareLights.h"

class CDefferedRenderFinal
	: public RenderPassPipelined
{
public:
	CDefferedRenderFinal(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<CDefferedRender> DefferedRender, std::shared_ptr<CDefferedRenderPrepareLights> DefferedRenderPrepareLights);
	virtual ~CDefferedRenderFinal();

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

	// IRenderPassPipelined
	void CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

protected:
	void BindLightParamsForCurrentIteration(const RenderEventArgs& e, const CDefferedRenderPrepareLights::SLightResult& LightResult);

private: // some every frame data
	__declspec(align(16)) struct SScreenToViewParams
	{
		glm::mat4 InverseProjection;
		glm::vec2 ScreenDimensions;
	};
	SScreenToViewParams* m_ScreenToViewData;
	std::shared_ptr<IConstantBuffer> m_ScreenToViewConstantBuffer;

private: // Pass light params
	struct __declspec(novtable, align(16)) SLightResult
	{
		SLight Light;

		glm::mat4 LightViewMatrix;

		glm::mat4 LightProjectionMatrix;

		uint32 IsShadowEnabled;
		glm::vec3 __Padding;
	};
	SLightResult* m_LightResultData;
	std::shared_ptr<IConstantBuffer> m_LightResultConstantBuffer;

private:
	std::shared_ptr<CDefferedRender> m_DefferedRender;
	std::shared_ptr<CDefferedRenderPrepareLights> m_DefferedRenderPrepareLights;

	std::shared_ptr<IMesh> m_QuadMesh;
};