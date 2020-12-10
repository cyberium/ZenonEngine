#include "stdafx.h"

// Gerenal
#include "SceneRTS.h"


// Additional
#include "../znPluginFBXModels/FBXInterfaces.h"


CSceneRTS::CSceneRTS(IBaseManager& BaseManager, IRenderWindow& RenderWindow)
	: SceneBase(BaseManager, RenderWindow)
	, m_RTSCurrentWave(0)
	, m_RTSCurrentWaveUnit(0)
	, m_LastUnitTime(0.0f)

	// Tower creation
	, m_CurrentTowerNode(nullptr)
{

}

CSceneRTS::~CSceneRTS()
{
	Log::Info("SceneRTS destroyed");
}


//
// IGameState
//
void CSceneRTS::Initialize()
{
	SceneBase::Initialize();

	// Light
	/*{
		auto lightNode = CreateSceneNodeT<ISceneNode>();
		lightNode->SetName("Light");
		lightNode->SetTranslate(glm::vec3(-300.0f, 500.0f, -500.0f) / 3.0f);
		lightNode->SetRotation(glm::vec3(0.5f, -0.5f, 0.5f));

		lightNode->AddComponent(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *lightNode.get()));
		lightNode->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		lightNode->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightNode->GetComponent<ILightComponent3D>()->SetRange(500.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetIntensity(1.1f);
		lightNode->GetComponent<ILightComponent3D>()->SetSpotlightAngle(45.0f);
	}*/

	std::shared_ptr<IParticleComponent3D> particles = MakeShared(CParticlesComponent, *GetRootSceneNode());
	GetRootSceneNode()->AddComponentT(particles);

	

	// Light
	{
		auto lightNode = CreateSceneNodeT<ISceneNode>();
		lightNode->SetName("Light2");
		lightNode->SetLocalPosition(glm::vec3(150.0f, 150.0f, 150.0f));
		lightNode->SetRotationEuler(glm::vec3(-0.5f, -0.85f, -0.5f));

		auto lightComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<CLightComponent>(cSceneNodeLightComponent, *lightNode.get());
		lightComponent->SetCastShadows(true);
		lightComponent->SetType(ELightType::Directional);
		lightComponent->SetAmbientColor(glm::vec3(0.25f));
		lightComponent->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightComponent->SetRange(1000.0f);
		lightComponent->SetIntensity(1.0077f);
		lightComponent->SetSpotlightAngle(30.0f);

		lightNode->AddComponent(cSceneNodeLightComponent, lightComponent);
	}

	// Camera
	{
		float aspect = static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight());

		auto cameraNode = CreateSceneNodeT<ISceneNode>();
		cameraNode->SetName("Camera");
		cameraNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponentT<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 5000.0f);
		//GetCameraController()->GetCamera()->SetOrthographicProjection(aspect, -55, 55, 55, -55, -10.0f, 1.0f);
		
		GetCameraController()->GetCamera()->SetTranslation(glm::vec3(0.0f));
		GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f, -0.5f, -0.5f));
		//GetCameraController()->GetCamera()->SetYaw(0);
		//GetCameraController()->GetCamera()->SetPitch(-45);
	}


	//--------------------------------------------------------------------------
	// Sphere
	//--------------------------------------------------------------------------
	{
		const float cPlaneSize = 75.0f;
		const float cPlaneY = 0.0f;

		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetEnviorementMappingEnable(true);

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateSphere());

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
		node->SetName("Sphere");
		node->SetLocalPosition(glm::vec3(0, 175.0f, 0));
		node->SetScale(glm::vec3(cPlaneSize, cPlaneSize, cPlaneSize));
		node->GetComponentT<IModelComponent>()->SetModel(modelPlane);
		//node->GetComponent<IModelComponent>()->SetCastShadows(false);
	}







	//--------------------------------------------------------------------------
	// XML
	//--------------------------------------------------------------------------
	if (auto file = GetBaseManager().GetManager<IFilesManager>()->Open("RTS2265612"))
	{
		CXMLManager xml(GetBaseManager());
		auto reader = xml.CreateReaderFromFile(file);
		auto rootNodeXML = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->LoadSceneNode3DXML(reader->GetChilds()[0], *this);

		m_RTSUnitsPath = std::dynamic_pointer_cast<ISceneNodeRTSPath>(rootNodeXML->GetChild("Waypoint"));
		_ASSERT(m_RTSUnitsPath != nullptr);
	}



	{
		auto uiControlRTSResourcesPanel = CreateUIControlTCast<CUIControlRTSResourcesPanel>();

		uiControlRTSResourcesPanel->SetLocalPosition(glm::vec2((GetRenderWindow().GetWindowWidth() / 2.0f) - (uiControlRTSResourcesPanel->GetSize().x / 2.0f), 0.0f));
		uiControlRTSResourcesPanel->SetScale(glm::vec2(0.5f));
	}

	{
		m_UIControlRTSTowersPanel = CreateUIControlTCast<CUIControlRTSTowersPanel>();
		m_UIControlRTSTowersPanel->AddTowerButton("Tower 1", "sceneNodesProtos/tower1.znobj", "sceneNodesProtos/tower1.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower 2", "sceneNodesProtos/tower2.znobj", "sceneNodesProtos/tower2.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower A", "sceneNodesProtos/towerA.znobj", "sceneNodesProtos/towerA.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower B", "sceneNodesProtos/towerB.znobj", "sceneNodesProtos/towerB.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower C", "sceneNodesProtos/towerC.znobj", "sceneNodesProtos/towerC.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower E", "sceneNodesProtos/towerE.znobj", "sceneNodesProtos/towerE.png", 100, *this);

		m_UIControlRTSTowersPanel->SetLocalPosition(glm::vec2(
			(GetRenderWindow().GetWindowWidth()         -  m_UIControlRTSTowersPanel->GetSize().x),
			 GetRenderWindow().GetWindowHeight() / 2.0f - (m_UIControlRTSTowersPanel->GetSize().y / 2.0f)
		));

		m_UIControlRTSTowersPanel->SetTowerButtonClickCallback(std::bind(&CSceneRTS::OnTowerButtonClicked, this, std::placeholders::_1));
		//m_UIControlRTSTowersPanel->SetScale(glm::vec2(0.5f));
	}
	


	CreateUnitsModels();
}

void CSceneRTS::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}

void CSceneRTS::OnUpdate(UpdateEventArgs & e)
{
	__super::OnUpdate(e);

	if (m_RTSCurrentWave >= m_RTSWaves.size())
	{
		m_RTSCurrentWave = 0;
		return;
	}

	const auto& currentWave = m_RTSWaves.at(m_RTSCurrentWave);

	if (m_LastUnitTime + currentWave.IndervalMS > e.TotalTime)
		return;
	
	// After last unit of wave created, change wave
	if (m_RTSCurrentWaveUnit >= m_RTSWaves.at(m_RTSCurrentWave).Count)
	{
		m_RTSCurrentWaveUnit = 0;
		m_RTSCurrentWave += 1;
	}

	if (m_RTSCurrentWave >= m_RTSWaves.size())
		m_RTSCurrentWave = 0;

	CreateUnit();

	m_LastUnitTime = e.TotalTime;
}

bool CSceneRTS::OnMousePressed(const MouseButtonEventArgs& e, const Ray& RayToWorld)
{
	if (e.Button == MouseButton::Left)
	{
		CreateTower();
	}
	else if (e.Button == MouseButton::Right)
	{
		CancelTower();
	}
	return __super::OnMousePressed(e, RayToWorld);
}

void CSceneRTS::OnMouseMoved(const MouseMotionEventArgs& e, const Ray & RayToWorld)
{
	MoveTower(RayToWorld);

	__super::OnMouseMoved(e, RayToWorld);
}



//
// Keyboard events
//
bool CSceneRTS::OnWindowKeyPressed(KeyEventArgs & e)
{
	return SceneBase::OnWindowKeyPressed(e);
}

void CSceneRTS::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}



//
// Protected
//
void CSceneRTS::CreateUnitsModels()
{
	{
		SRTSWave wave0;
		wave0.Model = CreateUnitModel("Toon_RTS/models/WK_archer.FBX", "Toon_RTS/animation/archer/WK_archer_03_run.FBX", "Toon_RTS/animation/archer/WK_archer_10_death_A.FBX");
		wave0.Count = 2;
		wave0.IndervalMS = 1500;
		m_RTSWaves.push_back(wave0);
	}

	{
		SRTSWave wave1;
		wave1.Model = CreateUnitModel("Toon_RTS/models/WK_catapult.FBX", "Toon_RTS/animation/catapult/WK_catapult_02_move.FBX", "Toon_RTS/animation/catapult/WK_catapult_04_death.FBX");
		wave1.Count = 2;
		wave1.IndervalMS = 1500;
		m_RTSWaves.push_back(wave1);
	}

	{
		SRTSWave wave2;
		wave2.Model = CreateUnitModel("Toon_RTS/models/WK_cavalry.FBX", "Toon_RTS/animation/cavalry/WK_cavalry_sword_02_walk.FBX", "Toon_RTS/animation/cavalry/WK_cavalry_sword_09_death_A.FBX");
		wave2.Count = 2;
		wave2.IndervalMS = 1500;
		m_RTSWaves.push_back(wave2);
	}

	{
		SRTSWave wave3;
		wave3.Model = CreateUnitModel("Toon_RTS/models/WK_heavy_infantry.FBX", "Toon_RTS/animation/heavy_infantry/WK_heavy_infantry_03_run.FBX", "Toon_RTS/animation/heavy_infantry/WK_heavy_infantry_09_death_A.FBX");
		wave3.Count = 2;
		wave3.IndervalMS = 1500;
		m_RTSWaves.push_back(wave3);
	}

	{
		SRTSWave wave4;
		wave4.Model = CreateUnitModel("Toon_RTS/models/WK_worker.FBX", "Toon_RTS/animation/worker/WK_worker_03_run.FBX", "Toon_RTS/animation/worker/WK_worker_05_death_A.FBX");
		wave4.Count = 2;
		wave4.IndervalMS = 1500;
		m_RTSWaves.push_back(wave4);
	}
}

std::shared_ptr<IModel> CSceneRTS::CreateUnitModel(std::string ModelName, std::string RunAnimationName, std::string DeathAnimationName)
{
	auto filesManager = GetBaseManager().GetManager<IFilesManager>();

	/*auto fileNameStruct = Utils::SplitFilename(ModelName);
	auto existingXMLModel = fileNameStruct.NameWithoutExtension + ".znxmdl";
	if (filesManager->IsFileExists(existingXMLModel))
		return GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(existingXMLModel);*/

	CznFBXLoaderParams fbxLoaderParams;
	fbxLoaderParams.TexturesPathRoot = "Toon_RTS/models/textures/";
	fbxLoaderParams.OverrideTextureByMaterial["WK_StandardUnits_Blue"] = "WK_StandardUnits_Blue.png";
	fbxLoaderParams.OverrideTextureByMaterial["WK_Horse"] = "WK_Horse_A.png";

	auto fbxModelsLoader = GetBaseManager().GetManager<IznModelsFactory>()->GetLoaderForModel("fbx");
	_ASSERT(fbxModelsLoader != nullptr);
	auto fbxSceneLoader = std::dynamic_pointer_cast<IFBXSceneLoader>(fbxModelsLoader);
	_ASSERT(fbxSceneLoader != nullptr);

	// Original skeleton
	std::shared_ptr<IFile> modelFile = filesManager->Open(ModelName);
	auto originalSkeletonModel = fbxSceneLoader->LoadScene(modelFile, &fbxLoaderParams)->MergeModels();
	
	// Fix materials
	for (const auto& connection : originalSkeletonModel->GetConnections())
	{
		std::dynamic_pointer_cast<MaterialModel>(connection.Material)->SetDiffuseColor(glm::vec3(1.0f));
		std::dynamic_pointer_cast<MaterialModel>(connection.Material)->SetDiffuseFactor(1.0f);
	}

	// Animated skeleton
	std::shared_ptr<IFile> animationFile = filesManager->Open(RunAnimationName);
	auto animatedSkeletonModel = fbxSceneLoader->LoadScene(animationFile, &fbxLoaderParams)->MergeModels();
	for (const auto& anim : animatedSkeletonModel->GetAnimations())
		originalSkeletonModel->AddAnimation("run", anim.second);

	auto animatedSkeletonModel2 = fbxSceneLoader->LoadScene(DeathAnimationName, &fbxLoaderParams)->MergeModels();
	for (const auto& anim : animatedSkeletonModel2->GetAnimations())
		originalSkeletonModel->AddAnimation("death", anim.second);

	std::shared_ptr<IFile> modelResult = nullptr;

	{
		std::string znmdlFilename = modelFile->Name_NoExtension() + ".znmdl";
		auto znMdlFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(originalSkeletonModel, znmdlFilename);
		znMdlFile->Save();

		//modelResult = znMdlFile;
	}

	{
		std::string znxmdlFilename = modelFile->Name_NoExtension() + ".znxmdl";
		auto znMdlXMLFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(originalSkeletonModel, znxmdlFilename);
		znMdlXMLFile->Save();

		modelResult = znMdlXMLFile;
	}

	try
	{
		return GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(modelResult);
	}
	catch (const CException& e)
	{
		Log::Error("Error while loading converted model.");
		Log::Error("--->%s", e.MessageCStr());
		throw;
	}
}

void CSceneRTS::CreateUnit()
{


	const auto& currentWave = m_RTSWaves.at(m_RTSCurrentWave);

	auto newRTSUnit = CreateSceneNodeCast<ISceneNodeRTSUnit>(cSceneNodeRTSUnit);
	newRTSUnit->SetName("RTSUnit");
	newRTSUnit->SetLocalPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	newRTSUnit->SetScale(glm::vec3(0.08f));
	newRTSUnit->GetComponentT<IModelComponent>()->SetModel(currentWave.Model);
	try
	{
		newRTSUnit->GetComponentT<IModelComponent>()->PlayAnimation("run", true);
	}
	catch (const CException& e)
	{

	}
	newRTSUnit->SetPath(m_RTSUnitsPath);

	m_RTSCurrentWaveUnit++;
}

void CSceneRTS::CreateTower()
{
	if (m_CurrentTowerNode == nullptr)
		return;

	auto createdTower = std::dynamic_pointer_cast<ISceneNode>(m_CurrentTowerNode->Copy());

	m_CurrentTowerNode->MakeMeOrphan();
	m_CurrentTowerNode.reset();
}

void CSceneRTS::CancelTower()
{
	if (m_CurrentTowerNode == nullptr)
		return;

	m_CurrentTowerNode->MakeMeOrphan();
	m_CurrentTowerNode.reset();
}

void CSceneRTS::MoveTower(const Ray& RayToWorld)
{
	if (m_CurrentTowerNode == nullptr)
		return;

	auto nodes = GetFinder().FindIntersection(RayToWorld, [](const std::shared_ptr<ISceneNode>& Node) -> bool {
		return Node->GetName() == "tile";
	}, GetRootSceneNode());
	if (nodes.empty())
		return;

	auto nearestNode = nodes.begin()->second;
	_ASSERT(nearestNode != nullptr);

	auto nearestTileBounds = nearestNode->GetComponentT<IColliderComponent>();
	auto bounds = nearestTileBounds->GetWorldBounds();
	glm::vec3 center = bounds.getCenter();

	if (auto collider = m_CurrentTowerNode->GetComponentT<IColliderComponent>())
	{
		const auto& colliderBounds = collider->GetBounds();
		if (false == colliderBounds.IsInfinite())
		{
			//auto pos = GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), collider->GetBounds().getCenter().y));
			m_CurrentTowerNode->SetLocalPosition(center);
			return;
		}
	}

	if (center.y != 1.0f)
		printf("s");

	//auto pos = GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));
	m_CurrentTowerNode->SetPosition(center);
}

namespace
{
	void ChangeMaterialRecursive(std::shared_ptr<IMaterial> Material, std::shared_ptr<ISceneNode> Node)
	{
		if (auto modelsComponent = Node->GetComponentT<IModelComponent>())
		{
			if (auto model = modelsComponent->GetModel())
			{
				for (auto& connection : model->GetConnections())
				{
					//connection.Material = Material;
				}
			}
		}
	}
}

bool CSceneRTS::OnTowerButtonClicked(const STowerDescription& TowerDesription)
{
	if (m_CurrentTowerNode)
		m_CurrentTowerNode->MakeMeOrphan();

	m_CurrentTowerNode = std::dynamic_pointer_cast<ISceneNode>(TowerDesription.SceneNode->Copy());
	

	return true;
}
