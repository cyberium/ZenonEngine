#include "stdafx.h"

// Gerenal
#include "Editor3DFrame.h"

CEdtor3DFrame::CEdtor3DFrame(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
	, m_EditorUI(nullptr)
	, m_Tools(*this)
{
	m_EditedScene = std::make_shared<CEditedScene>(BaseManager);
	m_EditedScene->Initialize();
}

CEdtor3DFrame::~CEdtor3DFrame()
{
}

void CEdtor3DFrame::SetEditorUI(IEditorUIFrame * EditorUIFrame)
{
	m_EditorUI = EditorUIFrame;
}

void CEdtor3DFrame::SetPreviewScene(const std::shared_ptr<CEditor3DPreviewScene>& PreviewScene)
{
	m_PreviewScene = PreviewScene;
}


//
// IGameState
//
void CEdtor3DFrame::Initialize()
{
	SceneBase::Initialize();

	auto cameraNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
	cameraNode->SetName("Camera");
	auto geom = GetRenderDevice().GetPrimitivesFactory().CreateBBox();
	auto mat = std::make_shared<MaterialDebug>(GetRenderDevice());
	mat->SetDiffuseColor(glm::vec4(1.0f, 1.0f, 0.3f, 1.0f));
	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	model->AddConnection(mat, geom);
	cameraNode->GetComponent<IModelsComponent3D>()->AddModel(model);

	auto cameraComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode);
	cameraNode->AddComponent(cameraComponent);

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 1.0f, 5000.0f);

	m_LightsBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 8, sizeof(SLight), EAccess::CPUWrite);

	m_Tools.Initialize();
	m_Tools.m_Selector->SetOtherSelector(m_EditorUI->GetNodesSelector());

	Load3D();
	
	//
	// Passes
	//
	{
		glm::vec4 color = glm::vec4(0.33f, 0.33f, 0.33f, 1.0f);
		m_Technique3D.AddPass(std::make_shared<ClearRenderTargetPass>(GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), ClearFlags::All, color /*glm::vec4(0.2f, 0.2f, 0.2f, 0.2f)*/, 1.0f, 0));

		auto materialModelPass = GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("MaterialModelPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this());
		m_MaterialModelPass = std::dynamic_pointer_cast<IMaterialModelPass>(materialModelPass);

		{
			auto invokePass = GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("InvokePass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this());
			std::dynamic_pointer_cast<IInvokeFunctionPass>(invokePass)->SetFunc([this]() {

				std::vector<SLight> lights;
				auto lightStruct = std::dynamic_pointer_cast<ILight3D>(GetDefaultLight())->GetLightStruct();
				lightStruct.PositionVS = GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightStruct.PositionWS.xyz(), 1.0f);
				lightStruct.DirectionVS = glm::normalize(GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightStruct.DirectionWS.xyz(), 0.0f));
				lights.push_back(lightStruct);
				SetLights(lights);

				m_MaterialModelPass->GetLightsShaderParameter()->Set(m_LightsBuffer);
			});
			m_Technique3D.AddPass(invokePass);
		}

		m_Technique3D.AddPass(materialModelPass);

		// Debug render
		m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("DebugPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));

		m_Tools.m_Selector->AddPasses(m_Technique3D, GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());


		m_TechniqueUI.AddPass(std::make_shared<CUIColorPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
		m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	}


	GetRootNode3D()->AddChild(m_EditedScene->GetRootNode3D());

	GetCameraController()->GetCamera()->SetTranslation(glm::vec3(15.0f * 2.0f));
	GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
	GetCameraController()->GetCamera()->SetYaw(225);
	GetCameraController()->GetCamera()->SetPitch(-45);
}

void CEdtor3DFrame::Finalize()
{
	SceneBase::Finalize();
}

void CEdtor3DFrame::AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	__super::AddChild(ParentNode, ChildNode);
}

void CEdtor3DFrame::RemoveChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	__super::RemoveChild(ParentNode, ChildNode);
}

void CEdtor3DFrame::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	if (SceneChangeType == ESceneChangeType::NodeRemovedFromParent)
	{
		for (const auto& node : m_Tools.m_Selector->GetSelectedNodes())
		{
			if (node == ChildNode)
			{
				m_Tools.m_Selector->RemoveNode(ChildNode);
			}
		}
	}

	if (IsChildOf(GetRealRootNode3D(), ChildNode) || IsChildOf(GetRealRootNode3D(), OwnerNode))
		m_EditorUI->OnSceneChanged();
}

bool CEdtor3DFrame::OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_Tools.OnMouseClickToWorld(e, RayToWorld))
		return true;

	return false;
}

void CEdtor3DFrame::OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	m_Tools.OnMouseReleaseToWorld(e, RayToWorld);
}

void CEdtor3DFrame::OnMouseMoveToWorld(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	m_Tools.OnMouseMoveToWorld(e, RayToWorld);
}



void CEdtor3DFrame::OnPreRender(RenderEventArgs& e)
{
	SceneBase::OnPreRender(e);
}



void CEdtor3DFrame::OnWindowMouseMoved(MouseMotionEventArgs & e)
{
	__super::OnWindowMouseMoved(e);
}

bool CEdtor3DFrame::OnWindowMouseButtonPressed(MouseButtonEventArgs & e)
{
	return __super::OnWindowMouseButtonPressed(e);;
}

void CEdtor3DFrame::OnWindowMouseButtonReleased(MouseButtonEventArgs & e)
{
	__super::OnWindowMouseButtonReleased(e);
}

bool CEdtor3DFrame::OnWindowKeyPressed(KeyEventArgs & e)
{
	return __super::OnWindowKeyPressed(e);
}

void CEdtor3DFrame::OnWindowKeyReleased(KeyEventArgs & e)
{
	__super::OnWindowKeyReleased(e);
}



//
// IEditorSharedFrame
//
IEditor_NodesSelector * CEdtor3DFrame::GetNodesSelector()
{
	return m_Tools.m_Selector.get();
}




//
// IEditor3DFrame
//
std::shared_ptr<IScene> CEdtor3DFrame::GetScene()
{
	return shared_from_this();
}

IBaseManager& CEdtor3DFrame::GetBaseManager2() const
{
	return GetBaseManager();
}

IRenderDevice& CEdtor3DFrame::GetRenderDevice2() const
{
	return GetRenderDevice();
}

void CEdtor3DFrame::LockUpdates()
{
	Freeze();
}

void CEdtor3DFrame::UnlockUpdates()
{
	Unfreeze();
}

void CEdtor3DFrame::EnableSelectorTool()
{
}

void CEdtor3DFrame::EnableMoverTool()
{
	m_Tools.m_Mover->Enable();
	m_Tools.m_Selector->Disable();
}

void CEdtor3DFrame::EnableDraggerTool()
{
	m_Tools.m_Mover->Disable();
	m_Tools.m_Selector->Enable();
}

std::shared_ptr<IScene> CEdtor3DFrame::GetRealScene() const
{
	return m_EditedScene;
}

std::shared_ptr<ISceneNode3D> CEdtor3DFrame::GetRealRootNode3D() const
{
	return m_EditedScene->GetRootNode3D();
}

std::shared_ptr<ISceneNode3D> CEdtor3DFrame::GetNodeUnderMouse(const glm::ivec2& MousePos) const
{
	auto nodes = FindIntersection(GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), MousePos));
	if (nodes.empty())
		return nullptr;
	return nodes.begin()->second;
}

void CEdtor3DFrame::OnCollectionWidget_ModelSelected(const std::shared_ptr<IModel>& Model)
{
	if (m_PreviewScene)
		m_PreviewScene->SetModel(Model);
}

void CEdtor3DFrame::DropEvent(const glm::vec2& Position)
{
	m_Tools.m_Drager->DropEvent(Position);
}

void CEdtor3DFrame::DragEnterEvent(const SDragData& Data)
{
	m_Tools.m_Drager->DragEnterEvent(Data);
}

void CEdtor3DFrame::DragMoveEvent(const glm::vec2& Position)
{
	m_Tools.m_Drager->DragMoveEvent(Position);
}

void CEdtor3DFrame::DragLeaveEvent()
{
	m_Tools.m_Drager->DragLeaveEvent();
}

void CEdtor3DFrame::SetMoverValue(float value)
{
	m_Tools.m_Mover->SetMoverValue(value);
}



//
// IEditor_NodesSelectorEventListener
//
void CEdtor3DFrame::OnSelectNodes()
{
	
}



//
// Protected
//
void CEdtor3DFrame::Load3D()
{
	GetDefaultLight()->SetType(ELightType::Spot);
	GetDefaultLight()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	GetDefaultLight()->SetRange(99000.0f);
	GetDefaultLight()->SetIntensity(1.0f);
	GetDefaultLight()->SetSpotlightAngle(75.0f);


	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		node->SetName("Grid node x1.");
		node->SetTranslate(glm::vec3(0.0f));
		node->SetScale(glm::vec3(1.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = std::make_shared<MaterialDebug>(GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponent<IModelsComponent3D>()->AddModel(model);
	}

	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		node->SetName("Grid node x10.");
		node->SetTranslate(glm::vec3(0.0f, 0.03f, 0.0f));
		node->SetScale(glm::vec3(10.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = std::make_shared<MaterialDebug>(GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponent<IModelsComponent3D>()->AddModel(model);
	}

	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		node->SetName("Grid node x100.");
		node->SetTranslate(glm::vec3(0.0f, 0.06f, 0.0f));
		node->SetScale(glm::vec3(100.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines(10);

		auto mat = std::make_shared<MaterialDebug>(GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.7f, 0.7f, 0.7f, 1.0f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponent<IModelsComponent3D>()->AddModel(model);
	}


}

void CEdtor3DFrame::SetLights(const std::vector<SLight>& Lights)
{
	if (Lights.size() > m_LightsBuffer->GetElementCount())
		m_LightsBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(Lights, EAccess::CPUWrite);
	else
		m_LightsBuffer->Set(Lights);

	m_LightsCnt = Lights.size();
}