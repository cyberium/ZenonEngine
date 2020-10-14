#pragma once

#include "Scene/SceneCreateTypelessListPass.h"

class ZN_API CPassDeffered_ProcessLights
	: public RenderPass
{
public:
	struct SLightResult
	{
		bool IsEnabled;
		const ISceneNode3D* SceneNode;
		const ILight3D* Light;
		bool IsShadowEnable;
		std::shared_ptr<ITexture> ShadowTexture;
	};

public:
	CPassDeffered_ProcessLights(IRenderDevice& RenderDevice, const std::shared_ptr<CSceneCreateTypelessListPass>& BuildRenderListPass);
	virtual ~CPassDeffered_ProcessLights();

	const std::vector<SLightResult>& GetLightResult() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport);
	void UpdateViewport(const Viewport* _viewport);

protected:
	std::shared_ptr<ITexture> CreateShadowTexture0() const;
	std::shared_ptr<ITexture> CreateShadowTextureDepthStencil() const;
	
	void BindPerFrameParamsForCurrentIteration(const ILight3D * Light);
	void BindPerObjectParamsForCurrentIteration(const ISceneNode3D* SceneNode);

private: // Pass light params
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectShaderParameter;

protected:
	std::shared_ptr<IConstantBuffer> m_PerFrameConstantBuffer;
	IShaderParameter* m_PerFrameShaderParameter;

private: // For shadow rendering
	std::shared_ptr<IPipelineState> m_ShadowPipeline;
	std::shared_ptr<IRenderTarget> m_ShadowRenderTarget;
	Viewport m_ShadowViewport;
	std::vector<SLightResult> m_LightResult;

private:
	std::shared_ptr<CSceneCreateTypelessListPass> m_SceneCreateTypelessListPass;

private: // consts
	const float cShadowTextureSize = 4096.0f;
};