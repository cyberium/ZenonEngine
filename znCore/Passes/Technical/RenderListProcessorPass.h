#pragma once

#include "../RenderPassPipelined.h"
#include "BuildRenderListPass.h"

class ZN_API RenderListProcessorPass
	: public RenderPassPipelined
{
public:
	RenderListProcessorPass(IRenderDevice& RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass);
	virtual ~RenderListProcessorPass();

	// IRenderPass
	virtual void Render(RenderEventArgs& e) override;

private:
	std::shared_ptr<BuildRenderListPass> m_BuildRenderListPass;
};
