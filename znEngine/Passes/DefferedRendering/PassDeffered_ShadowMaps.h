#pragma once

#include "../Scene/SceneCreateTypelessListPass.h"

class ZN_API CPassDeffered_ShadowMaps
	: public RenderPass
{
public:
	struct SShadowMap
	{
		std::shared_ptr<const ISceneNode>  SceneNode;
		std::shared_ptr<const ILight>      LightNode;
		std::shared_ptr<ITexture>          ShadowTexture;
	};

public:
	CPassDeffered_ShadowMaps(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& BuildRenderListPass);
	virtual ~CPassDeffered_ShadowMaps();

	void CreateShadowPipeline();
	const std::vector<SShadowMap>& GetShadowMaps() const;

	// IRenderPass
	void PreRender(RenderEventArgs& e) override;
	void Render(RenderEventArgs& e) override;
	void PostRender(RenderEventArgs& e) override;

protected:
	std::shared_ptr<IRenderTarget> CreateShadowRT();
	std::shared_ptr<ITexture> CreateShadowTexture0() const;
	std::shared_ptr<ITexture> CreateShadowTextureDepthStencil() const;
	
	void RenderScene();

	void BindPerFrameParamsForCurrentIteration(const std::shared_ptr<const ILight>& Light);
	void BindPerObjectParamsForCurrentIteration(const std::shared_ptr<const ISceneNode>& SceneNode);

	std::shared_ptr<ITexture> GetShadowMapTexture();

private:
	std::shared_ptr<IPipelineState> m_ShadowPipeline;
	std::shared_ptr<IRenderTarget>  m_ShadowRenderTarget;

	// Per object
	std::shared_ptr<IConstantBuffer> m_PerObjectConstantBuffer;
	IShaderParameter* m_PerObjectShaderParameter;

	// Per frame
	std::shared_ptr<IConstantBuffer> m_PerFrameConstantBuffer;
	IShaderParameter* m_PerFrameShaderParameter;

	// Per object bones
	IShaderParameter* m_ShaderBonesBufferParameter;

private:
	std::vector<SShadowMap> m_ShadowMaps;

	mutable size_t m_CurrentShadowMapTexture;
	std::vector <std::shared_ptr<ITexture>> m_ShadowMapsTexturesCache;

private:
	std::shared_ptr<IRenderPassCreateTypelessList> m_SceneCreateTypelessListPass;

private: // consts
	const size_t cShadowTextureSize = 4096;
};