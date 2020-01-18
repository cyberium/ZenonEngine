#pragma once

#include "DefferedRender.h"

class CDefferedRenderFinal
	: public RenderPassPipelined
{
public:
	CDefferedRenderFinal(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<CDefferedRender> DefferedRender, std::shared_ptr<BuildRenderListPass> BuildRenderListPass);
	virtual ~CDefferedRenderFinal();

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

	// IRenderPassPipelined
	void CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

private:
	__declspec(align(16)) struct SScreenToViewParams
	{
		glm::mat4 InverseProjection;
		glm::vec2 ScreenDimensions;
	};
	SScreenToViewParams* m_ScreenToViewData;
	std::shared_ptr<IConstantBuffer> m_ScreenToViewConstantBuffer;

	SLight* m_LightData;
	std::shared_ptr<IConstantBuffer> m_LightConstantBuffer;

	std::shared_ptr<CDefferedRender> m_DefferedRender;
	std::shared_ptr<BuildRenderListPass> m_BuildRenderListPass;

	std::shared_ptr<IMesh> m_QuadMesh;
};