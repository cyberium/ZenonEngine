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
		ITexture* ShadowTexture;
	};

public:
	CDefferedRenderPrepareLights(IRenderDevice* RenderDevice, std::shared_ptr<BuildRenderListPass> BuildRenderListPass);
	virtual ~CDefferedRenderPrepareLights();

	const std::vector<SLightResult>& GetLightResult() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> CreatePipeline(IRenderTarget* RenderTarget, const Viewport* Viewport);
	void UpdateViewport(const Viewport * _viewport);

protected:
	ITexture* CreateShadowTexture0() const;
	ITexture* CreateShadowTextureDepthStencil() const;
	
	void DoRenderToOneContext();
	void BindPerFrameParamsForCurrentIteration(const ILightComponent3D* LightComponent);
	void BindPerObjectParamsForCurrentIteration(const ISceneNode3D* SceneNode);

private: // Pass light params
	PerObject3D* m_PerObjectData;
	IConstantBuffer* m_PerObjectConstantBuffer;

protected:
	__declspec(align(16)) struct PerFrame
	{
		glm::mat4 View;
		glm::mat4 Projection;
	};
	void SetPerFrameData(const PerFrame& PerFrame);
	void BindPerFrameDataToVertexShader(const IShader* VertexShader) const;
	IConstantBuffer* m_PerFrameConstantBuffer;

private: // For shadow rendering
	IPipelineState* m_ShadowPipeline;
	IRenderTarget* m_ShadowRenderTarget;
	Viewport m_ShadowViewport;
	std::vector<SLightResult> m_LightResult;

private:
	std::shared_ptr<BuildRenderListPass> m_BuildRenderListPass;

private: // consts
	const float cShadowTextureSize = 4096.0f; //16384.0f;
};