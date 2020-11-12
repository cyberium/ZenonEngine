#pragma once

#include "M2/M2_Base_Instance.h"
#include "M2/RenderPass_M2.h"

class ZN_API CRenderPass_M2_Instanced
	: public CRenderPass_M2
{
public:
	CRenderPass_M2_Instanced(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypedListsPass, bool OpaqueDraw);
	virtual ~CRenderPass_M2_Instanced();

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

private:
	IShaderParameter*                  m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
};
