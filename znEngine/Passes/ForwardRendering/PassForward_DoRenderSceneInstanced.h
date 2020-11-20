#pragma once

#include "../Scene/SceneCreateTypelessListPass.h"

class CPassForward_DoRenderSceneInstanced
	: public RenderPassPipelined
{
public:
	CPassForward_DoRenderSceneInstanced(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass);
	virtual ~CPassForward_DoRenderSceneInstanced();

	IShaderParameter* GetLightsShaderParameter() const;

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

private:
	IShaderParameter* m_ShaderBonesBufferParameter;
	IShaderParameter* m_ShaderLightsBufferParameter;


	IShaderParameter* m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
	size_t							   m_InstancesCnt;

	std::shared_ptr<IRenderPassCreateTypelessList> m_SceneCreateTypelessListPass;
};