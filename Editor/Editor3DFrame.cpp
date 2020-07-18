#include "stdafx.h"

// Gerenal
#include "Editor3DFrame.h"

CEdtor3DFrame::CEdtor3DFrame(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
	, m_Selector(this)

	, m_EditorUI(nullptr)
	, m_IsDraggingEnabled(false)
	, m_IsDraggingPermanentCreation(false)
	, m_MoverValue(1.0f)
	, m_MoverNuber(-1)
	, m_IsMoverEnable(false)
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

	m_Selector.SetOtherSelector(m_EditorUI->GetNodesSelector());
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
	cameraNode->AddComponent(std::make_shared<CCameraComponent3D>(*cameraNode));

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 1.0f, 5000.0f);

	m_LightsBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 8, sizeof(SLight), EAccess::CPUWrite);

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

	if (Parent->GetGUID() == Child->GetGUID())
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
		for (const auto& node : m_Selector.GetSelectedNodes())
		{
			if (node == ChildNode)
			{
				m_Selector.RemoveNode(ChildNode);
			}
		}
	}

	if (IsChildOf(GetRealRootNode3D(), ChildNode) || IsChildOf(GetRealRootNode3D(), OwnerNode))
		m_EditorUI->OnSceneChanged();
}

bool CEdtor3DFrame::OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (e.Button == MouseButtonEventArgs::MouseButton::Left)
	{
		if (m_IsDraggingEnabled)
		{
			DoDropNodeAndCreateIt();
			return true;
		}

		auto nodes = FindIntersection(RayToWorld);
		if (nodes.empty())
		{
			m_SelectionPrevPos = e.GetPoint();
			m_SelectionTexture->SetTranslate(e.GetPoint());
			m_IsSelecting = true;
			return true;
		}

		// MOVER BEGIN
		m_MovingNode = m_Selector.GetFirstSelectedNode();
		if (m_MovingNode != nullptr && !IsChildOf(m_MoverRoot, m_MovingNode))
		{
			{
				auto pos = GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), m_MovingNode->GetTranslation().y));
				auto cameraPosX0Z = GetCameraController()->GetCamera()->GetTranslation();
				cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
				auto movedObjectPosX0Z = glm::vec3(m_MovingNode->GetTranslation().x, 0.0f, m_MovingNode->GetTranslation().z);
				auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);
				auto posYYY = GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
				m_MoverOffset = m_MoverRoot->GetTranslation() - glm::vec3(pos.x, posYYY.y, pos.z);
			}

			for (const auto& it : nodes)
			{
				if (it.second == m_MoverX)
				{
					m_MoverNuber = 1;
				}
				else if (it.second == m_MoverY)
				{
					m_MoverNuber = 2;
				}
				else if (it.second == m_MoverZ)
				{
					m_MoverNuber = 3;
				}

				if (m_MoverNuber > 0)
				{
					m_MovingObjectPos = m_MovingNode->GetTranslation();
					m_MoverRoot->SetScale(glm::vec3(m_MovingNode->GetColliderComponent()->GetBounds().getRadius() * 1.0f / 10.0f));
					m_IsMoverEnable = true;
					return true;
				}
			}
		}
		// MOVER END




		auto node = nodes.begin()->second;
		_ASSERT(node != nullptr);

		if (e.Control)
		{
			if (m_Selector.IsNodeSelected(node))
				m_Selector.RemoveNode(node);
			else
				m_Selector.AddNode(node);
		}
		else
		{
			m_Selector.SelectNode(nodes.begin()->second);
			m_MoverRoot->SetTranslate(nodes.begin()->second->GetTranslation());
		}
		return  true;
	}
	else if (e.Button == MouseButtonEventArgs::MouseButton::Right)
	{
		if (m_IsDraggingEnabled)
		{
			DragLeaveEvent();
			return true;
		}
	}

	return false;
}

void CEdtor3DFrame::OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (e.Button == MouseButtonEventArgs::MouseButton::Left)
	{
		if (m_IsMoverEnable)
		{
			m_MoverNuber = 0;
			//m_MoverRoot->SetScale(glm::vec3(0.001f));
			m_IsMoverEnable = false;
			m_MovingNode = nullptr;
			m_MovingObjectPos = glm::vec3(0.0f);
		}

		auto cachedSelectionPrevPos = m_SelectionPrevPos;

		// Clear
		m_SelectionTexture->SetScale(glm::vec3(0.001f));
		m_SelectionPrevPos = e.GetPoint();

		if (m_IsSelecting)
		{
			if (glm::length(glm::abs(cachedSelectionPrevPos - e.GetPoint())) > 10.0f)
			{
				Frustum f;
				f.buildBoxFrustum(
					GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), glm::vec2(glm::min(cachedSelectionPrevPos.x, e.GetPoint().x), glm::min(cachedSelectionPrevPos.y, e.GetPoint().y))),
					GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), glm::vec2(glm::min(cachedSelectionPrevPos.x, e.GetPoint().x), glm::max(cachedSelectionPrevPos.y, e.GetPoint().y))),
					GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), glm::vec2(glm::max(cachedSelectionPrevPos.x, e.GetPoint().x), glm::min(cachedSelectionPrevPos.y, e.GetPoint().y))),
					GetCameraController()->ScreenToRay(GetRenderWindow()->GetViewport(), glm::vec2(glm::max(cachedSelectionPrevPos.x, e.GetPoint().x), glm::max(cachedSelectionPrevPos.y, e.GetPoint().y))),
					15000.0f
				);

				auto nodes = FindIntersections(f);
				if (!nodes.empty())
					m_Selector.SelectNodes(nodes);
			}
			m_IsSelecting = false;
		}
	}
}

void CEdtor3DFrame::OnMouseMoveToWorld(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{

	if (m_IsDraggingEnabled)
	{
		DoMoveNode(e.GetPoint());
		return;
	}

	if (e.LeftButton)
	{
		if (m_IsMoverEnable)
		{
			glm::vec3 oldPos = m_MovingNode->GetTranslation();
			glm::vec3 newPos = glm::vec3(0.0f);

			if (m_MoverNuber == 1)
			{
				auto mousePos = GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
				newPos = glm::vec3(mousePos.x + m_MoverOffset.x, oldPos.y, oldPos.z);
			}
			else if (m_MoverNuber == 3)
			{
				auto mousePos = GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
				newPos = glm::vec3(oldPos.x, oldPos.y, mousePos.z + m_MoverOffset.z);
			}
			else if (m_MoverNuber == 2)
			{
				auto cameraPosX0Z = GetCameraController()->GetCamera()->GetTranslation();
				cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
				auto movedObjectPosX0Z = glm::vec3(oldPos.x, 0.0f, oldPos.z);
				auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);

				auto mousePos = GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
				newPos = glm::vec3(oldPos.x, mousePos.y + m_MoverOffset.y, oldPos.z);
			}

			m_MovingNode->SetTranslate(FixBoxCoords(newPos));
			m_MoverRoot->SetTranslate(m_MovingNode->GetTranslation());

			// Refresh selection bounds
			m_DrawSelectionPass->SetNeedRefresh();
			return;
		}

		if (m_IsSelecting)
		{
			glm::vec2 scale = e.GetPoint() - m_SelectionTexture->GetTranslation();
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
// IEditorSharedFrame
//
IEditor_NodesSelector * CEdtor3DFrame::GetNodesSelector()
{
	return &m_Selector;
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
// IEditor_NodesSelectorEventListener
//
void CEdtor3DFrame::OnSelectNodes()
{
	m_DrawSelectionPass->SetNeedRefresh();
}



//
// CSceneNodesSelector
//

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
	GetDefaultLight()->SetType(ELightType::Spot);
	GetDefaultLight()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	GetDefaultLight()->SetRange(99000.0f);
	GetDefaultLight()->SetIntensity(1.0f);
	GetDefaultLight()->SetSpotlightAngle(75.0f);

	{
		m_DraggedNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		m_DraggedNode->SetName("Dragged node parent.");
	}

	{
		m_MoverRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, GetRootNode3D());
		m_MoverRoot->SetName("Mover");

		std::shared_ptr<ISceneNode3D> sceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode_FBXNode, this);

		std::shared_ptr<IFBXSceneNode3D> fbxSceneNode = std::dynamic_pointer_cast<IFBXSceneNode3D>(sceneNode);
		fbxSceneNode->InitializeFromFile("C:\\_engine\\ZenonEngine_gamedata\\arrow.FBX");

		auto model = std::dynamic_pointer_cast<IFBXSceneNode3D>(sceneNode->GetChilds().at(0))->GetModel();
		auto geom = model->GetConnections().begin()->Geometry;
		if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
		{
			std::shared_ptr<IFile> file = std::make_shared<CFile>("C:\\_engine\\ZenonEngine_gamedata\\arrow.znmdl");
			loadable->Save(file);

			GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("PCEveryFileAccess")->SaveFile(file);
		}

		// Mover node
		{
			auto materialX = std::make_shared<MaterialDebug>(GetRenderDevice());
			materialX->SetDiffuseColor(glm::vec4(1.0f, 0.2f, 0.1f, 1.0f));
			auto modelX = GetRenderDevice().GetObjectsFactory().CreateModel();
			modelX->AddConnection(materialX, geom);

			auto materialY = std::make_shared<MaterialDebug>(GetRenderDevice());
			materialY->SetDiffuseColor(glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
			auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
			modelY->AddConnection(materialY, geom);

			auto materialZ = std::make_shared<MaterialDebug>(GetRenderDevice());
			materialZ->SetDiffuseColor(glm::vec4(0.1f, 0.2f, 1.0f, 1.0f));
			auto modelZ = GetRenderDevice().GetObjectsFactory().CreateModel();
			modelZ->AddConnection(materialZ, geom);


			m_MoverX = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, m_MoverRoot);
			m_MoverX->SetName("Mover_X");
			m_MoverX->SetRotation(glm::vec3(0.0f, 0.0f, -glm::half_pi<float>()));
			//moverX->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
			m_MoverX->GetModelsComponent()->AddModel(modelX);
			m_MoverX->GetColliderComponent()->SetBounds(model->GetBounds());

			m_MoverY = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, m_MoverRoot);
			m_MoverY->SetName("Mover_Y");
			//moverY->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
			m_MoverY->GetModelsComponent()->AddModel(modelY);
			m_MoverY->GetColliderComponent()->SetBounds(model->GetBounds());

			m_MoverZ = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, m_MoverRoot);
			m_MoverZ->SetName("Mover_Z");
			//moverZ->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
			m_MoverZ->SetRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
			m_MoverZ->GetModelsComponent()->AddModel(modelZ);
			m_MoverZ->GetColliderComponent()->SetBounds(model->GetBounds());
		}


		sceneNode->GetParent().lock()->RemoveChild(sceneNode);
	}

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

	{
		m_DrawSelectionPass = std::make_shared<CDrawSelectionPass>(GetRenderDevice(), m_Selector);
		m_DrawSelectionPass->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
		m_Technique3D.AddPass(m_DrawSelectionPass);
	}
}

void CEdtor3DFrame::LoadUI()
{
	m_DraggerTextUI = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Text);
	GetRootNodeUI()->AddChild(m_DraggerTextUI);

	m_SelectionTexture = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(this, cSceneNodeUI_Color, GetRootNodeUI());
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
	pos = FixBoxCoords(pos);
	
	m_DraggedNode->SetTranslate(pos);

	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z));
	m_DraggerTextUI->SetTranslate(MousePos + glm::vec2(0.0f, -15.0f));
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
	m_Selector.SelectNode(firstChild);

	if (m_IsDraggingPermanentCreation)
	{
		auto copiedNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		firstChild->Copy(copiedNode);
		copiedNode->SetTranslate(glm::vec3(0.0f));
		_ASSERT(m_DraggedNode->GetChilds().empty());
		m_DraggedNode->AddChild(copiedNode);
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