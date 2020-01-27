#include "stdafx.h"

// Gerenal
#include "Scene_Default.h"

// Additional
#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"
#include "Materials/MaterialModel.h"

CGameState_World::CGameState_World(IBaseManager * BaseManager)
	: SceneBase(BaseManager)
{}

CGameState_World::~CGameState_World()
{}


//
// IGameState
//
void CGameState_World::Initialize()
{
	SceneBase::Initialize();

	ISceneNode3D* cameraNode = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
	cameraNode->AddComponent(std::make_shared<CCameraComponent3D>(cameraNode));

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, 1.0f/*GetRenderWindow()->GetWindowWidth() / GetRenderWindow()->GetWindowHeight()*/, 0.5f, 10000.0f);

	Load3D();
	LoadUI();

	cameraNode->SetTranslate(vec3(-50, 160, 170));
	GetCameraController()->GetCamera()->SetYaw(-51);
	GetCameraController()->GetCamera()->SetPitch(-38);
}

void CGameState_World::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}

void CGameState_World::OnRayIntersected(const glm::vec3& Point)
{
	/*
	std::shared_ptr<MaterialDebug> matDebug = std::make_shared<MaterialDebug>(GetRenderDevice());
	matDebug->SetDiffuseColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	matDebug->SetWrapper(matDebug);

	IMesh* meshPlane = GetRenderDevice()->GetPrimitiveCollection()->CreateSphere();
	meshPlane->SetMaterial(matDebug);

	std::shared_ptr<ISceneNode3D> sceneNodePlane = m_Scene->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene->GetRootNode());
	sceneNodePlane->SetName("Sphere.");
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetTranslate(Point);
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetScale(vec3(50.0f, 50.0f, 50.0f));
	sceneNodePlane->GetComponent<IMeshComponent3D>()->AddMesh(meshPlane);

	Log::Green("Sphere created at %f %f %f", Point.x, Point.y, Point.z);
	*/
}



//
//
//
void CGameState_World::OnPreRender(RenderEventArgs& e)
{
	m_RootForBoxes->SetRotation(glm::vec3(m_RootForBoxes->GetRotation().x, m_RootForBoxes->GetRotation().y + 0.01, 0.0f));

	SceneBase::OnPreRender(e);
}



//
// Keyboard events
//
bool CGameState_World::OnWindowKeyPressed(KeyEventArgs & e)
{
	if (e.Key == KeyCode::F4)
		m_Model_Pass_Opaque->SetEnabled(!m_Model_Pass_Opaque->IsEnabled());

	if (e.Key == KeyCode::F5)
		m_Model_Pass_Transperent->SetEnabled(!m_Model_Pass_Transperent->IsEnabled());

	return SceneBase::OnWindowKeyPressed(e);
}

void CGameState_World::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}



//
//
//
void CGameState_World::Load3D()
{
	//--------------------------------------------------------------------------
	// Lights
	//--------------------------------------------------------------------------

	{
		SceneNode3D* sceneNodeLight = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		sceneNodeLight->SetName("Light node");
		sceneNodeLight->SetTranslate(glm::vec3(300.0f, 300.0f, 300.0f));
		sceneNodeLight->SetRotation(glm::vec3(-0.5f, -0.5f, -0.5f));

		sceneNodeLight->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 0.0f));
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetRange(10000.0f);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetIntensity(1.5f);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetSpotlightAngle(20.0f);

		SceneNode3D* sceneNodeLight2 = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		sceneNodeLight2->SetName("Light node2");
		sceneNodeLight2->SetTranslate(glm::vec3(-300.0f, 300.0f, 300.0f));
		sceneNodeLight2->SetRotation(glm::vec3(0.5f, -0.5f, -0.5f));

		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetRange(4000.0f);
		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetSpotlightAngle(20.0f);


#if 0
		std::shared_ptr<SceneNode3D> sceneNodeLightCenter = CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene->GetRootNode());
		sceneNodeLightCenter->SetName("Directional light");
		sceneNodeLightCenter->SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
		sceneNodeLightCenter->SetRotation(glm::normalize(glm::vec3(-0.5f, -0.5f, -0.5f)));

		sceneNodeLightCenter->GetComponent<ILightComponent3D>()->SetType(ELightType::Directional);
		sceneNodeLightCenter->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		sceneNodeLightCenter->GetComponent<ILightComponent3D>()->SetRange(10000.0f);
		sceneNodeLightCenter->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		sceneNodeLightCenter->GetComponent<ILightComponent3D>()->SetSpotlightAngle(20.0f);
#endif
	}

	//std::shared_ptr<MaterialTextured> materialTextured = std::make_shared<MaterialTextured>(GetRenderDevice());
	//materialTextured->SetDiffuseColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	//materialTextured->SetTexture(0, GetRenderDevice()->CreateTexture2D("Sponza_Floor_diffuse.png"));
	//materialTextured->SetWrapper(materialTextured);


	//--------------------------------------------------------------------------
	// Cubes & Spheres
	//--------------------------------------------------------------------------


	{
		const int iterCnt = 6;
		const float offset = 13.0f;
		const float scale = 5.0f;

		std::shared_ptr<MaterialModel> mat = std::make_shared<MaterialModel>(GetBaseManager());
		mat->SetDiffuseColor(vec3(1.0f, 1.0f, 1.0f));
		mat->SetSpecularColor(vec3(1.0f, 1.0f, 1.0f));
		mat->SetSpecularFactor(4.0f);
		mat->SetBumpFactor(4.0f);
		mat->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice()->CreateTexture2D("Sponza_Floor_diffuse.png"));
		//mat->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice()->CreateTexture2D("Sponza_Floor_normal.png"));
		mat->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice()->CreateTexture2D("Sponza_Floor_roughness.png"));
		//mat->SetTexture(MaterialModel::ETextureType::TextureBump, GetRenderDevice()->CreateTexture2D("Sponza_Floor_roughness.png"));
		mat->SetWrapper(mat.get());

		IMesh* mesh = GetRenderDevice()->GetPrimitiveCollection()->CreateCube();
		mesh->SetMaterial(mat);

		IMesh* mesh2 = GetRenderDevice()->GetPrimitiveCollection()->CreateSphere();
		mesh2->SetMaterial(mat);

		m_RootForBoxes = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		//m_RootForBoxes->SetTranslate(glm::vec3(150, 0, 150));


		for (int i = -(iterCnt / 2); i < iterCnt / 2; i++)
		{
			for (int j = -(iterCnt / 2); j < iterCnt / 2; j++)
			{
				for (int k = 0; k < iterCnt; k++)
				{
					ISceneNode3D* sceneNode = m_RootForBoxes->CreateSceneNode<SceneNode3D>();
					sceneNode->SetName("Ball [" + std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + "]");
					sceneNode->SetTranslate(vec3(offset * i, offset * k, offset * j));
					sceneNode->SetScale(vec3(scale));
					sceneNode->GetComponent<IMeshComponent3D>()->AddMesh(((i % 2 == 0) || (j % 2 == 0) && (k % 2 == 0)) ? mesh : mesh2);

					BoundingBox bbox = BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
					bbox.transform(sceneNode->GetWorldTransfom());
					sceneNode->GetComponent<CColliderComponent3D>()->SetBounds(bbox);
				}
			}
		}
	}

	//--------------------------------------------------------------------------
	// Plane
	//--------------------------------------------------------------------------

	{
		const float cPlaneSize = 500.0f;
		const float cPlaneY = -10.0f;

		std::shared_ptr<MaterialModel> mat2 = std::make_shared<MaterialModel>(GetBaseManager());
		mat2->SetDiffuseColor(vec3(1.0f, 1.0f, 1.0f));
		mat2->SetSpecularColor(vec3(1.0f, 1.0f, 1.0f));
		mat2->SetSpecularFactor(0.0f);
		mat2->SetBumpFactor(4.0f);
		//mat2->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice()->CreateTexture2D("Sponza_Ceiling_diffuse.png"));
		//mat2->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice()->CreateTexture2D("Sponza_Ceiling_normal.png"));
		//mat2->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice()->CreateTexture2D("Sponza_Ceiling_roughness.png"));
		//mat2->SetTexture(MaterialModel::ETextureType::TextureBump, GetRenderDevice()->CreateTexture2D("Sponza_Ceiling_roughness.png"));
		mat2->SetWrapper(mat2.get());

		IMesh* meshPlane = GetRenderDevice()->GetPrimitiveCollection()->CreatePlane();
		meshPlane->SetMaterial(mat2);

		ISceneNode3D* sceneNodePlane = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		sceneNodePlane->SetName("Ground");
		sceneNodePlane->SetTranslate(vec3(0, cPlaneY, 0));
		sceneNodePlane->SetScale(vec3(cPlaneSize));
		sceneNodePlane->GetComponent<IMeshComponent3D>()->AddMesh(meshPlane);
	}

	//std::shared_ptr<ISceneNode3D> fbxSceneNode = GetBaseManager()->GetManager<ISceneNodesFactory>()->CreateSceneNode(m_Scene->GetRootNode(), "FBXSceneNode");
	//fbxSceneNode->GetComponent<ITransformComponent3D>()->SetScale(vec3(15.0f, 15.0f, 15.0f));



	m_BuildRenderListPass = std::make_shared<BuildRenderListPass>(GetRenderDevice(), shared_from_this());

	m_DefferedRenderPass = std::make_shared<CDefferedRender>(GetRenderDevice(), m_BuildRenderListPass);
	m_DefferedRenderPass->CreatePipeline(GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport());

	m_DefferedRenderPrepareLights = std::make_shared<CDefferedRenderPrepareLights>(GetRenderDevice(), m_BuildRenderListPass);
	m_DefferedRenderPrepareLights->CreatePipeline(nullptr, nullptr);

	m_DefferedFinalRenderPass = std::make_shared<CDefferedRenderFinal>(GetRenderDevice(), m_DefferedRenderPass, m_DefferedRenderPrepareLights);
	m_DefferedFinalRenderPass->CreatePipeline(GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport());



	m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("ClearPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), shared_from_this()));
	m_Technique3D.AddPass(m_BuildRenderListPass);
	m_Technique3D.AddPass(m_DefferedRenderPass);
	m_Technique3D.AddPass(m_DefferedRenderPrepareLights);
	m_Technique3D.AddPass(m_DefferedFinalRenderPass);
	m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("TexturedMaterialPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), shared_from_this()));
}

void CGameState_World::LoadUI()
{
	/*std::shared_ptr<CUITextureNode> TextureUI0 = m_SceneUI->GetRootNode()->CreateWrappedSceneNode<CUITextureNode>("Test", GetRenderDevice());
	TextureUI0->SetTranslate(vec2(000.0f, 000.0f));
	TextureUI0->SetScale(vec2(600, 600));
	TextureUI0->SetTexture(m_DefferedRenderPass->GetTexture0());

	std::shared_ptr<CUITextureNode> TextureUI1 = m_SceneUI->GetRootNode()->CreateWrappedSceneNode<CUITextureNode>("Test", GetRenderDevice());
	TextureUI1->SetTranslate(vec2(600.0f, 000.0f));
	TextureUI1->SetScale(vec2(600, 600));
	TextureUI1->SetTexture(m_DefferedRenderPass->GetTexture1());

	std::shared_ptr<CUITextureNode> TextureUI2 = m_SceneUI->GetRootNode()->CreateWrappedSceneNode<CUITextureNode>("Test", GetRenderDevice());
	TextureUI2->SetTranslate(vec2(000.0f, 600.0f));
	TextureUI2->SetScale(vec2(600, 600));
	TextureUI2->SetTexture(m_DefferedRenderPass->GetTexture2());

	std::shared_ptr<CUITextureNode> TextureUI3 = m_SceneUI->GetRootNode()->CreateWrappedSceneNode<CUITextureNode>("Test", GetRenderDevice());
	TextureUI3->SetTranslate(vec2(600.0f, 600.0f));
	TextureUI3->SetScale(vec2(600, 600));
	TextureUI3->SetTexture(m_DefferedRenderPass->GetTexture3());*/

	m_TechniqueUI.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("BaseUIPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), shared_from_this()));
}


void CGameState_World::GenerateLights(std::shared_ptr<ISceneNode3D> Node, uint32_t numLights)
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
		SLight light;

		light.PositionWS = glm::vec4(glm::linearRand(BoundsMin, BoundsMax), 1.0f);

		// Choose a color that will never be black.
		glm::vec2 colorWheel = glm::diskRand(1.0f);
		float radius = glm::length(colorWheel);
		light.Color.rgb = glm::lerp(
			glm::lerp(
				glm::lerp(glm::vec3(1), glm::vec3(0, 1, 0), radius),
				glm::lerp(glm::vec3(1), glm::vec3(1, 0, 0), radius),
				colorWheel.x * 0.5f + 0.5f),
			glm::lerp(
				glm::lerp(glm::vec3(1), glm::vec3(0, 0, 1), radius),
				glm::lerp(glm::vec3(1), glm::vec3(1, 1, 0), radius),
				colorWheel.y * 0.5f + 0.5f),
			glm::abs(colorWheel.y));

		light.DirectionWS = glm::vec4(glm::sphericalRand(1.0f), 0.0f);
		light.Range = glm::linearRand(MinRange, MaxRange);
		light.Intensity = 50;
		light.SpotlightAngle = glm::linearRand(MinSpotAngle, MaxSpotAngle);

		float fLightPropability = glm::linearRand(0.0f, 1.0f);

		if (GeneratePointLights && GenerateSpotLights && GenerateDirectionalLights)
		{
			light.Type = (fLightPropability < 0.33f ? ELightType::Point : fLightPropability < 0.66f ? ELightType::Spot : ELightType::Directional);
		}
		else if (GeneratePointLights && GenerateSpotLights && !GenerateDirectionalLights)
		{
			light.Type = (fLightPropability < 0.5f ? ELightType::Point : ELightType::Spot);
		}
		else if (GeneratePointLights && !GenerateSpotLights && GenerateDirectionalLights)
		{
			light.Type = (fLightPropability < 0.5f ? ELightType::Point : ELightType::Directional);
		}
		else if (GeneratePointLights && !GenerateSpotLights && !GenerateDirectionalLights)
		{
			light.Type = ELightType::Point;
		}
		else if (!GeneratePointLights && GenerateSpotLights && GenerateDirectionalLights)
		{
			light.Type = (fLightPropability < 0.5f ? ELightType::Spot : ELightType::Directional);
		}
		else if (!GeneratePointLights && GenerateSpotLights && !GenerateDirectionalLights)
		{
			light.Type = ELightType::Spot;
		}
		else if (!GeneratePointLights && !GenerateSpotLights && GenerateDirectionalLights)
		{
			light.Type = ELightType::Directional;
		}
		else if (!GeneratePointLights && !GenerateSpotLights && !GenerateDirectionalLights)
		{
			light.Type = (fLightPropability < 0.33f ? ELightType::Point : fLightPropability < 0.66f ? ELightType::Spot : ELightType::Directional);
		}

		//Node->GetComponent<ILightComponent3D>()->AddLight(std::make_shared<CLight3D>(light));
	}
}

void CGameState_World::UpdateLights()
{

}