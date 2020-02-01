#pragma once

#include "BlendStateDX11.h"
#include "RasterizerStateDX11.h"
#include "DepthStencilStateDX11.h"

class ZN_API PipelineStateDX11 
	: public PipelineStateBase
{
public:
	PipelineStateDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual ~PipelineStateDX11();

	// IPipelineState
	void Bind() override final;
	void UnBind() override final;

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};