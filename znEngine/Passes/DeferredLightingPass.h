#pragma once

#include "AbstractPass.h"
#include "../SceneFunctional/Light.h"

#include "../SceneFunctional/3D/SceneNode3D.h"

// Use this pass to render the lights as geometry in the scene.
class DeferredLightingPass : public AbstractPass
{
	typedef AbstractPass base;
public:

	DeferredLightingPass(
		std::shared_ptr<IScene> scene,
		std::shared_ptr<IPipelineState> lightPipeline0,
		std::shared_ptr<IPipelineState> lightPipeline1,
		std::shared_ptr<IPipelineState> directionalLightPipeline,
		std::shared_ptr<ITexture> positionTexture,
		std::shared_ptr<ITexture> diffuseTexture,
		std::shared_ptr<ITexture> specularTexture,
		std::shared_ptr<ITexture> normalTexture,
		std::shared_ptr<ITexture> depthTexture
	);

	virtual ~DeferredLightingPass();

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void PreRender(RenderEventArgs& e) override final;
	virtual void Render(RenderEventArgs& e) override final;
	virtual void PostRender(RenderEventArgs& e) override final;

	// Inherited from Visitor
	virtual bool Visit(SceneNode3D* node) override final;
	virtual bool Visit(IMesh* Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override final;
	virtual bool Visit(std::shared_ptr<CLight3D> light) override final;

	virtual void UpdateViewport(const Viewport * _viewport);

	void UpdateFog(float fogModifier, vec3 fogColor, float fogDistance);

protected:
	void RenderSubPass(RenderEventArgs* e, std::shared_ptr<IScene> scene, std::shared_ptr<IPipelineState> pipeline);

private:
	mat4 m_World;

	__declspec(align(16)) struct ScreenToViewParams
	{
		glm::mat4x4 m_InverseProjectionMatrix;
		glm::vec2 m_ScreenDimensions;
		glm::vec2 Padding;
		glm::vec4 m_CameraPos;
	};
	ScreenToViewParams* m_pScreenToViewParams;
	std::shared_ptr<IConstantBuffer> m_ScreenToViewParamsCB;

	__declspec(align(16)) struct LightParams
	{
		Light m_Light;
	};
	LightParams* m_pLightParams;
	std::shared_ptr<IConstantBuffer> m_LightParamsCB;

	__declspec(align(16)) struct FogParams
	{
		float FogModifier;
		vec3  FogColor;
		float FogDistance;
		vec3  Padding;
	};
	FogParams* m_pFogParams;
	std::shared_ptr<IConstantBuffer> m_FogParamsCB;

	// First pipeline to mark lit pixels.
	std::shared_ptr<IPipelineState> m_LightPipeline0;
	// Second pipeline to render lit pixels.
	std::shared_ptr<IPipelineState> m_LightPipeline1;
	// Pipeline for directional lights
	std::shared_ptr<IPipelineState> m_DirectionalLightPipeline;
	
	std::shared_ptr<IScene> m_Scene;
	std::shared_ptr<IScene> m_pPointLightScene;
	std::shared_ptr<IScene> m_pSpotLightScene;
	std::shared_ptr<IScene> m_pDirectionalLightScene;

	// Textures
	std::shared_ptr<ITexture> m_PositionTexture;
	std::shared_ptr<ITexture> m_DiffuseTexture;
	std::shared_ptr<ITexture> m_SpecularTexture;
	std::shared_ptr<ITexture> m_NormalTexture;
	std::shared_ptr<ITexture> m_DepthTexture;
};