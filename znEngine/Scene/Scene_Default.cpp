#include "stdafx.h"

// Gerenal
#include "Scene_Default.h"

// Additional
#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"
#include "Materials/MaterialParticle.h"
#include "Materials/MaterialModel.h"

#include "Passes/Renderer/RendererDeffered.h"
#include "Passes/Renderer/RendererForward.h"

#include "Scene/Camera/FreeCameraController.h"

#include "Scene/Components/ReactPhysicsComponent.h"
#include "Scene/Components/Skeleton/SkeletonComponent.h"
#include "Scene/Components/Skeleton/AnimatorComponent.h"

#include "FBX/FBXInterfaces.h"

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
	Log::Info("Scene destroyed.");
}


//
// IGameState
//
void CSceneDefault::Initialize()
{
	SceneBase::Initialize();

	// Light
	{
		auto lightNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		lightNode->SetName("Light");
		lightNode->SetTranslate(glm::vec3(-300.0f, 500.0f, -500.0f) / 3.0f);
		lightNode->SetRotation(glm::vec3(0.5f, -0.5f, 0.5f));

		lightNode->AddComponent(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *lightNode.get()));
		lightNode->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		lightNode->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightNode->GetComponent<ILightComponent3D>()->SetRange(5000.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetSpotlightAngle(45.0f);
	}

	// Light
	/*{
		auto lightNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		lightNode->SetName("Light2");
		lightNode->SetTranslate(glm::vec3(300.0f, 500.0f, 500.0f) / 5.0f);
		lightNode->SetRotation(glm::vec3(-0.5f, -0.5f, -0.5f));

		lightNode->AddComponent(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *lightNode.get()));
		lightNode->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		lightNode->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightNode->GetComponent<ILightComponent3D>()->SetRange(3000.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetSpotlightAngle(45.0f);
	}*/

	// Camera
	{
		auto cameraNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		cameraNode->SetName("Camera");
		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateBBox();
		
		/*auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(1.0f, 1.0f, 0.3f, 1.0f));
		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);
		cameraNode->GetComponent<IModelsComponent3D>()->SetModel(model);*/

		auto cameraComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode);
		cameraNode->AddComponent(cameraComponent);

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 75.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 1.0f, 5000.0f);
		GetCameraController()->GetCamera()->SetTranslation(glm::vec3(15.0f * 2.0f));
		GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
		GetCameraController()->GetCamera()->SetYaw(225);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}

	auto newRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, GetRootNode3D());
	newRoot->SetName("NewRoot3D");





	auto file = GetBaseManager().GetManager<IFilesManager>()->Open("SceneFBX.xml");
	if (file != nullptr)
	{
		CXMLManager xml;
		auto reader = xml.CreateReader(file);

		auto currentRoot = newRoot;
		while (false == currentRoot->GetChilds().empty())
			currentRoot->RemoveChild(currentRoot->GetChilds()[0]);

		auto rootNodeXML = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->LoadSceneNode3DXML(reader->GetChilds()[0], currentRoot->GetScene(), currentRoot->GetParent().lock());





		while (false == rootNodeXML->GetChilds().empty())
			currentRoot->AddChild(rootNodeXML->GetChilds()[0]);
	}

	rp3d::Vector3 gravity(0.0, -9.81, 0.0);
	// Create the dynamics world
	rp3d::DynamicsWorld world(gravity);


	//--------------------------------------------------------------------------
	// Plane
	//--------------------------------------------------------------------------

	{
		const float cPlaneSize = 240.0f;
		const float cPlaneY = 0.0f;

		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());

		auto sceneNodePlane = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, newRoot);
		sceneNodePlane->SetName("Ground");
		sceneNodePlane->SetTranslate(glm::vec3(0, -5, 0));
		sceneNodePlane->SetScale(glm::vec3(cPlaneSize, 5.0f, cPlaneSize));
		sceneNodePlane->GetComponent<IModelsComponent3D>()->SetModel(modelPlane);
		//sceneNodePlane->GetComponent<IModelsComponent3D>()->SetCastShadows(false);
	}



	//--------------------------------------------------------------------------
	// Sphere Metal
	//--------------------------------------------------------------------------

	{
		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		//textMaterial->SetSpecularFactor(8.0f);
		//textMaterial->SetBumpFactor(8.0f);
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice().GetObjectsFactory().LoadTexture2D("beaten-up-metal1-unity//beaten-up-metal1-albedo.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice().GetObjectsFactory().LoadTexture2D("beaten-up-metal1-unity//beaten-up-metal1-Normal-ogl.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice().GetObjectsFactory().LoadTexture2D("beaten-up-metal1-unity//beaten-up-metal1-ao.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetRenderDevice().GetObjectsFactory().LoadTexture2D("beaten-up-metal1-unity//beaten-up-metal1-Height.png"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureBump, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_ddna.dds"));

		//std::shared_ptr<MaterialTextured> textMaterial = MakeShared(MaterialTextured, GetRenderDevice());
		//textMaterial->SetTexture(0, GetRenderDevice().GetObjectsFactory().LoadTexture2D("idi na huy.png"));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateSphere());

		auto sceneNodePlane = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, newRoot);
		sceneNodePlane->SetName("Sphere");
		sceneNodePlane->SetTranslate(glm::vec3(-10, 15, -10));
		sceneNodePlane->SetScale(glm::vec3(15.0f));
		sceneNodePlane->GetComponent<IModelsComponent3D>()->SetModel(modelPlane);
		//sceneNodePlane->GetComponent<IModelsComponent3D>()->SetCastShadows(false);
	}

	//--------------------------------------------------------------------------
	// Cube Gold
	//--------------------------------------------------------------------------

	for (int i = 0; i < 5; i++)
	{
		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		//textMaterial->SetSpecularFactor(8.0f);
		//textMaterial->SetBumpFactor(16.0f);
		
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice().GetObjectsFactory().LoadTexture2D("oglTutor//bricks2.png"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice().GetObjectsFactory().LoadTexture2D("oglTutor//bricks2_normal.png"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice().GetObjectsFactory().LoadTexture2D("pirate-gold-unity//pirate-gold_ao.png"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetRenderDevice().GetObjectsFactory().LoadTexture2D("oglTutor//bricks2_disp.png"));

		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice().GetObjectsFactory().LoadTexture2D("pirate-gold-unity//pirate-gold_albedo.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice().GetObjectsFactory().LoadTexture2D("pirate-gold-unity//pirate-gold_normal-ogl.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice().GetObjectsFactory().LoadTexture2D("pirate-gold-unity//pirate-gold_ao.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetRenderDevice().GetObjectsFactory().LoadTexture2D("pirate-gold-unity//pirate-gold_height.png"));

		//std::shared_ptr<MaterialTextured> textMaterial = MakeShared(MaterialTextured, GetRenderDevice());
		//textMaterial->SetTexture(0, GetRenderDevice().GetObjectsFactory().LoadTexture2D("idi na huy.png"));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());

		auto sceneNodePlane = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, newRoot);
		sceneNodePlane->SetName("Sphere2");
		sceneNodePlane->SetTranslate(glm::vec3(-10, 15, 16 * i));
		sceneNodePlane->SetScale(glm::vec3(15.0f));
		sceneNodePlane->GetComponent<IModelsComponent3D>()->SetModel(modelPlane);
		//sceneNodePlane->GetComponent<IModelsComponent3D>()->SetCastShadows(false);
	}


	//Load3D();

	auto forwardRenderer = MakeShared(CRendererForward, GetBaseManager(), weak_from_this());
	forwardRenderer->Initialize(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
	m_ForwardRenderer = forwardRenderer;
	//SetRenderer(forwardRenderer);

	auto defferedRenderer = MakeShared(CRendererDeffered, GetBaseManager(), weak_from_this());
	defferedRenderer->Initialize(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
	m_DefferedRenderrer = defferedRenderer;

	SetRenderer(defferedRenderer);
}

void CSceneDefault::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}

bool CSceneDefault::OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld)
{
	if (e.Button == MouseButtonEventArgs::MouseButton::Left)
	{
		//sceneNodeParentt->SetTranslate(GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 25.0f)));
		return true;
	}
	return false;
}

void CSceneDefault::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
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
	if (e.Key == KeyCode::R)
	{
		SetRenderer(m_DefferedRenderrer);
	}
	else if (e.Key == KeyCode::T)
	{
		SetRenderer(m_ForwardRenderer);
	}
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
	// Cubes & Spheres
	//--------------------------------------------------------------------------

	Random r(0);


	{
		const int iterCnt = 0;
		const float offset = 13.0f;
		const float scale = 5.0f;

		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularFactor(4.0f);
		textMaterial->SetBumpFactor(8.0f);
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_diff.dds"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice().GetObjectsFactory().LoadTexture2D("Sponza_Ceiling_normal.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_spec.dds"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureBump, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_ddna.dds"));

		//std::shared_ptr<MaterialTextured> textMaterial = MakeShared(MaterialTextured, GetRenderDevice());
		//textMaterial->SetTexture(0, GetRenderDevice().GetObjectsFactory().LoadTexture2D("Sponza_Floor_diffuse.png"));

		auto cubeModel = GetRenderDevice().GetObjectsFactory().CreateModel();
		cubeModel->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());
	
		auto sphereModel = GetRenderDevice().GetObjectsFactory().CreateModel();
		sphereModel->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateSphere());


		m_RootForBoxes = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, GetRootNode3D());
		//m_RootForBoxes->SetTranslate(glm::vec3(150, 0, 150));


		for (int i = -(iterCnt / 2); i < iterCnt / 2; i++)
		{
			for (int j = -(iterCnt / 2); j < iterCnt / 2; j++)
			{
				for (int k = 0; k < iterCnt; k++)
				{
					auto sceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, m_RootForBoxes);
					sceneNode->SetName("Ball [" + std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + "]");
					sceneNode->SetTranslate(glm::vec3(offset * i, offset * k, offset * j));
					sceneNode->SetScale(glm::vec3(scale));
					//sceneNode->GetComponent<IModelsComponent3D>()->AddModel(((i % 2 == 0) || (j % 2 == 0) && (k % 2 == 0)) ? cubeModel : sphereModel);
					sceneNode->GetComponent<IModelsComponent3D>()->SetModel(sphereModel);

					BoundingBox bbox = BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
					bbox.transform(sceneNode->GetWorldTransfom());
					sceneNode->GetComponent<IColliderComponent3D>()->SetBounds(bbox);

#if 0
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


					std::shared_ptr<CReactPhysicsComponent> component = MakeShared(CReactPhysicsComponent, *sceneNode, body);
					sceneNode->AddComponent(component);
#endif
				}
			}
		}
	}

	//--------------------------------------------------------------------------
	// Plane
	//--------------------------------------------------------------------------

#if 0
	{
		const float cPlaneSize = 120.0f;
		const float cPlaneY = -50.0f;

		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularFactor(1.0f);
		textMaterial->SetBumpFactor(8.0f);
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene//OtherTextures//Colors//Orange.dds"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice().GetObjectsFactory().LoadTexture2D("Sponza_Ceiling_normal.png"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_spec.dds"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureBump, GetRenderDevice().GetObjectsFactory().LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_ddna.dds"));

		//std::shared_ptr<MaterialTextured> textMaterial = MakeShared(MaterialTextured, GetRenderDevice());
		//textMaterial->SetTexture(0, GetRenderDevice().GetObjectsFactory().LoadTexture2D("idi na huy.png"));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreatePlane());

		auto sceneNodePlane = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		sceneNodePlane->SetName("Ground");
		sceneNodePlane->SetTranslate(glm::vec3(0, cPlaneY, 0));
		sceneNodePlane->SetScale(glm::vec3(cPlaneSize));
		sceneNodePlane->GetComponent<IModelsComponent3D>()->SetModel(modelPlane);

		rp3d::Vector3 initPosition(0, cPlaneY, 0);
		rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
		rp3d::Transform transform(initPosition, initOrientation);

		// Create a rigid body in the world

#if 0
		rp3d::RigidBody * body;
		body = m_World.createRigidBody(transform);
		body->setType(rp3d::BodyType::STATIC);
		body->getMaterial().setFrictionCoefficient(0.8);
		body->getMaterial().setRollingResistance(0.2);

		rp3d::BoxShape * shape = new rp3d::BoxShape(rp3d::Vector3(cPlaneSize, 0.001f, cPlaneSize));

		rp3d::ProxyShape * proxyShape;
		proxyShape = body->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(5.0));

		std::shared_ptr<CReactPhysicsComponent> component = MakeShared(CReactPhysicsComponent, *sceneNodePlane, body);
		sceneNodePlane->AddComponent(component);
#endif
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

		auto material = MakeShared(MaterialParticle, GetRenderDevice());
		material->SetTexture(0, GetRenderDevice().GetObjectsFactory().LoadTexture2D("particle.png"));

		auto particlesNode = m_RootForBoxes->CreateSceneNode<SceneNode3D>();
		particlesNode->SetName("Particles");
		//particlesNode->GetComponent<IModelsComponent3D>()->AddModel(model);
		auto particlesComponent = MakeShared(CParticlesComponent3D, *particlesNode);
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


	//m_World.raycast(reactphysics3d::Ray(), reactphysics3d::RaycastCallback);

#if 0

	std::shared_ptr<ISceneNode3D> sceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
	
	// Models
	auto fbxSceneModel = GetBaseManager().GetManager<IFBXManager>()->LoadFBX("C:/Users/Alexander/Downloads/Assets/Toon_RTS/Orcs/models/Single_Mesh/Orc_SM_shaman.FBX");
	DoAddModels(sceneNode, fbxSceneModel->GetFBXRootNode());

	// Animator
	sceneNode->AddComponent<ISkeletonAnimationComponent>(MakeShared(CAnimatorComponent3D, *sceneNode));
	uint16 cntr = 0;
	auto fbxSceneAnim = GetBaseManager().GetManager<IFBXManager>()->LoadFBX("C:/Users/Alexander/Downloads/Assets/Toon_RTS/Orcs/animation/shaman/Orc_shaman_01_idle_A.FBX");
	for (const auto& anim : fbxSceneAnim->GetFBXAnimation()->GetAnimations())
		if (sceneNode->GetComponent<ISkeletonAnimationComponent>())
			sceneNode->GetComponent<ISkeletonAnimationComponent>()->AddAnimation(cntr++, anim);

	// Skeleton
	//auto skeletonFromModel = fbxSceneModel->GetSkeleton()->GetSkeleton();
	auto skeletonFromAnim = fbxSceneAnim->GetFBXSkeleton()->GetSkeleton();
	//skeletonFromModel.MergeWithOther(skeletonFromAnim);

	// Skeleton component
	sceneNode->AddComponent<ISkeletonComponent3D>(MakeShared(CSkeletonComponent3D, *sceneNode, skeletonFromAnim));
#endif

	//std::shared_ptr<IFBXNode> fbxSceneNode = std::dynamic_pointer_cast<IFBXNode>(sceneNode);
	//fbxSceneNode->InitializeFromFile("C:/Users/Alexander/Downloads/Assets/Toon_RTS/Orcs/animation/shaman/Orc_shaman_02_walk.FBX");
	//fbxSceneNode->InitializeFromFile("C:/Users/Alexander/Downloads/Assets/Toon_RTS/Orcs/models/Single_Mesh/Orc_SM_shaman.FBX");
	
	auto defferedRenderer = MakeShared(CRendererDeffered, GetBaseManager(), weak_from_this());
	defferedRenderer->Initialize(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
	SetRenderer(defferedRenderer);

	//auto forwardRenderer = MakeShared(CRendererForward, GetBaseManager(), weak_from_this());
	//forwardRenderer->Initialize(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
	//SetRenderer(forwardRenderer);

	//m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("TexturedMaterialPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	//m_Technique3D.AddPass(MakeShared(CMaterialParticlePass, GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}
