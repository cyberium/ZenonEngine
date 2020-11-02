#pragma once

#include "../Scene/SceneCreateTypelessListPass.h"

class CPassForward_DoRenderSceneInstanced
	: public RenderPassPipelined
{
public:
	CPassForward_DoRenderSceneInstanced(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypelessListPass>& SceneCreateTypelessListPass);
	virtual ~CPassForward_DoRenderSceneInstanced();

	IShaderParameter* GetLightsShaderParameter() const;

	// IRenderPass
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;

	// IVisitor
	EVisitResult Visit(const ISceneNode* SceneNode) override;
	EVisitResult Visit(const IModel* Model) override;
	EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;

private:
	IShaderParameter* m_ShaderBonesBufferParameter;
	IShaderParameter* m_ShaderLightsBufferParameter;


	IShaderParameter* m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
	size_t							   m_InstancesCnt;

	std::shared_ptr<CSceneCreateTypelessListPass> m_SceneCreateTypelessListPass;
};