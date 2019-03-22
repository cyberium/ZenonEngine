#pragma once

#include "AbstractPass.h"
#include "3D/Scene3D.h"
#include "..//Light.h"

// Use this pass to render the lights as geometry in the scene.
class DeferredLightingPass : public AbstractPass
{
	typedef AbstractPass base;
public:

	DeferredLightingPass(
		std::shared_ptr<Scene3D> scene,
		std::shared_ptr<PipelineState> lightPipeline0,
		std::shared_ptr<PipelineState> lightPipeline1,
		std::shared_ptr<PipelineState> directionalLightPipeline,
		std::shared_ptr<Texture> positionTexture,
		std::shared_ptr<Texture> diffuseTexture,
		std::shared_ptr<Texture> specularTexture,
		std::shared_ptr<Texture> normalTexture,
		std::shared_ptr<Texture> depthTexture
	);

	virtual ~DeferredLightingPass();

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void PreRender(Render3DEventArgs& e) override final;
	virtual void Render(Render3DEventArgs& e) override final;
	virtual void PostRender(Render3DEventArgs& e) override final;

	// Inherited from Visitor
	virtual bool Visit(SceneNode3D& node) override final;
	virtual bool Visit(IMesh& mesh, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0) override final;
	virtual bool Visit(CLight3D& light) override final;

	virtual void UpdateViewport(Viewport _viewport);

	void UpdateFog(float fogModifier, vec3 fogColor, float fogDistance);

	void SetRenderEventArgs(Render3DEventArgs& e) override;
	Render3DEventArgs& GetRenderEventArgs() const override;

protected:
	void RenderSubPass(Render3DEventArgs& e, std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);

private:
	mat4 m_World;
	Render3DEventArgs* m_pRenderEventArgs;

	__declspec(align(16)) struct ScreenToViewParams
	{
		glm::mat4x4 m_InverseProjectionMatrix;
		glm::vec2 m_ScreenDimensions;
		glm::vec2 Padding;
		glm::vec4 m_CameraPos;
	};
	ScreenToViewParams* m_pScreenToViewParams;
	std::shared_ptr<ConstantBuffer> m_ScreenToViewParamsCB;

	__declspec(align(16)) struct LightParams
	{
		Light m_Light;
	};
	LightParams* m_pLightParams;
	std::shared_ptr<ConstantBuffer> m_LightParamsCB;

	__declspec(align(16)) struct FogParams
	{
		float FogModifier;
		vec3  FogColor;
		float FogDistance;
		vec3  Padding;
	};
	FogParams* m_pFogParams;
	std::shared_ptr<ConstantBuffer> m_FogParamsCB;

	// First pipeline to mark lit pixels.
	std::shared_ptr<PipelineState> m_LightPipeline0;
	// Second pipeline to render lit pixels.
	std::shared_ptr<PipelineState> m_LightPipeline1;
	// Pipeline for directional lights
	std::shared_ptr<PipelineState> m_DirectionalLightPipeline;
	
	std::shared_ptr<Scene3D> m_Scene;
	std::shared_ptr<Scene3D> m_pPointLightScene;
	std::shared_ptr<Scene3D> m_pSpotLightScene;
	std::shared_ptr<Scene3D> m_pDirectionalLightScene;

	// Textures
	std::shared_ptr<Texture> m_PositionTexture;
	std::shared_ptr<Texture> m_DiffuseTexture;
	std::shared_ptr<Texture> m_SpecularTexture;
	std::shared_ptr<Texture> m_NormalTexture;
	std::shared_ptr<Texture> m_DepthTexture;
};