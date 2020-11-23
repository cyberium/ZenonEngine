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

	// Light
	{
		auto lightNode = CreateSceneNodeT<ISceneNode>();
		lightNode->SetName("Light2");
		lightNode->SetTranslate(glm::vec3(550.0f, 550.0f, 550.0f));
		lightNode->SetRotation(glm::vec3(-0.5f, -0.5f, -0.5f));

		lightNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *lightNode.get()));
		lightNode->GetComponentT<ILightComponent3D>()->SetType(ELightType::Spot);
		lightNode->GetComponentT<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightNode->GetComponentT<ILightComponent3D>()->SetRange(1350.0f);
		lightNode->GetComponentT<ILightComponent3D>()->SetIntensity(1.0f);
		lightNode->GetComponentT<ILightComponent3D>()->SetSpotlightAngle(30.0f);
	}

	// Camera
	{
		auto cameraNode = CreateSceneNodeT<ISceneNode>();
		cameraNode->SetName("Camera");
		cameraNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponentT<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 5000.0f);
		GetCameraController()->GetCamera()->SetTranslation(glm::vec3(100.0f));
		GetCameraController()->GetCamera()->SetYaw(225);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}

	//--------------------------------------------------------------------------
	// XML
	//--------------------------------------------------------------------------
	if (auto file = GetBaseManager().GetManager<IFilesManager>()->Open("RTS22656"))
	{
		CXMLManager xml(GetBaseManager());
		auto reader = xml.CreateReader(file);
		auto rootNodeXML = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->LoadSceneNode3DXML(reader->GetChilds()[0], *this);

		m_RTSUnitsPath = std::dynamic_pointer_cast<ISceneNodeRTSPath>(rootNodeXML->GetChild("Waypoint"));
		_ASSERT(m_RTSUnitsPath != nullptr);
	}


	{
		m_UIControlRTSTowersPanel = CreateUIControlTCast<CUIControlRTSTowersPanel>();
		m_UIControlRTSTowersPanel->AddTowerButton("Tower 1", "sceneNodesProtos/tower1.xml", "sceneNodesProtos/tower1.png", 100);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower 2", "sceneNodesProtos/tower2.xml", "sceneNodesProtos/tower2.png", 100);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower A", "sceneNodesProtos/towerA.xml", "sceneNodesProtos/towerA.png", 100);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower B", "sceneNodesProtos/towerB.xml", "sceneNodesProtos/towerB.png", 100);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower C", "sceneNodesProtos/towerC.xml", "sceneNodesProtos/towerC.png", 100);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower E", "sceneNodesProtos/towerE.xml", "sceneNodesProtos/towerE.png", 100);

		m_UIControlRTSTowersPanel->SetTranslate(glm::vec2(
			(GetRenderWindow().GetWindowWidth() / 2.0f) - (m_UIControlRTSTowersPanel->GetSize().x / 2.0f),
			 GetRenderWindow().GetWindowHeight() - m_UIControlRTSTowersPanel->GetSize().y)
		);

		m_UIControlRTSTowersPanel->SetTowerButtonClickCallback(std::bind(&CSceneRTS::OnTowerButtonClicked, this, std::placeholders::_1));


		//commonControl->SetScale(glm::vec2(0.75f));
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
		return;

	const auto& currentWave = m_RTSWaves.at(m_RTSCurrentWave);

	if (m_LastUnitTime + currentWave.IndervalMS > e.TotalTime)
		return;

	CreateUnit();

	m_LastUnitTime = e.TotalTime;
}

bool CSceneRTS::OnMousePressed(const MouseButtonEventArgs& e, const Ray& RayToWorld)
{
	return __super::OnMousePressed(e, RayToWorld);
}

void CSceneRTS::OnMouseMoved(const MouseMotionEventArgs& e, const Ray & RayToWorld)
{
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
		wave0.Count = 5;
		wave0.IndervalMS = 1500;
		m_RTSWaves.push_back(wave0);
	}

	{
		SRTSWave wave1;
		wave1.Model = CreateUnitModel("Toon_RTS/models/WK_catapult.FBX", "Toon_RTS/animation/catapult/WK_catapult_02_move.FBX", "Toon_RTS/animation/catapult/WK_catapult_04_death.FBX");
		wave1.Count = 5;
		wave1.IndervalMS = 1500;
		m_RTSWaves.push_back(wave1);
	}

	{
		SRTSWave wave2;
		wave2.Model = CreateUnitModel("Toon_RTS/models/WK_cavalry.FBX", "Toon_RTS/animation/cavalry/WK_cavalry_sword_03_run.FBX", "Toon_RTS/animation/cavalry/WK_cavalry_sword_09_death_A.FBX");
		wave2.Count = 5;
		wave2.IndervalMS = 1500;
		m_RTSWaves.push_back(wave2);
	}

	{
		SRTSWave wave3;
		wave3.Model = CreateUnitModel("Toon_RTS/models/WK_heavy_infantry.FBX", "Toon_RTS/animation/heavy_infantry/WK_heavy_infantry_03_run.FBX", "Toon_RTS/animation/heavy_infantry/WK_heavy_infantry_09_death_A.FBX");
		wave3.Count = 5;
		wave3.IndervalMS = 1500;
		m_RTSWaves.push_back(wave3);
	}


}

std::shared_ptr<IModel> CSceneRTS::CreateUnitModel(std::string ModelName, std::string RunAnimationName, std::string DeathAnimationName)
{
	auto filesManager = GetBaseManager().GetManager<IFilesManager>();

	std::shared_ptr<IFile> modelFile = filesManager->Open(ModelName);
	std::shared_ptr<IFile> animationFile = filesManager->Open(RunAnimationName);

	//auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
	//node->SetName(modelFile->Name_NoExtension());
	//node->SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
	//node->SetScale(glm::vec3(0.33f));

	CznFBXLoaderParams fbxLoaderParams;
	fbxLoaderParams.TexturesPathRoot = "Toon_RTS/models/textures/";
	fbxLoaderParams.OverrideTexture = "WK_StandardUnits_Blue.png";

	auto fbxModelsLoader = GetBaseManager().GetManager<IznModelsFactory>()->GetLoaderForModel("fbx");
	_ASSERT(fbxModelsLoader != nullptr);
	auto fbxSceneLoader = std::dynamic_pointer_cast<IFBXSceneLoader>(fbxModelsLoader);
	_ASSERT(fbxSceneLoader != nullptr);

	// Original skeleton
	auto originalSkeletonModel = fbxSceneLoader->LoadScene(modelFile, &fbxLoaderParams)->MergeModels();

	// Animated skeleton
	auto animatedSkeletonModel = fbxSceneLoader->LoadScene(animationFile, &fbxLoaderParams)->MergeModels();
	originalSkeletonModel->ApplyOtherSkeleton(animatedSkeletonModel);
	originalSkeletonModel->AddSkeletonAnimation(animatedSkeletonModel);
	for (const auto& anim : animatedSkeletonModel->GetAnimations())
		originalSkeletonModel->AddAnimation("run", anim.second);

	auto animatedSkeletonModel2 = fbxSceneLoader->LoadScene(DeathAnimationName, &fbxLoaderParams)->MergeModels();
	originalSkeletonModel->ApplyOtherSkeleton(animatedSkeletonModel2);
	originalSkeletonModel->AddSkeletonAnimation(animatedSkeletonModel2);
	for (const auto& anim : animatedSkeletonModel2->GetAnimations())
		originalSkeletonModel->AddAnimation("death", anim.second);

	std::string znModelFilename = modelFile->Name_NoExtension() + ".znmdl";
	if (filesManager->IsFileExists(znModelFilename))
		filesManager->Delete(znModelFilename);

	auto znMdlFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(originalSkeletonModel, znModelFilename);
	znMdlFile->Save();

	//node->GetComponentT<IModelsComponent3D>()->SetModel(originalSkeletonModel);
	//node->GetComponentT<IModelsComponent3D>()->PlayAnimation("run", true);
	return GetBaseManager().GetManager<IznModelsFactory>()->LoadModel(znMdlFile);
}

void CSceneRTS::CreateUnit()
{
	const auto& currentWave = m_RTSWaves.at(m_RTSCurrentWave);

	auto newRTSUnit = CreateSceneNodeCast<ISceneNodeRTSUnit>(cSceneNodeRTSUnit);
	newRTSUnit->SetName("RTSUnit");
	newRTSUnit->SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
	newRTSUnit->SetScale(glm::vec3(0.08f));
	newRTSUnit->GetComponentT<IModelsComponent3D>()->SetModel(currentWave.Model);
	newRTSUnit->GetComponentT<IModelsComponent3D>()->PlayAnimation("run", true);
	newRTSUnit->SetPath(m_RTSUnitsPath);

	// Change wave
	if (m_RTSCurrentWaveUnit >= currentWave.Count)
	{
		m_RTSCurrentWaveUnit = 0;
		m_RTSCurrentWave += 1;
	}

	m_RTSCurrentWaveUnit++;

}

bool CSceneRTS::OnTowerButtonClicked(const STowerDescription & TowerDesription)
{
	return true;
}
