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

	std::shared_ptr<IParticleComponent3D> particles = MakeShared(CParticlesComponent, *GetRootSceneNode());
	GetRootSceneNode()->AddComponentT(particles);

	

	// Light
	{
		auto lightNode = CreateSceneNodeT<ISceneNode>();
		lightNode->SetName("Light");
		lightNode->SetLocalPosition(glm::vec3(150.0f, 150.0f, 150.0f));
		lightNode->SetLocalRotationEuler(glm::vec3(225.0f, -45.0f, 0.0f));
		//lightNode->SetLocalRotationDirection(glm::vec3(-0.5f, -0.5f, -0.5f));

		auto lightComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<CLightComponent>(cSceneNodeLightComponent, *lightNode.get());
		lightComponent->SetLight(MakeShared(CLight, GetBaseManager()));
		lightComponent->GetLight()->SetCastShadows(true);
		lightComponent->GetLight()->SetType(ELightType::Directional);
		lightComponent->GetLight()->SetAmbientColor(ColorRGB(0.25f));
		lightComponent->GetLight()->SetColor(ColorRGB(1.0f, 1.0f, 1.0f));
		lightComponent->GetLight()->SetRange(1000.0f);
		lightComponent->GetLight()->SetIntensity(1.0077f);
		lightComponent->GetLight()->SetSpotlightAngle(30.0f);

		lightNode->AddComponent(cSceneNodeLightComponent, lightComponent);
	}


	// Light 2
	/*{
		auto lightNode = CreateSceneNodeT<ISceneNode>();
		lightNode->SetName("Light2");
		lightNode->SetLocalPosition(glm::vec3(150.0f, 150.0f, 150.0f));
		lightNode->SetRotationEuler(glm::vec3(-0.5f, -0.5f, 0.5f));

		auto lightComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<CLightComponent>(cSceneNodeLightComponent, *lightNode.get());
		lightComponent->SetLight(MakeShared(CLight, GetBaseManager()));
		lightComponent->GetLight()->SetCastShadows(true);
		lightComponent->GetLight()->SetType(ELightType::Directional);
		lightComponent->GetLight()->SetAmbientColor(glm::vec3(0.25f));
		lightComponent->GetLight()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightComponent->GetLight()->SetRange(1000.0f);
		lightComponent->GetLight()->SetIntensity(1.0077f);
		lightComponent->GetLight()->SetSpotlightAngle(30.0f);

		lightNode->AddComponent(cSceneNodeLightComponent, lightComponent);
	}*/

	// Camera
	{
		float aspect = static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight());

		auto cameraNode = CreateSceneNodeT<ISceneNode>();
		cameraNode->SetName("Camera");
		cameraNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponentT<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 2500.0f);
		//GetCameraController()->GetCamera()->SetOrthographicProjection(aspect, -55, 55, 55, -55, -10.0f, 1.0f);
		
		GetCameraController()->GetCamera()->SetPosition(glm::vec3(0.0f));
		GetCameraController()->GetCamera()->SetYaw(225);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}


	//--------------------------------------------------------------------------
	// Sphere
	//--------------------------------------------------------------------------
	{
		const float cPlaneSize = 75.0f;
		const float cPlaneY = 0.0f;

		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetDiffuseColor(ColorRGB(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularColor(ColorRGB(1.0f, 1.0f, 1.0f));
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




	m_TowersStorage = MakeShared(CTowersStorage, GetBaseManager());

	m_UnitsStorage = MakeShared(CUnitsStorage, GetBaseManager());



	//--------------------------------------------------------------------------
	// XML
	//--------------------------------------------------------------------------
	if (auto file = GetBaseManager().GetManager<IFilesManager>()->Open("RTS.xml"))
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
		for (const auto& towerXMLProto : m_TowersStorage->GetTowerPrototypes())
			m_UIControlRTSTowersPanel->AddTowerButton(towerXMLProto.TowerName, towerXMLProto.TowerIcon, towerXMLProto.TowerCost, *this);

		/*m_UIControlRTSTowersPanel->AddTowerButton("Tower 1", "sceneNodesProtos/tower1.znobj", "sceneNodesProtos/tower1.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower 2", "sceneNodesProtos/tower2.znobj", "sceneNodesProtos/tower2.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower A", "sceneNodesProtos/towerA.znobj", "sceneNodesProtos/towerA.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower B", "sceneNodesProtos/towerB.znobj", "sceneNodesProtos/towerB.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower C", "sceneNodesProtos/towerC.znobj", "sceneNodesProtos/towerC.png", 100, *this);
		m_UIControlRTSTowersPanel->AddTowerButton("Tower E", "sceneNodesProtos/towerE.znobj", "sceneNodesProtos/towerE.png", 100, *this);*/

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
	if (m_CurrentTowerNode != nullptr)
	{
		if (e.Button == MouseButton::Left)
		{
			CreateTower();
			return true;
		}
		else if (e.Button == MouseButton::Right)
		{
			CancelTower();
			return true;
		}
	}
	else
	{

	}

	return __super::OnMousePressed(e, RayToWorld);
}

void CSceneRTS::OnMouseMoved(const MouseMotionEventArgs& e, const Ray & RayToWorld)
{
	if (m_CurrentTowerNode != nullptr)
	{
		MoveTower(RayToWorld);
	}
	else
	{

	}

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
	for (const auto& model : m_UnitsStorage->GetModels())
	{
		SRTSWave wave0;
		wave0.Model = model; // CreateUnitModel("Toon_RTS/models/WK_archer.FBX", "Toon_RTS/animation/archer/WK_archer_03_run.FBX", "Toon_RTS/animation/archer/WK_archer_10_death_A.FBX");
		wave0.Count = 1;
		wave0.IndervalMS = 1500;
		m_RTSWaves.push_back(wave0);
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
	auto createdTower = std::dynamic_pointer_cast<ISceneNode>(m_CurrentTowerNode->Copy());
	m_CreatedTowers.push_back(createdTower);

	m_CurrentTowerNode->MakeMeOrphan();
	m_CurrentTowerNode.reset();
}

void CSceneRTS::CancelTower()
{
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

	std::shared_ptr<ISceneNode> newTowerNode = m_TowersStorage->CreateTower(TowerDesription.TowerName, *this);
	GetRootSceneNode()->AddChild(newTowerNode);

	m_CurrentTowerNode = newTowerNode;

	return true;
}
