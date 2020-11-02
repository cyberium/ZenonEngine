#include "stdafx.h"

// Gerenal
#include "Editor3DPreviewScene.h"

CEditor3DPreviewScene::CEditor3DPreviewScene(IBaseManager& BaseManager, IRenderWindow& RenderWindow)
	: SceneBase(BaseManager, RenderWindow)
{

}

CEditor3DPreviewScene::~CEditor3DPreviewScene()
{
}

void CEditor3DPreviewScene::SetModel(const std::shared_ptr<IModel>& Model)
{
	_ASSERT(Model != nullptr);

	if (m_Node == nullptr)
		return;

	auto modelComponent = m_Node->GetComponent<IModelsComponent3D>();
	if (modelComponent->GetModel())
		modelComponent->ResetModel();

	modelComponent->SetModel(Model);

	const auto& modelBBox = Model->GetBounds();
	float radius = modelBBox.getRadius();

	m_Node->SetTranslate(- modelBBox.getCenter());
	GetCameraController()->GetCamera()->SetTranslation(glm::vec3(radius * 1.5f));
	GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
}


//
// IGameState
//
void CEditor3DPreviewScene::Initialize()
{
	SceneBase::Initialize();

	// Light
	{
		auto lightNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		lightNode->SetName("Light");
		lightNode->SetTranslate(glm::vec3(250.0f, 250.0f, 250.0f));
		lightNode->SetRotation(glm::vec3(-0.9f, -0.9f, -0.9f));

		lightNode->AddComponent(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *lightNode.get()));
		lightNode->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		lightNode->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightNode->GetComponent<ILightComponent3D>()->SetRange(1000.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetIntensity(1.2f);
		lightNode->GetComponent<ILightComponent3D>()->SetSpotlightAngle(45.0f);
	}


	{
		auto cameraNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		cameraNode->SetName("Camera");
		cameraNode->AddComponent(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 5000.0f);
		GetCameraController()->GetCamera()->SetTranslation(glm::vec3(15.0f * 2.0f));
		GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
		GetCameraController()->GetCamera()->SetYaw(225);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}


	{
		m_Node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		m_Node->SetName("Node model preview.");
	}


	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		node->SetName("Grid node x10.");
		node->SetTranslate(glm::vec3(0.0f, 0.03f, 0.0f));
		node->SetScale(glm::vec3(10.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponent<IModelsComponent3D>()->SetModel(model);
	}



	//
	// Renderer
	//
	auto forwardrender = MakeShared(CRendererForward, GetBaseManager(), *this);
	forwardrender->Initialize(GetRenderWindow().GetRenderTarget(), &GetRenderWindow().GetViewport());
	SetRenderer(forwardrender);
}

void CEditor3DPreviewScene::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}
