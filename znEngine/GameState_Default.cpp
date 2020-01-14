#include "stdafx.h"

// Gerenal
#include "GameState_Default.h"

// Additional
#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"
#include "Materials/MaterialModel.h"

CGameState_World::CGameState_World(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents)
	: CGameState(BaseManager, RenderWindow, WindowEvents)
{}

CGameState_World::~CGameState_World()
{}


//
// IGameState
//
bool CGameState_World::Init()
{
	CGameState::Init();

	std::shared_ptr<CSceneNodeCamera> cameraNode = m_Scene3D->CreateSceneNode<CSceneNodeCamera>(m_Scene3D->GetRootNode());

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode);
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICamera::EPerspectiveProjectionHand::Right, 45.0f, GetRenderWindow()->GetWindowWidth() / GetRenderWindow()->GetWindowHeight(), 0.5f, 10000.0f);

	Load3D();
	LoadUI();

	GetCameraController()->GetCameraMovement()->SetTranslate(vec3(-500, 1600, 1700));
	GetCameraController()->GetCameraMovement()->SetYaw(-51);
	GetCameraController()->GetCameraMovement()->SetPitch(-38);

	return true;
}

void CGameState_World::Destroy()
{
	// Insert code here

	CGameState::Destroy();
}

void CGameState_World::OnRayIntersected(const glm::vec3& Point)
{
	std::shared_ptr<MaterialDebug> matDebug = std::make_shared<MaterialDebug>(GetRenderDevice());
	matDebug->SetDiffuseColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	matDebug->SetWrapper(matDebug);

	std::shared_ptr<IMesh> meshPlane = GetRenderDevice()->GetPrimitiveCollection()->CreateSphere();
	meshPlane->SetMaterial(matDebug);

	std::shared_ptr<ISceneNode> sceneNodePlane = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
	sceneNodePlane->SetName("Sphere.");
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetTranslate(Point);
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetScale(vec3(50.0f, 50.0f, 50.0f));
	sceneNodePlane->GetComponent<IMeshComponent3D>()->AddMesh(meshPlane);

	Log::Green("Sphere created at %f %f %f", Point.x, Point.y, Point.z);
}


//
//
//

void CGameState_World::OnResize(ResizeEventArgs& e)
{
	CGameState::OnResize(e);
}

void CGameState_World::OnPreRender(RenderEventArgs& e)
{
	CGameState::OnPreRender(e);
}

void CGameState_World::OnRender(RenderEventArgs& e)
{
	CGameState::OnRender(e);
}

void CGameState_World::OnPostRender(RenderEventArgs& e)
{
	CGameState::OnPostRender(e);
}

void CGameState_World::OnRenderUI(RenderEventArgs& e)
{
	CGameState::OnRenderUI(e);
}



//
// Keyboard events
//
void CGameState_World::OnKeyPressed(KeyEventArgs & e)
{
	if (e.Key == KeyCode::F4)
		m_Model_Pass_Opaque->SetEnabled(!m_Model_Pass_Opaque->IsEnabled());

	if (e.Key == KeyCode::F5)
		m_Model_Pass_Transperent->SetEnabled(!m_Model_Pass_Transperent->IsEnabled());

	CGameState::OnKeyPressed(e);
}

void CGameState_World::OnKeyReleased(KeyEventArgs & e)
{
	CGameState::OnKeyReleased(e);
}



//
//
//
void CGameState_World::Load3D()
{
	std::shared_ptr<ISceneNode> sceneNodeLight = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
	sceneNodeLight->SetName("Light node");

	Light light;
	light.m_Type = Light::LightType::Spot;
	light.m_Color.rgb = glm::vec3(1.0f, 1.0f, 1.0f);
	light.m_PositionWS = glm::vec4(glm::vec3(1032.0f, 1250.0f, 1085.0f), 1.0f);
	light.m_DirectionWS = glm::vec4(glm::vec3(-0.5f, -0.6f, -0.5f), 0.0f);
	light.m_Range = 6000.0f;
	light.m_Intensity = 1.5f;
	light.m_SpotlightAngle = 35.0f;
	sceneNodeLight->GetComponent<ILightComponent3D>()->AddLight(std::make_shared<CLight3D>(light));



	//GenerateLights(sceneNodeLight, 8);



	std::shared_ptr<MaterialTextured> materialTextured = std::make_shared<MaterialTextured>(GetRenderDevice());
	materialTextured->SetDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	materialTextured->SetTexture(0, GetRenderDevice()->CreateTexture2D("Sponza_Floor_diffuse.png"));
	materialTextured->SetWrapper(materialTextured);





	//-- Assimp -----------------

	//m_LightsStructuredBuffer = GetRenderDevice()->CreateStructuredBuffer();

	//---------------------------

	const size_t iterCnt = 3;
	const float offset = 145.0f;

	std::shared_ptr<MaterialModel> mat = std::make_shared<MaterialModel>(GetBaseManager());
	mat->SetDiffuseColor(vec3(1.0f, 1.0f, 1.0f));
	mat->SetSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	mat->SetSpecularFactor(4.0f);
	mat->SetBumpFactor(4.0f);
	mat->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice()->CreateTexture2D("Sponza_Floor_diffuse.png"));
	//mat->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice()->CreateTexture2D("Sponza_Floor_normal.png"));
	mat->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice()->CreateTexture2D("Sponza_Floor_roughness.png"));
	mat->SetTexture(MaterialModel::ETextureType::TextureBump, GetRenderDevice()->CreateTexture2D("Sponza_Floor_roughness.png"));
	mat->SetWrapper(mat);


	std::shared_ptr<MaterialModel> mat2 = std::make_shared<MaterialModel>(GetBaseManager());
	mat2->SetDiffuseColor(vec3(1.0f, 1.0f, 1.0f));
	mat2->SetSpecularColor(vec3(1.0f, 1.0f, 1.0f));
	mat2->SetSpecularFactor(4.0f);
	mat2->SetBumpFactor(4.0f);
	mat2->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice()->CreateTexture2D("Sponza_Ceiling_diffuse.png"));
	//mat2->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice()->CreateTexture2D("Sponza_Ceiling_normal.png"));
	mat2->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice()->CreateTexture2D("Sponza_Ceiling_roughness.png"));
	mat2->SetTexture(MaterialModel::ETextureType::TextureBump, GetRenderDevice()->CreateTexture2D("Sponza_Ceiling_roughness.png"));
	mat2->SetWrapper(mat2);


	std::shared_ptr<IMesh> mesh = GetRenderDevice()->GetPrimitiveCollection()->CreateCube();
	mesh->SetMaterial(mat);

	std::shared_ptr<IMesh> mesh2 = GetRenderDevice()->GetPrimitiveCollection()->CreateSphere();
	mesh2->SetMaterial(mat);

	for (size_t i = 0; i < iterCnt; i++)
	{
		for (size_t j = 0; j < iterCnt; j++)
		{
			for (size_t k = 0; k < iterCnt; k++)
			{
				std::shared_ptr<ISceneNode> sceneNode = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
				sceneNode->SetName("Ball [" + std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + "]");
				std::dynamic_pointer_cast<ISceneNode3D>(sceneNode)->SetTranslate(vec3(offset * i, offset * k, offset * j));
				std::dynamic_pointer_cast<ISceneNode3D>(sceneNode)->SetScale(vec3(100, 100, 100));
				sceneNode->GetComponent<IMeshComponent3D>()->AddMesh(((i % 2 == 0) && (j % 2 == 0) && (k % 2 == 0)) ? mesh : mesh2);

				BoundingBox bbox = BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				bbox.transform(sceneNode->GetWorldTransfom());
				sceneNode->GetComponent<CColliderComponent3D>()->SetBounds(bbox);
			}
		}
	}

	std::shared_ptr<IMesh> meshPlane = GetRenderDevice()->GetPrimitiveCollection()->CreatePlane();
	meshPlane->SetMaterial(mat2);

	std::shared_ptr<ISceneNode> sceneNodePlane = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
	sceneNodePlane->SetName("Ground.");
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetTranslate(vec3(0, -50.0f, 0));
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetScale(vec3(1000.0f, 1.0f, 1000.0f));
	sceneNodePlane->GetComponent<IMeshComponent3D>()->AddMesh(meshPlane);

	//std::shared_ptr<ISceneNode> fbxSceneNode = GetBaseManager()->GetManager<ISceneNodesFactory>()->CreateSceneNode(m_Scene3D->GetRootNode(), "FBXSceneNode");
	//fbxSceneNode->GetComponent<ITransformComponent3D>()->SetScale(vec3(15.0f, 15.0f, 15.0f));

	m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("ClearPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	//m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("DebugPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("TexturedMaterialPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));

	m_CollectLightPass = std::make_shared<CCollectLightPass>(GetRenderDevice(), m_Scene3D);
	m_Model_Pass_Opaque = GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassOpaque", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D);
	//m_Model_Pass_Transperent = GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassTransperent", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D);

	m_ShadowPass = std::make_shared<CShadowPass>(GetRenderDevice(), m_Scene3D);

	m_Technique3D.AddPass(m_CollectLightPass);
	m_Technique3D.AddPass
	(
		std::make_shared<CSetShaderParameterPass>
		(
			GetRenderDevice(),
			std::dynamic_pointer_cast<AbstractPass>(m_Model_Pass_Opaque)->GetPipelineState()->GetShader(SShaderType::VertexShader)->GetShaderParameterByName("Lights"),
			std::bind(&CCollectLightPass::GetLightBuffer, m_CollectLightPass)
			)
	);
	m_Technique3D.AddPass
	(
		std::make_shared<CSetShaderParameterPass>
		(
			GetRenderDevice(), 
			std::dynamic_pointer_cast<AbstractPass>(m_Model_Pass_Opaque)->GetPipelineState()->GetShader(SShaderType::PixelShader)->GetShaderParameterByName("Lights"), 
			std::bind(&CCollectLightPass::GetLightBuffer, m_CollectLightPass)
		)
	);

	//
	// Shadow
	//
	m_Technique3D.AddPass(m_ShadowPass);
	m_Technique3D.AddPass
	(
		std::make_shared<CSetShaderParameterPass>
		(
			GetRenderDevice(),
			std::dynamic_pointer_cast<AbstractPass>(m_Model_Pass_Opaque)->GetPipelineState()->GetShader(SShaderType::VertexShader)->GetShaderParameterByName("PerLight"),
			std::bind(&CShadowPass::GetPerLightBuffer, m_ShadowPass)
		)
	);
	m_Technique3D.AddPass
	(
		std::make_shared<CSetShaderParameterPass>
		(
			GetRenderDevice(),
			std::dynamic_pointer_cast<AbstractPass>(m_Model_Pass_Opaque)->GetPipelineState()->GetShader(SShaderType::PixelShader)->GetShaderParameterByName("TextureShadow"),
			std::bind(&CShadowPass::GetColorTexture, m_ShadowPass)
			)
	);

	m_Technique3D.AddPass(m_Model_Pass_Opaque);
	//m_Technique3D.AddPass(m_Model_Pass_Transperent);
}

void CGameState_World::LoadUI()
{
	m_TextureUI = m_SceneUI->GetRootNode()->CreateWrappedSceneNode<CUITextureNode>("Test", GetRenderDevice());
	m_TextureUI->SetTranslate(vec2(100.0f, 100.0f));
	m_TextureUI->SetScale(vec2(300, 300));
	m_TextureUI->SetTexture(m_ShadowPass->GetColorTexture());

	m_TechniqueUI.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("BaseUIPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_SceneUI));
}


void CGameState_World::GenerateLights(std::shared_ptr<ISceneNode> Node, uint32_t numLights)
{
	float MinRange = 100.1f;
	float MaxRange = 2000.0f;

	float MinSpotAngle = 1.0f;
	float MaxSpotAngle = 60.0f;

	vec3 BoundsMin = vec3(-500, -200, -500);
	vec3 BoundsMax = vec3(500, 200, 500);
	bool GeneratePointLights = true;
	bool GenerateSpotLights = false;
	bool GenerateDirectionalLights = false;

	for (uint32_t i = 0; i < numLights; i++)
	{
		Light light;

		light.m_PositionWS = glm::vec4(glm::linearRand(BoundsMin, BoundsMax), 1.0f);

		// Choose a color that will never be black.
		glm::vec2 colorWheel = glm::diskRand(1.0f);
		float radius = glm::length(colorWheel);
		light.m_Color.rgb = glm::lerp(
			glm::lerp(
				glm::lerp(glm::vec3(1), glm::vec3(0, 1, 0), radius),
				glm::lerp(glm::vec3(1), glm::vec3(1, 0, 0), radius),
				colorWheel.x * 0.5f + 0.5f),
			glm::lerp(
				glm::lerp(glm::vec3(1), glm::vec3(0, 0, 1), radius),
				glm::lerp(glm::vec3(1), glm::vec3(1, 1, 0), radius),
				colorWheel.y * 0.5f + 0.5f),
			glm::abs(colorWheel.y));

		light.m_DirectionWS = glm::vec4(glm::sphericalRand(1.0f), 0.0f);
		light.m_Range = glm::linearRand(MinRange, MaxRange);
		light.m_Intensity = 50;
		light.m_SpotlightAngle = glm::linearRand(MinSpotAngle, MaxSpotAngle);

		float fLightPropability = glm::linearRand(0.0f, 1.0f);

		if (GeneratePointLights && GenerateSpotLights && GenerateDirectionalLights)
		{
			light.m_Type = (fLightPropability < 0.33f ? Light::LightType::Point : fLightPropability < 0.66f ? Light::LightType::Spot : Light::LightType::Directional);
		}
		else if (GeneratePointLights && GenerateSpotLights && !GenerateDirectionalLights)
		{
			light.m_Type = (fLightPropability < 0.5f ? Light::LightType::Point : Light::LightType::Spot);
		}
		else if (GeneratePointLights && !GenerateSpotLights && GenerateDirectionalLights)
		{
			light.m_Type = (fLightPropability < 0.5f ? Light::LightType::Point : Light::LightType::Directional);
		}
		else if (GeneratePointLights && !GenerateSpotLights && !GenerateDirectionalLights)
		{
			light.m_Type = Light::LightType::Point;
		}
		else if (!GeneratePointLights && GenerateSpotLights && GenerateDirectionalLights)
		{
			light.m_Type = (fLightPropability < 0.5f ? Light::LightType::Spot : Light::LightType::Directional);
		}
		else if (!GeneratePointLights && GenerateSpotLights && !GenerateDirectionalLights)
		{
			light.m_Type = Light::LightType::Spot;
		}
		else if (!GeneratePointLights && !GenerateSpotLights && GenerateDirectionalLights)
		{
			light.m_Type = Light::LightType::Directional;
		}
		else if (!GeneratePointLights && !GenerateSpotLights && !GenerateDirectionalLights)
		{
			light.m_Type = (fLightPropability < 0.33f ? Light::LightType::Point : fLightPropability < 0.66f ? Light::LightType::Spot : Light::LightType::Directional);
		}

		Node->GetComponent<ILightComponent3D>()->AddLight(std::make_shared<CLight3D>(light));
	}
}

void CGameState_World::UpdateLights()
{

}