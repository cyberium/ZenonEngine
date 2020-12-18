#include "stdafx.h"

// Gerenal
#include "Scene_Default.h"

CSceneDefault::CSceneDefault(IBaseManager& BaseManager, IRenderWindow& RenderWindow)
	: SceneBase(BaseManager, RenderWindow)
	//, m_World(rp3d::Vector3(0.0f, -9.81f, 0.0f))
{
	// Change the number of iterations of the velocity solver
	//m_World.setNbIterationsVelocitySolver(15);
	// Change the number of iterations of the position solver
	//m_World.setNbIterationsPositionSolver(8);

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
	__super::Initialize();

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
		lightNode->SetLocalPosition(glm::vec3(150.0f, 150.0f, 150.0f));
		lightNode->SetRotationEuler(glm::vec3(-0.5f, -0.5f, -0.5f));

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
	}

	// Camera
	{
		auto cameraNode = CreateSceneNodeT<ISceneNode>();
		cameraNode->SetName("Camera");
		cameraNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponentT<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 5000.0f);
		GetCameraController()->GetCamera()->SetTranslation(glm::vec3(100.0f));
		GetCameraController()->GetCamera()->SetYaw(225);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}

	Load3D();

	//--------------------------------------------------------------------------
	// XML
	//--------------------------------------------------------------------------
	/*if (auto file = GetBaseManager().GetManager<IFilesManager>()->Open("RTS22656"))
	{
		CXMLManager xml(GetBaseManager());
		auto reader = xml.CreateReader(file);
		auto rootNodeXML = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->LoadSceneNode3DXML(reader->GetChilds()[0], *this);
	}*/
}

void CSceneDefault::Finalize()
{
	SceneBase::Finalize();
}

void CSceneDefault::OnUpdate(UpdateEventArgs & e)
{
	__super::OnUpdate(e);
}

bool CSceneDefault::OnMousePressed(const MouseButtonEventArgs & e, const Ray& RayToWorld)
{
	return false;
}

void CSceneDefault::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
}



//
// Keyboard events
//
bool CSceneDefault::OnWindowKeyPressed(KeyEventArgs & e)
{
	return SceneBase::OnWindowKeyPressed(e);
}

void CSceneDefault::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}
