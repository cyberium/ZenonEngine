#include "stdafx.h"

// General
#include "EditedScene.h"

CEditedScene::CEditedScene(IBaseManager & BaseManager)
	: SceneBase(BaseManager)
{
}

CEditedScene::~CEditedScene()
{
	Log::Info("Scene destroyed.");
}

void CEditedScene::Initialize()
{
	SceneBase::Initialize();

	m_LightNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
	m_LightNode->SetName("Light");
	m_LightNode->SetTranslate(glm::vec3(1500.0f, 1500.0f, 1500.0f));
	m_LightNode->SetRotation(glm::vec3(-0.9f, -0.9f, -0.9f));

	m_LightNode->AddComponent(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *m_LightNode.get()));
	m_LightNode->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
	m_LightNode->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_LightNode->GetComponent<ILightComponent3D>()->SetRange(99000.0f);
	m_LightNode->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
	m_LightNode->GetComponent<ILightComponent3D>()->SetSpotlightAngle(75.0f);

	auto cameraNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, GetRootNode3D());
	cameraNode->SetName("Camera");
	auto geom = GetRenderDevice().GetPrimitivesFactory().CreateBBox();
	auto mat = MakeShared(MaterialDebug, GetRenderDevice());
	mat->SetDiffuseColor(glm::vec4(1.0f, 1.0f, 0.3f, 1.0f));
	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	model->AddConnection(mat, geom);
	cameraNode->GetComponent<IModelsComponent3D>()->SetModel(model);

	auto cameraComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode);
	cameraNode->AddComponent(cameraComponent);

	SetCameraController(MakeShared(CFreeCameraController));
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, 1280.0f / 1024.0f, 1.0f, 5000.0f);	
}

void CEditedScene::Finalize()
{
	SceneBase::Finalize();
}

void CEditedScene::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	auto root = GetRootNode3D();
	if (root == nullptr)
		return;

	auto parent = root->GetParent().lock();
	if (parent == nullptr)
		return;

	root->GetParent().lock()->GetScene()->RaiseSceneChangeEvent(SceneChangeType, OwnerNode, ChildNode);
}
