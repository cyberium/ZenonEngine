#include "stdafx.h"

// Gerenal
#include "Editor3DFrame.h"

CEdtor3DFrame::CEdtor3DFrame(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
	, m_EditorUI(nullptr)
	, m_IsDraggingEnabled(false)
	, m_IsDraggingPermanentCreation(false)
	, m_MoverValue(1.0f)
	, m_IsSelecting(false)
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

	Selector_SetOtherSelector(dynamic_cast<CSceneNodesSelector*>(m_EditorUI));
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

	auto cameraNode = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
	cameraNode->SetName("Camera");
	cameraNode->AddComponent(std::make_shared<CCameraComponent3D>(*cameraNode));

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 1.0f, 5000.0f);

	Load3D();
	LoadUI();

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

bool IsChildOf(const std::shared_ptr<ISceneNode3D>& Parent, const std::shared_ptr<ISceneNode3D>& Child)
{
	if (Parent == nullptr || Child == nullptr)
		return false;

	if (Parent->GetGuid() == Child->GetGuid())
		return true;

	for (const auto& ch : Parent->GetChilds())
		if (IsChildOf(ch, Child))
			return true;

	return false;
}

void CEdtor3DFrame::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& OwnerNode, const std::shared_ptr<ISceneNode3D>& ChildNode)
{
	if (SceneChangeType == ESceneChangeType::NodeRemovedFromParent)
	{
		for (const auto& node : Selector_GetSelectedNodes())
		{
			if (node == ChildNode)
			{
				Selector_RemoveNode(ChildNode);
			}
		}
	}

	if (IsChildOf(GetRealRootNode3D(), ChildNode) || IsChildOf(GetRealRootNode3D(), OwnerNode))
		m_EditorUI->OnSceneChanged();
}

bool CEdtor3DFrame::OnMouseClickToWorld(MouseButtonEventArgs::MouseButton & MouseButton, const glm::vec2 & MousePosition, const Ray & RayToWorld)
{
	if (MouseButton == MouseButtonEventArgs::MouseButton::Left)
	{
		if (m_IsDraggingEnabled)
		{
			DoDropNodeAndCreateIt();
			return true;
		}

		auto nodes = FindIntersection(RayToWorld);
		if (nodes.empty())
		{
			m_SelectionPrevPos = MousePosition;
			m_SelectionTexture->SetTranslate(MousePosition);
			m_IsSelecting = true;
			return true;
		}

		Selector_SelectNode(nodes.begin()->second);
		return  true;
	}
	else if (MouseButton == MouseButtonEventArgs::MouseButton::Right)
	{
		if (m_IsDraggingEnabled)
		{
			DragLeaveEvent();
			return true;
		}
	}

	return false;
}

void CEdtor3DFrame::OnMouseReleaseToWorld(MouseButtonEventArgs::MouseButton & MouseButton, const glm::vec2 & MousePosition, const Ray & RayToWorld)
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

void CEdtor3DFrame::OnMouseMoveToWorld(MouseButtonEventArgs::MouseButton & MouseButton, const glm::vec2& MousePosition, const Ray & RayToWorld)
{
	if (m_IsDraggingEnabled)
	{
		DoMoveNode(MousePosition);
		return;
	}

	if (MouseButton == MouseButtonEventArgs::MouseButton::Left)
	{
		if (m_IsSelecting)
		{
			glm::vec2 scale = MousePosition - m_SelectionTexture->GetTranslation();
			m_SelectionTexture->SetScale(scale);
		}
	}
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
// IEditor3DFrame
//
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
	if (m_IsDraggingEnabled)
	{
		DoDropNodeAndCreateIt();

		if (!m_IsDraggingPermanentCreation)
			DragLeaveEvent();
	}

	
}

void CEdtor3DFrame::DragEnterEvent(const SDragData& Data)
{
	m_IsDraggingEnabled = true;
	m_IsDraggingPermanentCreation = Data.IsCtrl;


	auto ray = GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), Data.Position);
	auto node = m_EditedScene->CreateNode(glm::vec3(), Data.Message);
	auto pos = GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), node->GetModelsComponent()->GetModels()[0]->GetBounds().getCenter().y));
	m_DraggedNode->AddChild(node);
	m_DraggedNode->SetTranslate(pos);
}

void CEdtor3DFrame::DragMoveEvent(const glm::vec2& Position)
{
	if (m_IsDraggingEnabled)
	{
		DoMoveNode(Position);
	}
}

void CEdtor3DFrame::DragLeaveEvent()
{
	m_IsDraggingEnabled = false;
	m_IsDraggingPermanentCreation = false;
	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("");
	while (!m_DraggedNode->GetChilds().empty())
		m_DraggedNode->RemoveChild(m_DraggedNode->GetChilds()[0]);
}

void CEdtor3DFrame::SetMoverValue(float value)
{
	m_MoverValue = value;
}



//
// CSceneNodesSelector
//
void CEdtor3DFrame::Selector_OnSelectionChange()
{
	m_DrawSelectionPass->RefreshInstanceBuffer();
}


glm::ivec3 CEdtor3DFrame::ToBoxCoords(const glm::vec3 & Position)
{
	return glm::round(Position / m_MoverValue);
}

glm::vec3 CEdtor3DFrame::FixBoxCoords(const glm::vec3 & Position)
{
	glm::vec3 newPosition = Position;
	newPosition /= m_MoverValue;
	newPosition = glm::round(newPosition);
	newPosition *= m_MoverValue;
	return newPosition;
}

//
// Protected
//
void CEdtor3DFrame::Load3D()
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

	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
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
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
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
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
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

	glm::vec4 color = glm::vec4(0.33, 0.33f, 0.33f, 1.0f);
	m_Technique3D.AddPass(std::make_shared<ClearRenderTargetPass>(GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), ClearFlags::All, color /*glm::vec4(0.2f, 0.2f, 0.2f, 0.2f)*/, 1.0f, 0));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassOpaque", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassTransperent", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	//m_Technique3D.AddPass(std::make_shared<CDrawBoundingBoxPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("DebugPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	m_DrawSelectionPass = std::make_shared<CDrawSelectionPass>(GetRenderDevice(), this);
	m_DrawSelectionPass->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
	m_Technique3D.AddPass(m_DrawSelectionPass);
}

void CEdtor3DFrame::LoadUI()
{
	m_DraggerTextUI = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>(ofkSceneNodeUI)->CreateSceneNodeUI(this, cSceneNodeUI_Text);
	GetRootNodeUI()->AddChild(m_DraggerTextUI);

	m_SelectionTexture = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>(ofkSceneNodeUI)->CreateSceneNodeUI(this, cSceneNodeUI_Color, GetRootNodeUI());
	m_SelectionTexture->GetProperties()->GetPropertyT<glm::vec4>("Color")->Set(glm::vec4(0.1f, 0.3f, 1.0f, 0.3f));

	m_TechniqueUI.AddPass(std::make_shared<CUIColorPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}

void CEdtor3DFrame::DoMoveNode(const glm::vec2& MousePos)
{
	if (!m_IsDraggingEnabled)
		return;
	
	//auto posReal = GetCameraController()->ScreenToPlane(GetRenderWindow()->GetViewport(), MousePos, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));

	auto ray = GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), MousePos);
	auto bounds = m_DraggedNode->GetChilds()[0]->GetModelsComponent()->GetModels()[0]->GetBounds();
	auto pos = GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), bounds.getMax().y / 2.0f));

	pos -= bounds.getCenter();
	m_DraggedNode->SetTranslate(FixBoxCoords(pos));

	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z));
	m_DraggerTextUI->SetTranslate(MousePos + glm::vec2(0.0f, - 15.0f));
}

void CEdtor3DFrame::DoDropNodeAndCreateIt()
{
	if (!m_IsDraggingEnabled)
		return;

	const auto& childs = m_DraggedNode->GetChilds();
	if (childs.empty())
		throw CException("TEST FUCK!!!!");

	auto firstChild = *childs.begin();
	firstChild->SetTranslate(m_DraggedNode->GetTranslation());
	GetRealRootNode3D()->AddChild(firstChild);
	Selector_SelectNode(firstChild);

	if (m_IsDraggingPermanentCreation)
	{
		auto copiedNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
		firstChild->Copy(copiedNode);
		copiedNode->SetTranslate(glm::vec3(0.0f));
		_ASSERT(m_DraggedNode->GetChilds().empty());
		m_DraggedNode->AddChild(copiedNode);
	}
}
