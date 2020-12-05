#pragma once

#include "../Scene/SceneCreateTypelessListPass.h"

class ZN_API CPassDeffered_ProcessLights
	: public RenderPass
{
public:
	struct SLightResult
	{
		bool IsEnabled;
		std::shared_ptr<const ISceneNode> SceneNode;
		std::shared_ptr<const ILight3D> LightNode;
		bool                       IsLightEnabled;
		bool                       IsCastShadow;
		std::shared_ptr<ITexture>  ShadowTexture;
	};

public:
	CPassDeffered_ProcessLights(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& BuildRenderListPass);
	virtual ~CPassDeffered_ProcessLights();

	void CreateShadowPipeline();
	const std::vector<SLightResult>& GetLightResult() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

protected:
	std::shared_ptr<IRenderTarget> CreateShadowRT();
	std::shared_ptr<ITexture> CreateShadowTexture0() const;
	std::shared_ptr<ITexture> CreateShadowTextureDepthStencil() const;
	
	void BindPerFrameParamsForCurrentIteration(const std::shared_ptr<const ILight3D>& Light);
	void BindPerObjectParamsForCurrentIteration(const std::shared_ptr<const ISceneNode>& SceneNode);

private:
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectShaderParameter;

	std::shared_ptr<IConstantBuffer> m_PerFrameConstantBuffer;
	IShaderParameter* m_PerFrameShaderParameter;

	IShaderParameter* m_ShaderBonesBufferParameter;

private: // For shadow rendering
	std::shared_ptr<IPipelineState> m_ShadowPipeline;
	std::shared_ptr<IRenderTarget> m_ShadowRenderTarget;

private: // Result
	std::vector<SLightResult> m_LightResult;

private:
	std::shared_ptr<IRenderPassCreateTypelessList> m_SceneCreateTypelessListPass;

private: // consts
	const float cShadowTextureSize = 4096.0f;
};