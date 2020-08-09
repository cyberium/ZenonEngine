#include "stdafx.h"

// Gerenal
#include "Editor3DPreviewScene.h"

CEditor3DPreviewScene::CEditor3DPreviewScene(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
{

}

CEditor3DPreviewScene::~CEditor3DPreviewScene()
{
}

void CEditor3DPreviewScene::SetModel(const std::shared_ptr<IModel>& Model)
{
	_ASSERT(Model != nullptr);

	auto modelComponent = m_Node->GetComponent<IModelsComponent3D>();
	if (modelComponent->GetModel())
		modelComponent->ResetModel();

	modelComponent->SetModel(Model);

	const auto& modelBBox = Model->GetBounds();
	float radius = modelBBox.getRadius();

	m_Node->SetTranslate(- modelBBox.getCenter());
	GetCameraController()->GetCamera()->SetTranslation(glm::vec3(radius * 2.0f));
	GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));

	/*ResizeEventArgs resizeArgs(nullptr, GetRenderWindow()->GetWindowWidth(), GetRenderWindow()->GetWindowHeight());
	OnWindowResize(resizeArgs);

	UpdateEventArgs uArgs(nullptr, 0.0f, 0.0f, 0.0f, nullptr, nullptr);

	RenderEventArgs rArgs(uArgs, nullptr);
	OnUpdate(rArgs);
	OnPreRender(rArgs);
	OnRender(rArgs);
	OnPostRender(rArgs);
	OnRenderUI(rArgs);
	GetRenderWindow()->Present();*/
}


//
// IGameState
//
void CEditor3DPreviewScene::Initialize()
{
	SceneBase::Initialize();

	auto cameraNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
	cameraNode->SetName("Camera");
	cameraNode->AddComponent(MakeShared(CCameraComponent3D, *cameraNode));

	SetCameraController(MakeShared(CFreeCameraController));
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 1.0f, 5000.0f);

	Load3D();

	GetRenderer()->AddPass(MakeShared(CUIColorPass, GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
	GetRenderer()->AddPass(MakeShared(CUIFontPass, GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}

void CEditor3DPreviewScene::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}

void CEditor3DPreviewScene::Load3D()
{
	//--------------------------------------------------------------------------
	// Lights
	//--------------------------------------------------------------------------

	{
		auto lightNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		lightNode->SetName("Light");
		lightNode->SetTranslate(glm::vec3(1500.0f, 1500.0f, 1500.0f));
		lightNode->SetRotation(glm::vec3(-0.9f, -0.9f, -0.9f));

		lightNode->AddComponent(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *lightNode.get()));
		lightNode->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		lightNode->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightNode->GetComponent<ILightComponent3D>()->SetRange(99000.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		lightNode->GetComponent<ILightComponent3D>()->SetSpotlightAngle(75.0f);
	}

	//m_LightsBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 8, sizeof(SLight), EAccess::CPUWrite);

	{
		m_Node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		m_Node->SetName("Node model preview.");
	}


	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, this);
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

	auto defferedRenderer = MakeShared(CRendererDeffered, GetBaseManager(), weak_from_this());
	defferedRenderer->Initialize(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport());
	SetRenderer(defferedRenderer);

	/*glm::vec4 color = glm::vec4(0.33, 0.33f, 0.33f, 1.0f);
	m_Technique3D.AddPass(MakeShared(ClearRenderTargetPass, GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), ClearFlags::All, color, 1.0f, 0));

	auto materialModelPass = GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("MaterialModelPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this());
	m_MaterialModelPass = std::dynamic_pointer_cast<IMaterialModelPass>(materialModelPass);

	{
		auto invokePass = GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("InvokePass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this());
		std::dynamic_pointer_cast<IInvokeFunctionPass>(invokePass)->SetFunc([this]() {

			std::vector<SLight> lights;
			auto lightStruct = std::dynamic_pointer_cast<ILight3D>(m_LightNode->GetComponent<ILightComponent3D>())->GetLightStruct();
			lightStruct.PositionVS = GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightStruct.PositionWS.xyz(), 1.0f);
			lightStruct.DirectionVS = glm::normalize(GetCameraController()->GetCamera()->GetViewMatrix() * glm::vec4(lightStruct.DirectionWS.xyz(), 0.0f));
			lights.push_back(lightStruct);

			if (lights.size() > m_LightsBuffer->GetElementCount())
				m_LightsBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(lights, EAccess::CPUWrite);
			else
				m_LightsBuffer->Set(lights);
			m_LightsCnt = lights.size();

			m_MaterialModelPass->GetLightsShaderParameter()->Set(m_LightsBuffer);
		});
		m_Technique3D.AddPass(invokePass);
	}

	m_Technique3D.AddPass(materialModelPass);*/
}

void CEditor3DPreviewScene::LoadUI()
{

}
