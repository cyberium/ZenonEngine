#include <stdafx.h>

// General
#include "DeferredLightingPass.h"

// Additional
#include "Application.h"
#include "SceneFunctional/Base/SceneBase.h"
#include "SceneFunctional/3D/SceneNode3D.h"

DeferredLightingPass::DeferredLightingPass(
	std::shared_ptr<IRenderDevice> RenderDevice,
	std::shared_ptr<IScene> scene,
	std::shared_ptr<IPipelineState> lightPipeline0,
	std::shared_ptr<IPipelineState> lightPipeline1,
	std::shared_ptr<IPipelineState> directionalLightPipeline,
	std::shared_ptr<ITexture> positionTexture,
	std::shared_ptr<ITexture> diffuseTexture,
	std::shared_ptr<ITexture> specularTexture,
	std::shared_ptr<ITexture> normalTexture,
	std::shared_ptr<ITexture> depthTexture
)
	: CBaseScenePass(RenderDevice, scene, nullptr)
	, m_LightPipeline0(lightPipeline0)
	, m_LightPipeline1(lightPipeline1)
	, m_DirectionalLightPipeline(directionalLightPipeline)
	, m_PositionTexture(positionTexture)
	, m_DiffuseTexture(diffuseTexture)
	, m_SpecularTexture(specularTexture)
	, m_NormalTexture(normalTexture)
	, m_DepthTexture(depthTexture)
{
	IBaseManager* baseManager = std::dynamic_pointer_cast<IBaseManagerHolder>(scene)->GetBaseManager();

	m_pScreenToViewParams = (ScreenToViewParams*)_aligned_malloc(sizeof(ScreenToViewParams), 16);
	m_ScreenToViewParamsCB = _RenderDevice->CreateConstantBuffer(ScreenToViewParams());

	m_pLightParams = (LightParams*)_aligned_malloc(sizeof(LightParams), 16);
	m_LightParamsCB = _RenderDevice->CreateConstantBuffer(LightParams());

	m_pFogParams = (FogParams*)_aligned_malloc(sizeof(FogParams), 16);
	m_FogParamsCB = _RenderDevice->CreateConstantBuffer(FogParams());

	// PointLightScene
	m_pPointLightScene = std::make_shared<SceneBase>(baseManager);

	std::shared_ptr<SceneNode3D> sphereSceneNode = m_pPointLightScene->CreateSceneNode<SceneNode3D>(std::shared_ptr<ISceneNode>());

	std::shared_ptr<IMesh> sphereMesh = _RenderDevice->GetPrimitiveCollection()->CreateSphere();
	sphereSceneNode->GetComponent<IMeshComponent3D>()->AddMesh(sphereMesh);

	// Create a full-screen quad that is placed on the far clip plane.
	m_pDirectionalLightScene = std::make_shared<SceneBase>(baseManager);

	std::shared_ptr<SceneNode3D> quadSceneNode = m_pDirectionalLightScene->CreateSceneNode<SceneNode3D>(std::shared_ptr<ISceneNode>());
	quadSceneNode->SetParent(m_pDirectionalLightScene->GetRootNode());

	std::shared_ptr<IMesh> quadMesh = _RenderDevice->GetPrimitiveCollection()->CreateScreenQuad(0, 1280, 1024, 0); // _RenderDevice->CreateScreenQuad(-1, 1, -1, 1, -1);
	quadSceneNode->GetComponent<IMeshComponent3D>()->AddMesh(quadMesh);
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

void DeferredLightingPass::PreRender(RenderEventArgs& e)
{
	e.PipelineState = nullptr;
	SetRenderEventArgs(&e);

	// Bind the G-buffer textures to the pixel shader pipeline stage.
	m_PositionTexture->Bind(0, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
	m_DiffuseTexture->Bind(1, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
	m_SpecularTexture->Bind(2, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
	m_NormalTexture->Bind(3, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
	m_DepthTexture->Bind(4, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
}

void DeferredLightingPass::RenderSubPass(RenderEventArgs* e, std::shared_ptr<IScene> scene, std::shared_ptr<IPipelineState> pipeline)
{
	e->PipelineState = pipeline.get();
	SetRenderEventArgs(e);

	pipeline->Bind();

	scene->Accept(this);

	pipeline->UnBind();
}

// Render the pass. This should only be called by the RenderTechnique.
void DeferredLightingPass::Render(RenderEventArgs& e)
{
	const ICamera* pCamera = e.Camera;
	_ASSERT(pCamera != nullptr);

	const Viewport * viewport = e.PipelineState->GetRasterizerState()->GetViewports()[0];
	// We need the inverse projection matrix to compute the view space position of the fragment
	// in the deferred lighting shader.
	m_pScreenToViewParams->m_InverseProjectionMatrix = glm::inverse(pCamera->GetProjectionMatrix());
	m_pScreenToViewParams->m_ScreenDimensions = glm::vec2(viewport->GetWidth(), viewport->GetHeight());
	m_pScreenToViewParams->m_CameraPos = vec4(pCamera->GetTranslation(), 1.0f);
	m_ScreenToViewParamsCB->Set(m_pScreenToViewParams, sizeof(ScreenToViewParams));

	// Connect shader parameters to shaders.
	// This ensures the right parameters are bound to the rendering pipeline when the shader is bound.
	// (This can probably be done once in the constructor, but if the shaders are recompiled, the parameters
	// need to be reconnected to the shaders.. soooo better safe than sorry I guess).
	std::vector<IPipelineState* > pipelines = { m_LightPipeline0.get(), m_LightPipeline1.get(), m_DirectionalLightPipeline.get() };
	for (auto pipeline : pipelines)
	{
		std::shared_ptr<IShader> vertexShader = pipeline->GetShader(IShader::ShaderType::VertexShader);
		BindPerObjectConstantBuffer(vertexShader);

		std::shared_ptr<IShader> pixelShader = pipeline->GetShader(IShader::ShaderType::PixelShader);
		if (pixelShader)
		{
			// Bind the per-light & deferred lighting properties constant buffers to the pixel shader.
			pixelShader->GetShaderParameterByName("LightIndexBuffer")->Set(m_LightParamsCB.get());
			pixelShader->GetShaderParameterByName("ScreenToViewParams")->Set(m_ScreenToViewParamsCB.get());
			pixelShader->GetShaderParameterByName("FogParams")->Set(m_FogParamsCB.get());
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

	GetScene()->Accept(this);
}

void DeferredLightingPass::PostRender(RenderEventArgs& e)
{
	// Explicitly unbind these textures so they can be used as render target textures.
	m_PositionTexture->UnBind(0, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
	m_DiffuseTexture->UnBind(1, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
	m_SpecularTexture->UnBind(2, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
	m_NormalTexture->UnBind(3, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
	m_DepthTexture->UnBind(4, IShader::ShaderType::PixelShader, IShaderParameter::Type::Texture);
}

// Inherited from Visitor

bool DeferredLightingPass::Visit(ISceneNode3D* node)
{
	ISceneNode* sceneNode = dynamic_cast<ISceneNode*>(node);

	m_World = sceneNode->GetComponent<ITransformComponent>()->GetWorldTransfom();

	return true;
}

bool DeferredLightingPass::Visit(IMesh* Mesh, UINT IndexStartLocation, UINT IndexCnt, UINT VertexStartLocation, UINT VertexCnt)
{
	if (GetRenderEventArgs() && Mesh->GetMaterial() == nullptr) // TODO: Fixme
	{
		return Mesh->Render(GetRenderEventArgs(), m_PerObjectConstantBuffer.get());
	}

	return false;
}

bool DeferredLightingPass::Visit(std::shared_ptr < CLight3D> light)
{
	const ICamera* camera = GetRenderEventArgs()->Camera;
	_ASSERT(camera != nullptr);

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
	m_pLightParams->m_Light = light->getLight();
	m_LightParamsCB->Set(m_pLightParams, sizeof(LightParams));

	// Clear the stencil buffer for the next light
	m_LightPipeline0->GetRenderTarget()->Clear(ClearFlags::Stencil, glm::vec4(0), 1.0f, 1);
	// The other pipelines should have the same render target.. so no need to clear it 3 times.

	switch (light->getLight().m_Type)
	{
	case Light::LightType::Point:
		RenderSubPass(GetRenderEventArgs(), m_pDirectionalLightScene, m_DirectionalLightPipeline);
		RenderSubPass(GetRenderEventArgs(), m_pPointLightScene, m_LightPipeline1);
		break;
	case Light::LightType::Spot:
		RenderSubPass(GetRenderEventArgs(), m_pDirectionalLightScene, m_DirectionalLightPipeline);
		RenderSubPass(GetRenderEventArgs(), m_pSpotLightScene, m_LightPipeline1);
		break;
	case Light::LightType::Directional:
		RenderSubPass(GetRenderEventArgs(), m_pDirectionalLightScene, m_DirectionalLightPipeline);
		break;
	}

	return true;
}

void DeferredLightingPass::UpdateViewport(const Viewport * _viewport)
{
	m_LightPipeline0->GetRasterizerState()->SetViewport(_viewport);
	m_LightPipeline1->GetRasterizerState()->SetViewport(_viewport);
	m_DirectionalLightPipeline->GetRasterizerState()->SetViewport(_viewport);
}

void DeferredLightingPass::UpdateFog(float fogModifier, vec3 fogColor, float fogDistance)
{
	m_pFogParams->FogModifier = fogModifier;
	m_pFogParams->FogColor = fogColor;
	m_pFogParams->FogDistance = fogDistance;
	m_FogParamsCB->Set(*m_pFogParams);
}
