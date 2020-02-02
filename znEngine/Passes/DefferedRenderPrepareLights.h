#pragma once

#include "DefferedRender.h"

class CDefferedRenderPrepareLights
	: public RenderPass
{
public:
	struct SLightResult
	{
		bool IsEnabled;
		const ISceneNode3D* LightSceneNode;
		const ILightComponent3D* LightComponent;
		bool IsShadowEnable;
		std::shared_ptr<ITexture> ShadowTexture;
	};

public:
	CDefferedRenderPrepareLights(IRenderDevice& RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass);
	virtual ~CDefferedRenderPrepareLights();

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
	
	void BindPerFrameParamsForCurrentIteration(const ILightComponent3D* LightComponent);
	void BindPerObjectParamsForCurrentIteration(const ISceneNode3D* SceneNode);

private: // Pass light params
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectShaderParameter;

protected:
	__declspec(align(16)) struct PerFrame
	{
		glm::mat4 View;
		glm::mat4 Projection;
	};
	std::shared_ptr<IConstantBuffer> m_PerFrameConstantBuffer;
	IShaderParameter* m_PerFrameShaderParameter;

private: // For shadow rendering
	std::shared_ptr<IPipelineState> m_ShadowPipeline;
	std::shared_ptr<IRenderTarget> m_ShadowRenderTarget;
	Viewport m_ShadowViewport;
	std::vector<SLightResult> m_LightResult;

private:
	std::shared_ptr<BuildRenderListPass> m_BuildRenderListPass;

private: // consts
	const float cShadowTextureSize = 4096.0f; //16384.0f;
};