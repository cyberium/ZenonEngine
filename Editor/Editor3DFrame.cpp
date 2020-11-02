#include "stdafx.h"

// Gerenal
#include "Editor3DFrame.h"

// Additional
#include "Passes/DrawToolsPass.h"

CEditor3DFrame::CEditor3DFrame(IEditor& Editor, IRenderWindow& RenderWindow)
	: SceneBase(Editor.GetBaseManager(), RenderWindow)
	, m_Editor(Editor)
{
	dynamic_cast<IEditorPrivate&>(m_Editor).Set3DFrame(this);

	m_EditedScene = MakeShared(CEditedScene, Editor.GetBaseManager(), RenderWindow);
	m_EditedScene->Initialize();
}

CEditor3DFrame::~CEditor3DFrame()
{
	Log::Info("CEditor3DFrame deleted.");
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

	GetRootNode3D()->AddChild(m_EditedScene->GetRootNode3D());

	glm::vec3 rtsCenter = glm::vec3(cCellSize * cCellsCount) / 2.0f;

	// Light
	{
		auto lightNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		lightNode->SetName("Light");
		lightNode->SetTranslate(rtsCenter);
		lightNode->SetRotation(glm::vec3(0.f, -0.01f, 0.0f));

		lightNode->AddComponent(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *lightNode.get()));
		lightNode->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		lightNode->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightNode->GetComponent<ILightComponent3D>()->SetRange(5000.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetSpotlightAngle(75.0f);
	}

	// Camera
	{
		auto cameraNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
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
		GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 5000.0f);
		GetCameraController()->GetCamera()->SetTranslation(glm::vec3(15.0f * 2.0f));
		//GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
		//GetCameraController()->GetCamera()->SetYaw(225);
		//GetCameraController()->GetCamera()->SetPitch(-45);
	}

	{
		/*
		glm::vec4 color = glm::vec4(0.33f, 0.33f, 0.33f, 1.0f);
		m_Technique3D.AddPass(MakeShared(ClearRenderTargetPass, GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), ClearFlags::All, color, 1.0f, 0));

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
		*/


		//--------------------------------------------------------------------------
		// Cube Gold
		//--------------------------------------------------------------------------
		/*for (int i = 0; i < 5; i++)
		{
			std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
			textMaterial->SetSpecularFactor(32.0f);
			//textMaterial->SetBumpFactor(16.0f);

			textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetRenderDevice().GetObjectsFactory().LoadTexture2D("pirate-gold-unity//pirate-gold_albedo.png"));
			textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetRenderDevice().GetObjectsFactory().LoadTexture2D("pirate-gold-unity//pirate-gold_normal-ogl.png"));
			textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetRenderDevice().GetObjectsFactory().LoadTexture2D("pirate-gold-unity//pirate-gold_ao.png"));
			textMaterial->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetRenderDevice().GetObjectsFactory().LoadTexture2D("pirate-gold-unity//pirate-gold_height.png"));

			auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
			modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());

			auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this, GetRootNode3D());
			node->SetName("Sphere2");
			node->SetTranslate(glm::vec3(-10, 15, 16 * i));
			node->SetScale(glm::vec3(15.0f));
			node->GetComponent<IModelsComponent3D>()->SetModel(modelPlane);
			//node->GetComponent<IModelsComponent3D>()->SetCastShadows(false);
		}*/

		auto forwardRenderer = MakeShared(CRendererForward, GetBaseManager(), weak_from_this());
		forwardRenderer->Initialize(GetRenderWindow().GetRenderTarget(), &GetRenderWindow().GetViewport());
		forwardRenderer->AddPass(MakeShared(CDrawToolsPass, GetRenderDevice(), shared_from_this())->ConfigurePipeline(GetRenderWindow().GetRenderTarget(), &GetRenderWindow().GetViewport()));
		SetRenderer(forwardRenderer);
	}

}

void CEditor3DFrame::Finalize()
{
	SceneBase::Finalize();
}

void CEditor3DFrame::AddChild(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	__super::AddChild(ParentNode, ChildNode);
}

void CEditor3DFrame::RemoveChild(const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	__super::RemoveChild(ParentNode, ChildNode);
}

void CEditor3DFrame::RaiseSceneChangeEvent(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode)
{
	if (IsChildOf(GetEditedRootNode3D(), ChildNode) || IsChildOf(GetEditedRootNode3D(), ParentNode))
		GetEditor().GetUIFrame().OnSceneChanged(SceneChangeType, ParentNode, ChildNode);
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
// IEditor3DFrame
//
IEditor& CEditor3DFrame::GetEditor() const
{
	return m_Editor;
}

bool CEditor3DFrame::InitializeEditorFrame()
{
	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		node->SetName("Grid node x1.");
		node->SetTranslate(glm::vec3(0.0f));
		node->SetScale(glm::vec3(1.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.5f, 0.5f, 0.5f, 0.3f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponent<IModelsComponent3D>()->SetModel(model);
	}

	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		node->SetName("Grid node x10.");
		node->SetTranslate(glm::vec3(0.0f, 0.00f, 0.0f));
		node->SetScale(glm::vec3(10.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.6f, 0.6f, 0.6f, 0.5f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponent<IModelsComponent3D>()->SetModel(model);
	}

	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		node->SetName("Grid node x100.");
		node->SetTranslate(glm::vec3(0.0f, 0.00f, 0.0f));
		node->SetScale(glm::vec3(100.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines(10);

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.7f, 0.7f, 0.7f, 0.7f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponent<IModelsComponent3D>()->SetModel(model);
	}

	return true;
}

void CEditor3DFrame::DoInitializeTools3D()
{
	GetEditor().GetTools().DoInitialize3D(m_Renderer, GetRenderWindow().GetRenderTarget(), &GetRenderWindow().GetViewport());
}

IScene& CEditor3DFrame::GetScene()
{
	return *this;
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

std::shared_ptr<ISceneNode> CEditor3DFrame::GetEditedRootNode3D() const
{
	return m_EditedScene->GetRootNode3D();
}

std::shared_ptr<ISceneNode> CEditor3DFrame::GetEditedNodeUnderMouse(const glm::ivec2& MousePos) const
{
	auto ray = GetCameraController()->ScreenToRay(GetRenderWindow().GetViewport(), MousePos);
	auto nodes = GetFinder().FindIntersection(ray, nullptr, GetEditedRootNode3D());
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
// IEditorToolSelectorEventListener
//
void CEditor3DFrame::OnSelectNode()
{

	//m_Tools.m_Mover->Disable();
}
