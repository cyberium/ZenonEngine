#include "stdafx.h"

// Gerenal
#include "Editor3DFrame.h"

CEditor3DFrame::CEditor3DFrame(IEditor& Editor)
	: SceneBase(Editor.GetBaseManager())
	, m_Editor(Editor)
{
	dynamic_cast<IEditorPrivate&>(m_Editor).Set3DFrame(this);

	m_EditedScene = std::make_shared<CEditedScene>(Editor.GetBaseManager());
	m_EditedScene->Initialize();
}

CEditor3DFrame::~CEditor3DFrame()
{
}

void CEditor3DFrame::SetPreviewScene(const std::shared_ptr<CEditor3DPreviewScene>& PreviewScene)
{
	m_PreviewScene = PreviewScene;
}


//
// IGameState
//
void CEditor3DFrame::Initialize()
{
	SceneBase::Initialize();

	auto cameraNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
	cameraNode->SetName("Camera");
	auto geom = GetRenderDevice().GetPrimitivesFactory().CreateBBox();
	auto mat = std::make_shared<MaterialDebug>(GetRenderDevice());
	mat->SetDiffuseColor(glm::vec4(1.0f, 1.0f, 0.3f, 1.0f));
	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	model->AddConnection(mat, geom);
	cameraNode->GetComponent<IModelsComponent3D>()->SetModel(model);

	auto cameraComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode);
	cameraNode->AddComponent(cameraComponent);

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 1.0f, 5000.0f);

	m_LightsBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 8, sizeof(SLight), EAccess::CPUWrite);

	GetDefaultLight()->SetType(ELightType::Spot);
	GetDefaultLight()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	GetDefaultLight()->SetRange(99000.0f);
	GetDefaultLight()->SetIntensity(1.0f);
	GetDefaultLight()->SetSpotlightAngle(75.0f);
	
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

		m_TechniqueUI.AddPass(std::make_shared<CUIColorPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
		m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	}


	GetRootNode3D()->AddChild(m_EditedScene->GetRootNode3D());

	GetCameraController()->GetCamera()->SetTranslation(glm::vec3(15.0f * 2.0f));
	GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
	GetCameraController()->GetCamera()->SetYaw(225);
	GetCameraController()->GetCamera()->SetPitch(-45);
}

void CEditor3DFrame::Finalize()
{
	SceneBase::Finalize();
}

void CEditor3DFrame::AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	__super::AddChild(ParentNode, ChildNode);
}

void CEditor3DFrame::RemoveChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	__super::RemoveChild(ParentNode, ChildNode);
}

void CEditor3DFrame::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	if (SceneChangeType == ESceneChangeType::NodeRemovedFromParent)
	{
		auto& selector = reinterpret_cast<IEditor_NodesSelector&>(GetEditor().GetTools().GetTool(ETool::EToolSelector));
		for (const auto& node : GetEditor().GetSelectedNodes())
		{
			if (node == ChildNode)
			{
				selector.RemoveNode(ChildNode);
			}
		}
	}

	if (IsChildOf(GetEditedRootNode3D(), ChildNode) || IsChildOf(GetEditedRootNode3D(), OwnerNode))
		GetEditor().GetUIFrame().OnSceneChanged();
}


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
// IEditorSharedFrame
//
IEditor& CEditor3DFrame::GetEditor() const
{
	return m_Editor;
}

bool CEditor3DFrame::InitializeEditorFrame()
{
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

		node->GetComponent<IModelsComponent3D>()->SetModel(model);
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

		node->GetComponent<IModelsComponent3D>()->SetModel(model);
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

		node->GetComponent<IModelsComponent3D>()->SetModel(model);
	}

	return true;
}


//
// IEditor3DFrame
//
void CEditor3DFrame::DoInitializeTools3D()
{
	GetEditor().GetTools().AddPasses(m_Technique3D, GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
}

std::shared_ptr<IScene> CEditor3DFrame::GetScene()
{
	return shared_from_this();
}

void CEditor3DFrame::LockUpdates()
{
	Freeze();
}

void CEditor3DFrame::UnlockUpdates()
{
	Unfreeze();
}

void CEditor3DFrame::DoEnableTool(ETool Tool)
{
	GetEditor().GetTools().Enable(Tool);
}

std::shared_ptr<IScene> CEditor3DFrame::GetEditedScene() const
{
	return m_EditedScene;
}

std::shared_ptr<ISceneNode3D> CEditor3DFrame::GetEditedRootNode3D() const
{
	return m_EditedScene->GetRootNode3D();
}

std::shared_ptr<ISceneNode3D> CEditor3DFrame::GetNodeUnderMouse(const glm::ivec2& MousePos) const
{
	auto nodes = FindIntersection(GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), MousePos));
	if (nodes.empty())
		return nullptr;
	return nodes.begin()->second;
}

void CEditor3DFrame::OnCollectionWidget_ModelSelected(const std::shared_ptr<IModel>& Model)
{
	if (m_PreviewScene)
		m_PreviewScene->SetModel(Model);
}





//
// IEditor_NodesSelectorEventListener
//
void CEditor3DFrame::OnSelectNode()
{

	//m_Tools.m_Mover->Disable();
}



//
// Protected
//
void CEditor3DFrame::SetLights(const std::vector<SLight>& Lights)
{
	if (Lights.size() > m_LightsBuffer->GetElementCount())
		m_LightsBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(Lights, EAccess::CPUWrite);
	else
		m_LightsBuffer->Set(Lights);

	m_LightsCnt = Lights.size();
}