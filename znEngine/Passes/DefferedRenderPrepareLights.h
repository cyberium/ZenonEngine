#pragma once

#include "DefferedRender.h"

class CDefferedRenderPrepareLights
	: public RenderPassPipelined
{
public:
	struct SLightResult
	{
		const ISceneNode* LightSceneNode;
		const ILightComponent3D* LightComponent;
		bool IsShadowEnable;
		std::shared_ptr<ITexture> ShadowTexture;
	};

public:
	CDefferedRenderPrepareLights(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass);
	virtual ~CDefferedRenderPrepareLights();

	const std::vector<SLightResult>& GetLightResult() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

	// IRenderPassPipelined
	void CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	void UpdateViewport(const Viewport * _viewport) override;

protected:
	std::shared_ptr<ITexture> CreateShadowTexture() const;
	void BindPerFrameParamsForCurrentIteration(const ILightComponent3D* LightComponent);
	void BindPerObjectParamsForCurrentIteration(const ISceneNode* SceneNode);

private: // Pass light params
	PerObject3D* m_PerObjectData;
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;

private: // For shadow rendering
	std::shared_ptr<IRenderTarget> m_ShadowRenderTarget;
	Viewport v;

	std::vector<SLightResult> m_LightResult;

private:
	std::shared_ptr<BuildRenderListPass> m_BuildRenderListPass;

private: // consts
	const float cShadowTextureSize = 2048.0f;
};