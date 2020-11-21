#include "stdafx.h"

// Gerenal
#include "SceneRTS.h"

CSceneRTS::CSceneRTS(IBaseManager& BaseManager, IRenderWindow& RenderWindow)
	: SceneBase(BaseManager, RenderWindow)
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

		auto sceneNodeRTSUnit = CreateSceneNodeTCast<ISceneNodeRTSUnit>();
		sceneNodeRTSUnit->SetPath(m_RTSUnitsPath);
	}


	{
		std::shared_ptr<CUIControlRTSTowersPanel> commonControl = std::dynamic_pointer_cast<CUIControlRTSTowersPanel>(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IUIControlFactory>()->CreateSceneNodeUI(cUIControlRTSTowersPanel, *this));
		
		commonControl->AddTowerButton(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("t1.png"));
		commonControl->AddTowerButton(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("t2.png"));
		commonControl->AddTowerButton(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("tA.png"));
		commonControl->AddTowerButton(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("tB.png"));
		commonControl->AddTowerButton(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("tC.png"));
		commonControl->AddTowerButton(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("tE.png"));

		commonControl->SetTranslate(glm::vec2(
			(GetRenderWindow().GetWindowWidth() / 2.0f) - (commonControl->GetSize().x / 2.0f), 
			 GetRenderWindow().GetWindowHeight() - commonControl->GetSize().y)
		);
		//commonControl->SetScale(glm::vec2(0.75f));
	}


}

void CSceneRTS::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}

void CSceneRTS::OnUpdate(UpdateEventArgs & e)
{
	__super::OnUpdate(e);
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
