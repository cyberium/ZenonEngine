#include <stdafx.h>

#include <Application.h>
#include "3D//Scene3D.h"
#include "3D//SceneNodeModel3D.h"

// General
#include "DeferredLightingPass.h"

DeferredLightingPass::DeferredLightingPass(
	std::shared_ptr<Scene3D> scene,
	std::shared_ptr<PipelineState> lightPipeline0,
	std::shared_ptr<PipelineState> lightPipeline1,
	std::shared_ptr<PipelineState> directionalLightPipeline,
	std::shared_ptr<Texture> positionTexture,
	std::shared_ptr<Texture> diffuseTexture,
	std::shared_ptr<Texture> specularTexture,
	std::shared_ptr<Texture> normalTexture,
	std::shared_ptr<Texture> depthTexture
)
	: m_LightPipeline0(lightPipeline0)
	, m_LightPipeline1(lightPipeline1)
	, m_DirectionalLightPipeline(directionalLightPipeline)
	, m_Scene(scene)
	, m_PositionTexture(positionTexture)
	, m_DiffuseTexture(diffuseTexture)
	, m_SpecularTexture(specularTexture)
	, m_NormalTexture(normalTexture)
	, m_DepthTexture(depthTexture)
{
	m_pScreenToViewParams = (ScreenToViewParams*)_aligned_malloc(sizeof(ScreenToViewParams), 16);
	m_ScreenToViewParamsCB = _RenderDevice->CreateConstantBuffer(ScreenToViewParams());

	m_pLightParams = (LightParams*)_aligned_malloc(sizeof(LightParams), 16);
	m_LightParamsCB = _RenderDevice->CreateConstantBuffer(LightParams());

	m_pFogParams = (FogParams*)_aligned_malloc(sizeof(FogParams), 16);
	m_FogParamsCB = _RenderDevice->CreateConstantBuffer(FogParams());

	// PointLightScene
	m_pPointLightScene = std::make_shared<Scene3D>();

	std::shared_ptr<SceneNodeModel3D> sphereSceneNode = std::make_shared<SceneNodeModel3D>();
	sphereSceneNode->SetParent(m_pPointLightScene->GetRootNode());

	std::shared_ptr<IMesh> sphereMesh = _RenderDevice->CreateSphere();
	sphereSceneNode->AddMesh(sphereMesh);

	// Create a full-screen quad that is placed on the far clip plane.
	m_pDirectionalLightScene = std::make_shared<Scene3D>();

	std::shared_ptr<SceneNodeModel3D> quadSceneNode = std::make_shared<SceneNodeModel3D>();
	quadSceneNode->SetParent(m_pDirectionalLightScene->GetRootNode());

	std::shared_ptr<IMesh> quadMesh = _RenderDevice->CreateScreenQuad(0, 1280, 1024, 0); // _RenderDevice->CreateScreenQuad(-1, 1, -1, 1, -1);
	quadSceneNode->AddMesh(quadMesh);
}

DeferredLightingPass::~DeferredLightingPass()
{
	_aligned_free(m_pScreenToViewParams);
	_RenderDevice->DestroyConstantBuffer(m_ScreenToViewParamsCB);

	_aligned_free(m_pLightParams);
	_RenderDevice->DestroyConstantBuffer(m_LightParamsCB);

	_aligned_free(m_pFogParams);
	_RenderDevice->DestroyConstantBuffer(m_FogParamsCB);
}

void DeferredLightingPass::PreRender(Render3DEventArgs& e)
{
	e.PipelineState = nullptr;
	SetRenderEventArgs(e);

	// Bind the G-buffer textures to the pixel shader pipeline stage.
	m_PositionTexture->Bind(0, Shader::PixelShader, ShaderParameter::Type::Texture);
	m_DiffuseTexture->Bind(1, Shader::PixelShader, ShaderParameter::Type::Texture);
	m_SpecularTexture->Bind(2, Shader::PixelShader, ShaderParameter::Type::Texture);
	m_NormalTexture->Bind(3, Shader::PixelShader, ShaderParameter::Type::Texture);
	m_DepthTexture->Bind(4, Shader::PixelShader, ShaderParameter::Type::Texture);
}

void DeferredLightingPass::RenderSubPass(Render3DEventArgs& e, std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline)
{
	e.PipelineState = pipeline.get();
	SetRenderEventArgs(e);

	pipeline->Bind();

	scene->Accept(*this);

	pipeline->UnBind();
}

// Render the pass. This should only be called by the RenderTechnique.
void DeferredLightingPass::Render(Render3DEventArgs& e)
{
	const Camera* pCamera = e.Camera;
	assert1(pCamera != nullptr);

	Viewport viewport = pCamera->GetViewport();
	// We need the inverse projection matrix to compute the view space position of the fragment
	// in the deferred lighting shader.
	m_pScreenToViewParams->m_InverseProjectionMatrix = glm::inverse(pCamera->GetProjectionMatrix());
	m_pScreenToViewParams->m_ScreenDimensions = glm::vec2(viewport.Width, viewport.Height);
	m_pScreenToViewParams->m_CameraPos = vec4(pCamera->GetTranslation(), 1.0f);
	m_ScreenToViewParamsCB->Set(*m_pScreenToViewParams);

	// Connect shader parameters to shaders.
	// This ensures the right parameters are bound to the rendering pipeline when the shader is bound.
	// (This can probably be done once in the constructor, but if the shaders are recompiled, the parameters
	// need to be reconnected to the shaders.. soooo better safe than sorry I guess).
	std::vector< PipelineState* > pipelines = { m_LightPipeline0.get(), m_LightPipeline1.get(), m_DirectionalLightPipeline.get() };
	for (auto pipeline : pipelines)
	{
		std::shared_ptr<Shader> vertexShader = pipeline->GetShader(Shader::VertexShader);
		BindPerObjectConstantBuffer(vertexShader);

		std::shared_ptr<Shader> pixelShader = pipeline->GetShader(Shader::PixelShader);
		if (pixelShader)
		{
			// Bind the per-light & deferred lighting properties constant buffers to the pixel shader.
			pixelShader->GetShaderParameterByName("LightIndexBuffer").Set(m_LightParamsCB);
			pixelShader->GetShaderParameterByName("ScreenToViewParams").Set(m_ScreenToViewParamsCB);
			pixelShader->GetShaderParameterByName("FogParams").Set(m_FogParamsCB);
		}
	}

	/*m_pLightParams->m_LightIndex = 0;
	for (Light& light : m_Lights)
	{
		if (light.m_Enabled)
		{
			m_pCurrentLight = &light;

			// Update the constant buffer for the per-light data.
			m_LightParamsCB->Set(*m_pLightParams);

			// Clear the stencil buffer for the next light
			m_LightPipeline0->GetRenderTarget()->Clear(ClearFlags::Stencil, glm::vec4(0), 1.0f, 1);
			// The other pipelines should have the same render target.. so no need to clear it 3 times.

			switch (light.m_Type)
			{
			case Light::LightType::Point:
				RenderSubPass(e, m_pPointLightScene, m_LightPipeline0);
				RenderSubPass(e, m_pPointLightScene, m_LightPipeline1);
				break;
			case Light::LightType::Spot:
				RenderSubPass(e, m_pSpotLightScene, m_LightPipeline0);
				RenderSubPass(e, m_pSpotLightScene, m_LightPipeline1);
				break;
			case Light::LightType::Directional:
				RenderSubPass(e, m_pDirectionalLightScene, m_DirectionalLightPipeline);
				break;
			}
		}
		m_pLightParams->m_LightIndex++;
	}*/

	m_Scene->Accept(*this);
}

void DeferredLightingPass::PostRender(Render3DEventArgs& e)
{
	// Explicitly unbind these textures so they can be used as render target textures.
	m_PositionTexture->UnBind(0, Shader::PixelShader, ShaderParameter::Type::Texture);
	m_DiffuseTexture->UnBind(1, Shader::PixelShader, ShaderParameter::Type::Texture);
	m_SpecularTexture->UnBind(2, Shader::PixelShader, ShaderParameter::Type::Texture);
	m_NormalTexture->UnBind(3, Shader::PixelShader, ShaderParameter::Type::Texture);
	m_DepthTexture->UnBind(4, Shader::PixelShader, ShaderParameter::Type::Texture);
}

// Inherited from Visitor

bool DeferredLightingPass::Visit(SceneNode3D& node)
{
	m_World = node.GetWorldTransfom();

	return true;
}

bool DeferredLightingPass::Visit(IMesh& mesh, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	if (m_pRenderEventArgs && mesh.GetMaterial() == nullptr) // TODO: Fixme
	{
		return mesh.Render(*m_pRenderEventArgs, m_PerObjectConstantBuffer);
	}

	return false;
}

bool DeferredLightingPass::Visit(CLight3D& light)
{
	const Camera* camera = GetRenderEventArgs().Camera;
	assert1(camera != nullptr);

	PerObject perObjectData;

	//if (light.getLight().m_Type == Light::LightType::Directional)
	{
		perObjectData.ModelView = glm::mat4(1.0f);
		perObjectData.ModelViewProjection = glm::ortho<float>(0.0f, 1280.0f, 1024.0f, 0.0f, -1.0f, 1.0f);
	}
	/*else
	{
		// Setup constant buffer for node.
		// Create a model matrix from the light properties.
		glm::mat4 translation = glm::translate(glm::vec3(light.getLight().m_PositionWS));
		// Create a rotation matrix that rotates the model towards the direction of the light.
		//glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(0, 0, 1), glm::normalize(glm::vec3(light.getLight().m_DirectionWS))));

		// Compute the scale depending on the light type.
		float scaleX, scaleY, scaleZ;
		// For point lights, we want to scale the geometry by the range of the light.
		scaleX = scaleY = scaleZ = light.getLight().m_Range;
		if (light.getLight().m_Type == Light::LightType::Spot)
		{
			// For spotlights, we want to scale the base of the cone by the spotlight angle.
			scaleX = scaleY = glm::tan(glm::radians(light.getLight().m_SpotlightAngle)) * light.getLight().m_Range;
		}

		glm::mat4 scale = glm::scale(glm::vec3(scaleX, scaleY, scaleZ));

		perObjectData.ModelView = camera->GetViewMatrix() * translation  * scale * m_World;
		perObjectData.ModelViewProjection = camera->GetProjectionMatrix() * perObjectData.ModelView;
	}*/
	SetPerObjectConstantBufferData(perObjectData);

	// Update the constant buffer for the per-light data.
	m_pLightParams->m_Light = light.getLight();
	m_LightParamsCB->Set(*m_pLightParams);

	// Clear the stencil buffer for the next light
	m_LightPipeline0->GetRenderTarget()->Clear(ClearFlags::Stencil, glm::vec4(0), 1.0f, 1);
	// The other pipelines should have the same render target.. so no need to clear it 3 times.

	switch (light.getLight().m_Type)
	{
	case Light::LightType::Point:
		RenderSubPass(GetRenderEventArgs(), m_pDirectionalLightScene, m_DirectionalLightPipeline);
		//RenderSubPass(GetRenderEventArgs(), m_pPointLightScene, m_LightPipeline1);
		break;
	case Light::LightType::Spot:
		RenderSubPass(GetRenderEventArgs(), m_pDirectionalLightScene, m_DirectionalLightPipeline);
		//RenderSubPass(GetRenderEventArgs(), m_pSpotLightScene, m_LightPipeline1);
		break;
	case Light::LightType::Directional:
		RenderSubPass(GetRenderEventArgs(), m_pDirectionalLightScene, m_DirectionalLightPipeline);
		break;
	}

	return true;
}

void DeferredLightingPass::UpdateViewport(Viewport _viewport)
{
	m_LightPipeline0->GetRasterizerState().SetViewport(_viewport);
	m_LightPipeline1->GetRasterizerState().SetViewport(_viewport);
	m_DirectionalLightPipeline->GetRasterizerState().SetViewport(_viewport);
}

void DeferredLightingPass::UpdateFog(float fogModifier, vec3 fogColor, float fogDistance)
{
	m_pFogParams->FogModifier = fogModifier;
	m_pFogParams->FogColor = fogColor;
	m_pFogParams->FogDistance = fogDistance;
	m_FogParamsCB->Set(*m_pFogParams);
}

void DeferredLightingPass::SetRenderEventArgs(Render3DEventArgs & e)
{
	m_pRenderEventArgs = &e;
}

Render3DEventArgs& DeferredLightingPass::GetRenderEventArgs() const
{
	assert1(m_pRenderEventArgs);
	return *m_pRenderEventArgs;
}
