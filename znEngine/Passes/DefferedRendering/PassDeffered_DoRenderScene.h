#pragma once

#include "../Scene/SceneCreateTypelessListPass.h"

class ZN_API CPassDeffered_DoRenderScene
	: public RenderPassPipelined
{
public:
	CPassDeffered_DoRenderScene(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass);
	virtual ~CPassDeffered_DoRenderScene();

	void SetEnviorementTexture(std::shared_ptr<ITexture> Texture);

	// CPassDeffered_DoRenderScene
	std::shared_ptr<IRenderTarget> GetGBufferRenderTarget() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	void DoRenderSceneNode(const std::shared_ptr<const ISceneNode>& SceneNode);
	void DoRenderGeometry(const std::shared_ptr<const IGeometry>& Geometry, const std::shared_ptr<const IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs);

private:
	std::shared_ptr<IRenderTarget> CreateGBuffer(std::shared_ptr<IRenderTarget> RenderTarget);

private:
	std::shared_ptr<IRenderPassCreateTypelessList> m_SceneCreateTypelessListPass;

private:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectShaderParameter;

	IShaderParameter* m_ShaderBonesBufferParameter;

private:
	std::shared_ptr<IRenderTarget> m_GBufferRenderTarget;
};