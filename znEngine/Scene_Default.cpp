#include "stdafx.h"

// Gerenal
#include "Scene_Default.h"

// Additional
#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"
#include "Materials/MaterialParticle.h"
#include "Materials/MaterialModel.h"

#include "Passes/MaterialDebugPass.h"
#include "Passes/MaterialTexturedPass.h"
#include "Passes/MaterialParticlePass.h"

#include "Passes/MaterialPassOpaque.h"
#include "Passes/MaterialPassTransperent.h"
#include "Passes/UIFontPass.h"

#include "Physics/Adapters/ReactPhysicsComponent.h"

// Additional
#include <filesystem>
namespace fs = std::experimental::filesystem;

namespace
{
	std::string str_tolower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), [](char c)
		{
			return std::tolower(c, std::locale()); }
		);
		return s;
	}

	std::vector<std::string> GetAllFilesInDirectory(const std::string& Directory, const std::vector<std::string> DirSkipList = { }, const std::string& FileExtention = "")
	{
		std::vector<std::string> listOfFiles;

		try
		{
			// Check if given path exists and points to a directory
			if (fs::exists(Directory) && fs::is_directory(Directory))
			{
				// Create a Recursive Directory Iterator object and points to the starting of directory
				fs::recursive_directory_iterator iter(Directory);

				// Create a Recursive Directory Iterator object pointing to end.
				fs::recursive_directory_iterator end;

				// Iterate till end
				while (iter != end)
				{
					// Check if current entry is a directory and if exists in skip list
					if (fs::is_directory(iter->path()) && (std::find(DirSkipList.begin(), DirSkipList.end(), iter->path().filename()) != DirSkipList.end()))
					{
						iter.disable_recursion_pending();
					}
					else if (!fs::is_directory(iter->path()) && (!FileExtention.empty()) && (iter->path().has_extension()) && (str_tolower(iter->path().extension().string()) == str_tolower(FileExtention)))
					{
						listOfFiles.push_back(iter->path().string());
					}
					else
					{
						iter.disable_recursion_pending();
					}

					std::error_code ec;
					// Increment the iterator to point to next entry in recursive iteration
					iter.increment(ec);
					if (ec)
					{
						Log::Error("GetAllFilesInDirectory: Error while accessing '%s'. Error: '%s'.", iter->path().string().c_str(), ec.message().c_str());
					}
				}
			}
		}
		catch (const std::system_error& e)
		{
			Log::Error("GetAllFilesInDirectory: Exception '%s'", e.what());
		}

		return listOfFiles;
	}
}

CSceneDefault::CSceneDefault(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
	, m_World(rp3d::Vector3(0.0f, -9.81f, 0.0f))
{
	// Change the number of iterations of the velocity solver
	m_World.setNbIterationsVelocitySolver(15);

	// Change the number of iterations of the position solver
	m_World.setNbIterationsPositionSolver(8);

}

CSceneDefault::~CSceneDefault()
{
}


//
// IGameState
//
void CSceneDefault::Initialize()
{
	SceneBase::Initialize();

	auto cameraNode = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
	cameraNode->AddComponent(std::make_shared<CCameraComponent3D>(*cameraNode));

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 0.5f, 10000.0f);

	rp3d::Vector3 gravity(0.0, -9.81, 0.0);
	// Create the dynamics world
	rp3d::DynamicsWorld world(gravity);


	Load3D();
	LoadUI();

	cameraNode->SetTranslate(glm::vec3(-50, 160, 170));
	GetCameraController()->GetCamera()->SetYaw(-51);
	GetCameraController()->GetCamera()->SetPitch(-38);
}

void CSceneDefault::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}

void CSceneDefault::OnRayIntersected(const glm::vec3& Point)
{
	/*
	std::shared_ptr<MaterialDebug> matDebug = std::make_shared<MaterialDebug>(GetRenderDevice());
	matDebug->SetDiffuseColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	matDebug->SetWrapper(matDebug);

	IModel* meshPlane = GetRenderDevice().GetPrimitiveCollection()->CreateSphere();
	meshPlane->SetMaterial(matDebug);

	std::shared_ptr<ISceneNode3D> sceneNodePlane = m_Scene->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene->GetRootNode());
	sceneNodePlane->SetName("Sphere.");
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetTranslate(Point);
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetScale(vec3(50.0f, 50.0f, 50.0f));
	sceneNodePlane->GetComponent<IModelsComponent3D>()->AddMesh(meshPlane);

	Log::Green("Sphere created at %f %f %f", Point.x, Point.y, Point.z);
	*/
}



//
//
//
void CSceneDefault::OnPreRender(RenderEventArgs& e)
{
	m_World.update(e.DeltaTime / 1000.0f * 2.0f);

	//m_RootForBoxes->SetRotation(glm::vec3(m_RootForBoxes->GetRotation().x, m_RootForBoxes->GetRotation().y + 0.01, 0.0f));

	SceneBase::OnPreRender(e);
}



//
// Keyboard events
//
bool CSceneDefault::OnWindowKeyPressed(KeyEventArgs & e)
{
	//if (e.Key == KeyCode::F4)
	//	m_Model_Pass_Opaque->SetEnabled(!m_Model_Pass_Opaque->IsEnabled());

	//if (e.Key == KeyCode::F5)
	//	m_Model_Pass_Transperent->SetEnabled(!m_Model_Pass_Transperent->IsEnabled());

	return SceneBase::OnWindowKeyPressed(e);
}

void CSceneDefault::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}



//
//
//
void CSceneDefault::Load3D()
{
	//--------------------------------------------------------------------------
	// Lights
	//--------------------------------------------------------------------------

	{
		auto sceneNodeLight = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		sceneNodeLight->SetName("Light node");
		sceneNodeLight->SetTranslate(glm::vec3(300.0f, 300.0f, 300.0f));
		sceneNodeLight->SetRotation(glm::vec3(-0.5f, -0.5f, -0.5f));

		sceneNodeLight->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetRange(5000.0f);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetSpotlightAngle(25.0f);
#if 0
		auto sceneNodeLight2 = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		sceneNodeLight2->SetName("Light node2");
		sceneNodeLight2->SetTranslate(glm::vec3(-300.0f, 300.0f, 300.0f));
		sceneNodeLight2->SetRotation(glm::vec3(0.5f, -0.5f, -0.5f));

		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(0.0f, 0.0f, 1.0f));
		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetRange(7000.0f);
		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		sceneNodeLight2->GetComponent<ILightComponent3D>()->SetSpotlightAngle(20.0f);



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
	//materialTextured->SetTexture(0, GetRenderDevice().CreateTexture2D("Sponza_Floor_diffuse.png"));
	//materialTextured->SetWrapper(materialTextured);


	//--------------------------------------------------------------------------
	// Cubes & Spheres
	//--------------------------------------------------------------------------

	Random r(0);


	{
		const int iterCnt = 0;
		const float offset = 13.0f;
		const float scale = 5.0f;

		std::shared_ptr<MaterialModel> textMaterial = std::make_shared<MaterialModel>(GetBaseManager());
		textMaterial->SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularFactor(4.0f);
		textMaterial->SetBumpFactor(8.0f);
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene\\BuildingTextures\\concrete_smooth_03_diff.dds"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice().GetObjectsFactory().LoadTexture2D("Sponza_Ceiling_normal.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene\\BuildingTextures\\concrete_smooth_03_spec.dds"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureBump, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene\\BuildingTextures\\concrete_smooth_03_ddna.dds"));

		//std::shared_ptr<MaterialTextured> textMaterial = std::make_shared<MaterialTextured>(GetRenderDevice());
		//textMaterial->SetTexture(0, GetRenderDevice().GetObjectsFactory().LoadTexture2D("Sponza_Floor_diffuse.png"));

		auto cubeModel = GetRenderDevice().GetObjectsFactory().CreateModel();
		cubeModel->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());
	
		auto sphereModel = GetRenderDevice().GetObjectsFactory().CreateModel();
		sphereModel->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateSphere());


		m_RootForBoxes = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		//m_RootForBoxes->SetTranslate(glm::vec3(150, 0, 150));


		for (int i = -(iterCnt / 2); i < iterCnt / 2; i++)
		{
			for (int j = -(iterCnt / 2); j < iterCnt / 2; j++)
			{
				for (int k = 0; k < iterCnt; k++)
				{
					auto sceneNode = m_RootForBoxes->CreateSceneNode<SceneNode3D>();
					sceneNode->SetName("Ball [" + std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + "]");
					sceneNode->SetTranslate(glm::vec3(offset * i, offset * k, offset * j));
					sceneNode->SetScale(glm::vec3(scale));
					//sceneNode->GetComponent<IModelsComponent3D>()->AddModel(((i % 2 == 0) || (j % 2 == 0) && (k % 2 == 0)) ? cubeModel : sphereModel);
					sceneNode->GetComponent<IModelsComponent3D>()->AddModel(sphereModel);

					BoundingBox bbox = BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
					bbox.transform(sceneNode->GetWorldTransfom());
					sceneNode->GetColliderComponent()->SetBounds(bbox);





					rp3d::Vector3 initPosition(offset * i + r.NextFloat() / 5.0f, offset * k, offset * j + r.NextFloat() / 5.0f);
					rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
					rp3d::Transform transform(initPosition, initOrientation);

					// Create a rigid body in the world
					rp3d::RigidBody * body;
					body = m_World.createRigidBody(transform);
					body->setType(rp3d::BodyType::DYNAMIC);
					body->getMaterial().setFrictionCoefficient(0.8);

					// Create the sphere collision shape
					float radius = rp3d::decimal(scale);
					rp3d::SphereShape * shape = new rp3d::SphereShape(radius / 2.0f);

					rp3d::ProxyShape * proxyShape;
					proxyShape = body->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(5.0));


					std::shared_ptr<CReactPhysicsComponent> component = std::make_shared<CReactPhysicsComponent>(*sceneNode, body);
					sceneNode->AddComponent(component);
				}
			}
		}
	}

	//--------------------------------------------------------------------------
	// Plane
	//--------------------------------------------------------------------------

#if 1
	{
		const float cPlaneSize = 120.0f;
		const float cPlaneY = 0.0f;

		std::shared_ptr<MaterialModel> textMaterial = std::make_shared<MaterialModel>(GetBaseManager());
		textMaterial->SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularFactor(1.0f);
		textMaterial->SetBumpFactor(8.0f);
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene\\OtherTextures\\Colors\\Orange.dds"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice().GetObjectsFactory().LoadTexture2D("Sponza_Ceiling_normal.png"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene\\BuildingTextures\\concrete_smooth_03_spec.dds"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureBump, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene\\BuildingTextures\\concrete_smooth_03_ddna.dds"));

		//std::shared_ptr<MaterialTextured> textMaterial = std::make_shared<MaterialTextured>(GetRenderDevice());
		//textMaterial->SetTexture(0, GetRenderDevice().GetObjectsFactory().LoadTexture2D("idi na huy.png"));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreatePlane());

		auto sceneNodePlane = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		sceneNodePlane->SetName("Ground");
		sceneNodePlane->SetTranslate(glm::vec3(0, cPlaneY, 0));
		sceneNodePlane->SetScale(glm::vec3(cPlaneSize));
		sceneNodePlane->GetComponent<IModelsComponent3D>()->AddModel(modelPlane);



		rp3d::Vector3 initPosition(0, cPlaneY, 0);
		rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
		rp3d::Transform transform(initPosition, initOrientation);

		// Create a rigid body in the world
		rp3d::RigidBody * body;
		body = m_World.createRigidBody(transform);
		body->setType(rp3d::BodyType::STATIC);
		body->getMaterial().setFrictionCoefficient(0.8);
		body->getMaterial().setRollingResistance(0.2);

		rp3d::BoxShape * shape = new rp3d::BoxShape(rp3d::Vector3(cPlaneSize, 0.001f, cPlaneSize));

		rp3d::ProxyShape * proxyShape;
		proxyShape = body->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(5.0));


		std::shared_ptr<CReactPhysicsComponent> component = std::make_shared<CReactPhysicsComponent>(*sceneNodePlane, body);
		sceneNodePlane->AddComponent(component);


	}
#endif



	//
	// PARTICLES
	{

		/*auto buffer = GetRenderDevice().GetObjectsFactory().CreateVertexBuffer(particles);

		auto geom = GetRenderDevice().GetObjectsFactory().CreateGeometry();
		geom->SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);
		geom->AddVertexBuffer(BufferBinding("POSITION", 0), buffer);

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(material, geom);*/



		/*

		auto material = std::make_shared<MaterialParticle>(GetRenderDevice());
		material->SetTexture(0, GetRenderDevice().GetObjectsFactory().LoadTexture2D("particle.png"));

		auto particlesNode = m_RootForBoxes->CreateSceneNode<SceneNode3D>();
		particlesNode->SetName("Particles");
		//particlesNode->GetComponent<IModelsComponent3D>()->AddModel(model);
		auto particlesComponent = std::make_shared<CParticlesComponent3D>(*particlesNode);
		particlesComponent->SetMaterial(material);
		particlesNode->AddComponent(particlesComponent);



		float areaSize = 2000.0f;
		Random r(time(0));
		for (size_t i = 0; i < 1000000; i++)
		{
			SParticle particle;
			particle.Position = glm::vec3(r.NextFloat(), r.NextFloat(), r.NextFloat()) * areaSize - glm::vec3(areaSize / 2.0f, 0.0f, areaSize / 2.0f);
			float size = r.NextFloat() * 3.0f + 1.0f;
			particle.Size = glm::vec2(size, size);
			particle.Color = glm::vec4(r.NextFloat(), r.NextFloat(), r.NextFloat(), 1.0f);
			particlesComponent->AddParticle(particle);
		}
		*/
	}

	std::shared_ptr<ISceneNode3D> fbxSceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode_FBXNode);
	std::shared_ptr<ISceneNode3D> fbxSceneNode2 = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode_FBXNode);
	fbxSceneNode2->SetTranslate(glm::vec3(10, 0, 0));
	//fbxSceneNode->SetScale(glm::vec3(15.0f, 15.0f, 15.0f));



	m_SceneCreateTypelessListPass = std::make_shared<CSceneCreateTypelessListPass>(GetRenderDevice(), shared_from_this());

	m_DefferedRenderPass = std::make_shared<CDefferedRender>(GetRenderDevice(), m_SceneCreateTypelessListPass);
	m_DefferedRenderPass->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());

	m_DefferedRenderPrepareLights = std::make_shared<CDefferedRenderPrepareLights>(GetRenderDevice(), m_SceneCreateTypelessListPass);
	m_DefferedRenderPrepareLights->CreatePipeline(nullptr, nullptr);

	m_DefferedFinalRenderPass = std::make_shared<CDefferedRenderFinal>(GetRenderDevice(), m_DefferedRenderPass, m_DefferedRenderPrepareLights);
	m_DefferedFinalRenderPass->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());

	glm::vec4 color = glm::vec4(0.0, 0.0f, 0.0f, 1.0f);
	m_Technique3D.AddPass(std::make_shared<ClearRenderTargetPass>(GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), ClearFlags::All, color /*glm::vec4(0.2f, 0.2f, 0.2f, 0.2f)*/, 1.0f, 0));
	
#if 1
	m_Technique3D.AddPass(m_SceneCreateTypelessListPass);
	m_Technique3D.AddPass(m_DefferedRenderPass);
	m_Technique3D.AddPass(m_DefferedRenderPrepareLights);
	m_Technique3D.AddPass(m_DefferedFinalRenderPass);
#else
	m_Technique3D.AddPass(std::make_shared<CMaterialPassTransperent>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_Technique3D.AddPass(std::make_shared<CMaterialPassOpaque>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
#endif

	//m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("TexturedMaterialPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	//m_Technique3D.AddPass(std::make_shared<CMaterialParticlePass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}

void CSceneDefault::LoadUI()
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

	m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}


void CSceneDefault::GenerateLights(std::shared_ptr<ISceneNode3D> Node, uint32_t numLights)
{
	float MinRange = 100.1f;
	float MaxRange = 2000.0f;

	float MinSpotAngle = 1.0f;
	float MaxSpotAngle = 60.0f;

	glm::vec3 BoundsMin = glm::vec3(-500, -200, -500);
	glm::vec3 BoundsMax = glm::vec3(500, 200, 500);
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

void CSceneDefault::UpdateLights()
{

}