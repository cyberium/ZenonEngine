#include "stdafx.h"

// Gerenal
#include "Scene_Default.h"

CSceneDefault::CSceneDefault(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
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
	SceneBase::Initialize();

	// Light
	/*{
		auto lightNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
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
		auto lightNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		lightNode->SetName("Light2");
		lightNode->SetTranslate(glm::vec3(150.0f, 150.0f, 150.0f));
		lightNode->SetRotation(glm::vec3(-0.5f, -0.5f, -0.5f));

		lightNode->AddComponent(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *lightNode.get()));
		lightNode->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		lightNode->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightNode->GetComponent<ILightComponent3D>()->SetRange(3500.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetIntensity(1.5f);
		lightNode->GetComponent<ILightComponent3D>()->SetSpotlightAngle(45.0f);
	}

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

	Load3D();

	//--------------------------------------------------------------------------
	// RENDERERS
	//--------------------------------------------------------------------------
	auto forwardRenderer = MakeShared(CRendererForward, GetBaseManager(), weak_from_this());
	forwardRenderer->Initialize(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
	m_ForwardRenderer = forwardRenderer;

	auto defferedRenderer = MakeShared(CRendererDeffered, GetBaseManager(), weak_from_this());
	defferedRenderer->Initialize(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
	m_DefferedRenderrer = defferedRenderer;

	SetRenderer(forwardRenderer);
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
	//m_World.update(e.DeltaTime / 1000.0f * 2.0f);
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
