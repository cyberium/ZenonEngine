#pragma once

#include "DefferedRender.h"

class CDefferedRenderFinal
	: public RenderPassPipelined
{
public:
	CDefferedRenderFinal(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<CDefferedRender> DefferedRender);
	virtual ~CDefferedRenderFinal();

	// IRenderPass
	void Render(RenderEventArgs& e);

	// IRenderPassPipelined
	void CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

private:
	std::shared_ptr<CDefferedRender> m_DefferedRender;

	std::shared_ptr<IMesh> m_QuadMesh;
};