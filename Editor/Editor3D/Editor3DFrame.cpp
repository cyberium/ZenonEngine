#include "stdafx.h"

// Gerenal
#include "Editor3DFrame.h"

// Additional
#include "Editor3D/EditorPasses/DrawToolsPass.h"

CEditor3DFrame::CEditor3DFrame(IEditor& Editor, IRenderWindow& RenderWindow)
	: SceneBase(Editor.GetBaseManager(), RenderWindow)
	, m_Editor(Editor)
{
	dynamic_cast<IEditorPrivate&>(m_Editor).Set3DFrame(this);

	m_EditedScene = MakeShared(CEditedScene, Editor.GetBaseManager(), RenderWindow);
	m_EditedScene->AddSceneEventsListener(this);
	m_EditedScene->Initialize();
}

CEditor3DFrame::~CEditor3DFrame()
{
	m_EditedScene->RemoveSceneEventsListener(this);

	Log::Info("CEditor3DFrame deleted.");
}




//
// IGameState
//
void CEditor3DFrame::Initialize()
{
	__super::Initialize();

	GetRootSceneNode()->AddChild(m_EditedScene->GetRootSceneNode());

	glm::vec3 rtsCenter = glm::vec3(cCellSize * cCellsCount) / 2.0f;

	// Light
	{
		auto lightNode = CreateSceneNodeT<ISceneNode>();
		lightNode->SetName("Light2");
		lightNode->SetLocalPosition(glm::vec3(150.0f, 150.0f, 150.0f));
		lightNode->SetLocalRotationEuler(glm::vec3(-45.0f, -45.0f, -45.0f));

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

	// Camera
	{
		auto cameraNode = CreateSceneNodeT<ISceneNode>();
		cameraNode->SetName("Camera");
		cameraNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponentT<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 5000.0f);
		GetCameraController()->GetCamera()->SetTranslation(glm::vec3(15.0f * 2.0f));
		GetCameraController()->GetCamera()->SetYaw(225);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}
}

void CEditor3DFrame::Finalize()
{
	SceneBase::Finalize();
}



//
// ISceneInternal
//
bool CEditor3DFrame::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_Editor.GetTools().OnMousePressed(e, RayToWorld))
		return true;

	return false;
}

void CEditor3DFrame::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	m_Editor.GetTools().OnMouseReleased(e, RayToWorld);
}

void CEditor3DFrame::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	m_Editor.GetTools().OnMouseMoved(e, RayToWorld);
}



//
// IEditor3DFrame
//
IEditor& CEditor3DFrame::GetEditor() const
{
	return m_Editor;
}

bool CEditor3DFrame::InitializeEditorFrame()
{
	{
		auto node = CreateSceneNodeT<ISceneNode>();
		node->SetName("GridNodeX1");
		node->SetPosition(glm::vec3(0.0f));
		node->SetScale(glm::vec3(1.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.5f, 0.5f, 0.5f, 0.3f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponentT<IModelComponent>()->SetModel(model);
	}

	{
		auto node = CreateSceneNodeT<ISceneNode>();
		node->SetName("GridNodeX10");
		node->SetPosition(glm::vec3(0.0f, 0.00f, 0.0f));
		node->SetScale(glm::vec3(10.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.6f, 0.6f, 0.6f, 0.5f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponentT<IModelComponent>()->SetModel(model);
	}

	{
		auto node = CreateSceneNodeT<ISceneNode>();
		node->SetName("GridNodeX100");
		node->SetPosition(glm::vec3(0.0f, 0.00f, 0.0f));
		node->SetScale(glm::vec3(100.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines(10);

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.7f, 0.7f, 0.7f, 0.7f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponentT<IModelComponent>()->SetModel(model);
	}

	return true;
}

void CEditor3DFrame::DoInitializeTools3D()
{
	GetEditor().GetTools().DoInitialize3D(GetRenderer(), GetRenderWindow().GetRenderTarget());
}

IScene& CEditor3DFrame::GetScene()
{
	return *this;
}

void CEditor3DFrame::DoEnableTool(ETool Tool)
{
	GetEditor().GetTools().Enable(Tool);
}

std::shared_ptr<IScene> CEditor3DFrame::GetEditedScene() const
{
	return m_EditedScene;
}

std::shared_ptr<ISceneNode> CEditor3DFrame::GetEditedRootNode3D() const
{
	return m_EditedScene->GetRootSceneNode();
}

std::shared_ptr<ISceneNode> CEditor3DFrame::GetEditedNodeUnderMouse(const glm::ivec2& MousePos) const
{
	auto ray = GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), MousePos);
	auto nodes = GetFinder().FindIntersection(ray, nullptr, GetEditedRootNode3D());
	if (nodes.empty())
		return nullptr;
	return nodes.begin()->second;
}



//
// IEditorToolSelectorEventListener
//
void CEditor3DFrame::OnSelectNode()
{

}



//
// ISceneEventsListener // from edited scene
//
void CEditor3DFrame::OnSceneNodeAdded(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode)
{
	RaiseSceneChangeEvent(ESceneChangeType::NodeAddedToParent, ParentNode, ChildNode);
}

void CEditor3DFrame::OnSceneNodeRemoved(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode)
{
	RaiseSceneChangeEvent(ESceneChangeType::NodeRemovedFromParent, ParentNode, ChildNode);
}
