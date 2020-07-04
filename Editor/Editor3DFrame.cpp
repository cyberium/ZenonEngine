#include "stdafx.h"

// Gerenal
#include "Editor3DFrame.h"

CSceneEditor::CSceneEditor(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
	, m_EditorUI(nullptr)
	, m_IsSelecting(false)
{
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
	return m_FakeRootNode3D;
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
	auto childs = m_RootForSelectedBBoxes->GetChilds();
	for (const auto& ch : childs)
		RemoveChild(m_RootForSelectedBBoxes, ch);

	for (const auto& it : Selector_GetSelectedNodes())
		CreateSelectionBBoxNodeForNode(it);
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

std::shared_ptr<ISceneNode3D> CSceneEditor::CreateNode(const glm::ivec3& Position, int32 type)
{
	auto it = std::find_if(m_Nodes.begin(), m_Nodes.end(), [&Position] (const CSceneEditor::SNode& Object) -> bool {
		return (Object.X == Position.x) && (Object.Y == Position.y) && (Object.Z == Position.z);
	});

	if (it != m_Nodes.end())
		return it->SceneNode;

	auto node = CreateSceneNode<SceneNode3D>(GetRootNode3D());
	node->SetTranslate(glm::vec3(Position));
	node->SetScale(glm::vec3(25.0f));
	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
		loadable->Load(GetBaseManager().GetManager<IFilesManager>()->Open("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_grass.fbx.znmdl"));

	node->GetComponent<IModelsComponent3D>()->AddModel(model);
	node->GetComponent<IColliderComponent3D>()->SetBounds(/*BoundingBox(glm::vec3(-5.0f), glm::vec3(5.0f))*/model->GetBounds());

	return node;
}

std::shared_ptr<ISceneNode3D> CSceneEditor::CreateSelectionBBoxNodeForNode(const std::shared_ptr<ISceneNode3D>& Node)
{
	if (Node == nullptr)
		return nullptr;

	auto selectionBBoxNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, m_RootForSelectedBBoxes);
	selectionBBoxNode->SetName("Selector for [" + Node->GetName() + "]");
	auto geometry = GetRenderDevice().GetPrimitivesFactory().CreateBBox();
	auto material = std::make_shared<MaterialDebug>(GetRenderDevice());
	material->SetDiffuseColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	model->AddConnection(material, geometry);
	selectionBBoxNode->GetComponent<IModelsComponent3D>()->AddModel(model);

	const auto& bbox = Node->GetComponent<IColliderComponent3D>()->GetBounds();
	auto size = glm::abs(bbox.getMax() - bbox.getMin());

	selectionBBoxNode->SetTranslate(Node->GetTranslation() + bbox.getMin() - 0.1f * size);
	selectionBBoxNode->SetScale(size * 1.2f);

	return selectionBBoxNode;
}

void CSceneEditor::Load3D()
{
	m_FakeRootNode3D = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
	m_FakeRootNode3D->SetName("SceneRoot");

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

	auto fileNames = Utils::GetAllFilesInDirectory("C:\\_engine\\ZenonEngine_gamedata\\models", ".znmdl");

	std::vector<std::string> modelsList;
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirt.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiver.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverBanks.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCornerBank.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCornerInner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCrossing.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverEnd.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverEntrance.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverRocks.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverSide.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverSideCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverT.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverTile.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverWater.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_grass.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathBend.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathBendBank.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathCornerSmall.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathCross.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathEnd.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathEndClosed.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathOpen.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathRocks.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathSide.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathSideOpen.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathSplit.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathStraight.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathTile.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverBend.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverBendBank.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverCornerSmall.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverCross.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverEnd.fbx");

	if (!fileNames.empty())
	{
		auto it = fileNames.begin();
		auto sizeSqrtDouble = glm::sqrt(fileNames.size());
		size_t sizeSqrt = glm::round(sizeSqrtDouble);
		//sizeSqrt = 6;

		for (size_t x = 0; x < sizeSqrt; x++)
		{
			for (size_t y = 0; y < sizeSqrt; y++)
			{
				if (it == fileNames.end())
					continue;

				auto fileName = (*it++);

				Log::Info(fileName.c_str());

				try
				{
					auto name = CFile(fileName + ".znmdl").Name();
					name = name.substr(0, name.find_first_of('.'));

					std::shared_ptr<ISceneNode3D> sceneNodeParent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRealRootNode3D());
					sceneNodeParent->SetName(name);
					sceneNodeParent->SetTranslate(500.0f + glm::vec3(float(x) * 40.0f, 0.0f, float(y) * 40.0f));

					if (GetBaseManager().GetManager<IFilesManager>()->IsFileExists(fileName))
					{
						auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
						if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
						{
							loadable->Load(GetBaseManager().GetManager<IFilesManager>()->Open(fileName));
						}

						sceneNodeParent->GetComponent<IModelsComponent3D>()->AddModel(model);
						sceneNodeParent->GetComponent<IColliderComponent3D>()->SetBounds(model->GetBounds());
						continue;
					}
				}
				catch (const CException& e)
				{
					Log::Error(e.MessageCStr());
				}
			}
		}
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
		m_RootForSelectedBBoxes = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
		m_RootForSelectedBBoxes->SetName("Root for BBox");
	}

	glm::vec4 color = glm::vec4(0.0, 0.0f, 0.0f, 1.0f);
	m_Technique3D.AddPass(std::make_shared<ClearRenderTargetPass>(GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), ClearFlags::All, color /*glm::vec4(0.2f, 0.2f, 0.2f, 0.2f)*/, 1.0f, 0));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassOpaque", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassTransperent", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	//m_Technique3D.AddPass(std::make_shared<CDrawBoundingBoxPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("DebugPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
}

void CSceneEditor::LoadUI()
{
	m_SelectionTexture = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>(ofkSceneNodeUI)->CreateSceneNodeUI(this, cSceneNodeUI_Color, GetRootNodeUI());
	m_SelectionTexture->GetProperties()->GetPropertyT<glm::vec4>("Color")->Set(glm::vec4(0.1f, 0.3f, 1.0f, 0.3f));

	m_TechniqueUI.AddPass(std::make_shared<CUIColorPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}
