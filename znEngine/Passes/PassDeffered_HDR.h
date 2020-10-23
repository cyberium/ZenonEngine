#pragma once

#include "PassDeffered_DoRenderScene.h"
#include "PassDeffered_ProcessLights.h"

class ZN_API CPassDeffered_HDR
	: public RenderPassPipelined
{
public:
	CPassDeffered_HDR(IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> HDRRenderTarget);
	virtual ~CPassDeffered_HDR();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

protected:
	void BindParamsForCurrentIteration(const RenderEventArgs& e);

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
	std::shared_ptr<IRenderTarget> m_HDRRenderTarget;
	std::shared_ptr<IGeometry> m_QuadGeometry;
};