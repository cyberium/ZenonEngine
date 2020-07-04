#include "stdafx.h"

// Gerenal
#include "Editor3DFrame.h"

CSceneEditor::CSceneEditor(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
	, m_EditorUI(nullptr)
	, m_IsSelecting(false)
{
	m_EditedScene = std::make_shared<CEditedScene>(BaseManager);
	m_EditedScene->Initialize();
}

CSceneEditor::~CSceneEditor()
{
}

void CSceneEditor::SetEditorUI(IEditorUIFrame * EditorUIFrame)
{
	m_EditorUI = EditorUIFrame;

	Selector_SetOtherSelector(dynamic_cast<CSceneNodesSelector*>(m_EditorUI));
}


//
// IGameState
//
void CSceneEditor::Initialize()
{
	SceneBase::Initialize();

	auto cameraNode = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
	cameraNode->SetName("Camera");
	cameraNode->AddComponent(std::make_shared<CCameraComponent3D>(*cameraNode));

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 1.0f, 5000.0f);

	Load3D();
	LoadUI();

	GetRootNode3D()->AddChild(m_EditedScene->GetRootNode3D());

	cameraNode->SetTranslate(glm::vec3(-50, 160, 170));
	GetCameraController()->GetCamera()->SetYaw(-51);
	GetCameraController()->GetCamera()->SetPitch(-38);
}

void CSceneEditor::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}

void CSceneEditor::AddChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	//if (ChildNode != m_SelectedNode)
	//	DoSelectNode(ChildNode);

	__super::AddChild(ParentNode, ChildNode);
}

void CSceneEditor::RemoveChild(const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	//if (ChildNode == m_SelectedNode)
	//	DoSelectNode(nullptr);

	__super::RemoveChild(ParentNode, ChildNode);
}

bool IsChildOf(const std::shared_ptr<ISceneNode3D>& Parent, const std::shared_ptr<ISceneNode3D>& Child)
{
	if (Parent == nullptr || Child == nullptr)
		return false;

	if (Parent == Child)
		return true;

	for (const auto& ch : Parent->GetChilds())
		if (IsChildOf(ch, Child))
			return true;

	return false;
}

void CSceneEditor::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	if (IsChildOf(GetRealRootNode3D(), ChildNode) || IsChildOf(GetRealRootNode3D(), OwnerNode))
		m_EditorUI->OnSceneChanged();
}

void CSceneEditor::OnMouseClickToWorld(MouseButtonEventArgs::MouseButton & MouseButton, const glm::vec2 & MousePosition, const Ray & RayToWorld)
{
	if (MouseButton == MouseButtonEventArgs::MouseButton::Left)
	{
		auto node = FindIntersection(RayToWorld);
		if (node == nullptr)
		{
			m_SelectionPrevPos = MousePosition;
			m_SelectionTexture->SetTranslate(MousePosition);
			m_IsSelecting = true;

			//glm::vec3 resultPosition = GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));

//MoveDraggedNode(resultPosition);

			//CreateNode(ToBoxCoords(resultPosition), 0);

			return;
		}

		Selector_SelectNode(node);
	}
	else if (MouseButton == MouseButtonEventArgs::MouseButton::Right)
	{

	}
}

void CSceneEditor::OnMouseReleaseToWorld(MouseButtonEventArgs::MouseButton & MouseButton, const glm::vec2 & MousePosition, const Ray & RayToWorld)
{
	if (MouseButton == MouseButtonEventArgs::MouseButton::Left)
	{
		auto cachedSelectionPrevPos = m_SelectionPrevPos;

		// Clear
		m_SelectionTexture->SetScale(glm::vec3(0.001f));
		m_SelectionPrevPos = MousePosition;

		if (m_IsSelecting)
		{
			if (glm::length(glm::abs(cachedSelectionPrevPos - MousePosition)) > 10.0f)
			{
				Frustum f;
				f.buildBoxFrustum(
					GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), glm::vec2(glm::min(cachedSelectionPrevPos.x, MousePosition.x), glm::min(cachedSelectionPrevPos.y, MousePosition.y))),
					GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), glm::vec2(glm::min(cachedSelectionPrevPos.x, MousePosition.x), glm::max(cachedSelectionPrevPos.y, MousePosition.y))),
					GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), glm::vec2(glm::max(cachedSelectionPrevPos.x, MousePosition.x), glm::min(cachedSelectionPrevPos.y, MousePosition.y))),
					GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), glm::vec2(glm::max(cachedSelectionPrevPos.x, MousePosition.x), glm::max(cachedSelectionPrevPos.y, MousePosition.y))),
					15000.0f
				);

				auto nodes = FindIntersections(f);
				if (!nodes.empty())
					Selector_SelectNodes(nodes);
			}
			m_IsSelecting = false;
		}
	}
}

void CSceneEditor::OnMouseMoveToWorld(MouseButtonEventArgs::MouseButton & MouseButton, const glm::vec2& MousePosition, const Ray & RayToWorld)
{
	if (MouseButton == MouseButtonEventArgs::MouseButton::Left)
	{
		if (m_IsSelecting)
		{
			glm::vec2 scale = MousePosition - m_SelectionTexture->GetTranslation();
			m_SelectionTexture->SetScale(scale);
		}
	}
}


void CSceneEditor::OnPreRender(RenderEventArgs& e)
{

	SceneBase::OnPreRender(e);
}

bool CSceneEditor::OnWindowKeyPressed(KeyEventArgs & e)
{
	return SceneBase::OnWindowKeyPressed(e);
}

void CSceneEditor::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}



//
// IEditor3DFrame
//
void CSceneEditor::LockUpdates()
{
	Freeze();
}

void CSceneEditor::UnlockUpdates()
{
	Unfreeze();
}

std::shared_ptr<ISceneNode3D> CSceneEditor::GetRealRootNode3D() const
{
	return m_EditedScene->GetRootNode3D();
}

std::shared_ptr<ISceneNode3D> CSceneEditor::GetNodeUnderMouse(const glm::ivec2& MousePos) const
{
	return FindIntersection(GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), MousePos));
}



//
// CSceneNodesSelector
//
void CSceneEditor::Selector_OnSelectionChange()
{
	m_DrawSelectionPass->RefreshInstanceBuffer();
}








glm::ivec3 CSceneEditor::ToBoxCoords(const glm::vec3 & Position)
{
	return glm::round(Position / 10.0f);
}

glm::vec3 CSceneEditor::FixBoxCoords(const glm::vec3 & Position)
{
	glm::vec3 newPosition = Position;
	newPosition /= 10.0f;
	newPosition = glm::round(newPosition);
	newPosition *= 10.0f;
	return newPosition;
}

//
// Protected
//
void CSceneEditor::MoveDraggedNode(const glm::vec3 & Position)
{
	if (m_DraggedNode)
	{
		glm::vec3 newPosition = Position;
		newPosition /= 10.0f;
		newPosition = glm::round(newPosition);
		newPosition *= 10.0f;
		m_DraggedNode->SetTranslate(newPosition);
	}
}

void CSceneEditor::Load3D()
{
	{
		auto sceneNodeLight = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		sceneNodeLight->SetName("Light");
		sceneNodeLight->SetTranslate(glm::vec3(80.0f, 600.0f, 80.0f));
		sceneNodeLight->SetRotation(glm::vec3(0.0f, -1.0f, 0.0f));

		sceneNodeLight->AddComponent(std::make_shared<CLightComponent3D>(*sceneNodeLight.get()));
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetRange(48000.0f);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetSpotlightAngle(55.0f);
	}

	{
		m_DraggedNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
		m_DraggedNode->SetName("Dragged node parent.");
	}
	
	{
		m_Mover = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
		m_Mover->SetName("Mover node.");

		auto X_Geom = GetRenderDevice().GetPrimitivesFactory().CreateLine(glm::vec3(1.0f, 0.0f, 0.0f));
		auto Y_Geom = GetRenderDevice().GetPrimitivesFactory().CreateLine(glm::vec3(0.0f, 1.0f, 0.0f));
		auto Z_Geom = GetRenderDevice().GetPrimitivesFactory().CreateLine(glm::vec3(0.0f, 0.0f, 1.0f));

		auto materialX = std::make_shared<MaterialDebug>(GetRenderDevice());
		materialX->SetDiffuseColor(glm::vec4(1.0f, 0.3f, 0.1f, 1.0f));

		auto materialY = std::make_shared<MaterialDebug>(GetRenderDevice());
		materialY->SetDiffuseColor(glm::vec4(0.3f, 1.0f, 0.3f, 1.0f));

		auto materialZ = std::make_shared<MaterialDebug>(GetRenderDevice());
		materialZ->SetDiffuseColor(glm::vec4(0.1f, 0.3f, 1.0f, 1.0f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(materialX, X_Geom);
		model->AddConnection(materialY, Y_Geom);
		model->AddConnection(materialZ, Z_Geom);

		m_Mover->GetComponent<IModelsComponent3D>()->AddModel(model);
	}

	glm::vec4 color = glm::vec4(0.0, 0.0f, 0.0f, 1.0f);
	m_Technique3D.AddPass(std::make_shared<ClearRenderTargetPass>(GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), ClearFlags::All, color /*glm::vec4(0.2f, 0.2f, 0.2f, 0.2f)*/, 1.0f, 0));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassOpaque", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassTransperent", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	//m_Technique3D.AddPass(std::make_shared<CDrawBoundingBoxPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("DebugPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	m_DrawSelectionPass = std::make_shared<CDrawSelectionPass>(GetRenderDevice(), this);
	m_DrawSelectionPass->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
	m_Technique3D.AddPass(m_DrawSelectionPass);
}

void CSceneEditor::LoadUI()
{
	m_SelectionTexture = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>(ofkSceneNodeUI)->CreateSceneNodeUI(this, cSceneNodeUI_Color, GetRootNodeUI());
	m_SelectionTexture->GetProperties()->GetPropertyT<glm::vec4>("Color")->Set(glm::vec4(0.1f, 0.3f, 1.0f, 0.3f));

	m_TechniqueUI.AddPass(std::make_shared<CUIColorPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}
