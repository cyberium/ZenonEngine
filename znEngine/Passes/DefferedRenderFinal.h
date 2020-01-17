#pragma once

#include "DefferedRender.h"

class CDefferedRenderFinal
	: public AbstractPass
{
public:
	CDefferedRenderFinal(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<CDefferedRender> DefferedRender, std::shared_ptr<IRenderWindow> RenderWindow);
	virtual ~CDefferedRenderFinal();

	// IRenderPass
	void Render(RenderEventArgs& e);

private:
	void CreatePipeline();

private:
	std::shared_ptr<CDefferedRender> m_DefferedRender;
	std::shared_ptr<IRenderWindow> m_RenderWindow;

	std::shared_ptr<IPipelineState> m_DefferedFinalPipeline;

	std::shared_ptr<IMesh> m_QuadMesh;
};