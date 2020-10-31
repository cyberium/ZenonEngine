#pragma once

#include "Scene/SceneCreateTypelessListPass.h"

class ZN_API CRTSGround_Pass
	: public Base3DPass
{
public:
	CRTSGround_Pass(IRenderDevice& RenderDevice, const std::weak_ptr<IScene>& scene);
	virtual ~CRTSGround_Pass();

	IShaderParameter* GetLightsShaderParameter() const;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	EVisitResult Visit(const ISceneNode3D* SceneNode) override;

private:
	IShaderParameter* m_ShaderLightsBufferParameter;

	IShaderParameter*                  m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
	size_t							   m_InstancesCnt;
};