#include "stdafx.h"

// General
#include "GameState_World.h"

// Additional

CGameState_World::CGameState_World(const IApplication * _application, std::shared_ptr<IRenderWindow> RenderWindow)
    : base(_application, RenderWindow)
{}

CGameState_World::~CGameState_World()
{}


//
// IGameState
//
bool CGameState_World::Init()
{
	base::Init();

    SetCameraController(std::make_shared<CFreeCameraController>());
    GetCameraController()->GetCamera()->SetProjection(ICamera::ProjectionHand::Right, 45.0f, GetRenderWindow()->GetWindowWidth() / GetRenderWindow()->GetWindowHeight(), 0.5f, 4000.0f);

    Load3D();
    LoadUI();

	GetCameraController()->GetCameraMovement()->SetTranslate(vec3(-104, 105, 105));
	GetCameraController()->GetCameraMovement()->SetYaw(-45);
	GetCameraController()->GetCameraMovement()->SetPitch(-45);

	return true;
}

void CGameState_World::Destroy()
{
	// Insert code here

    base::Destroy();
}


//
//
//

void CGameState_World::OnResize(ResizeEventArgs& e)
{
	base::OnResize(e);
}

void CGameState_World::OnPreRender(RenderEventArgs& e)
{
	base::OnPreRender(e);
}

void CGameState_World::OnRender(RenderEventArgs& e)
{
	base::OnRender(e);

	Application::Get().GetLoader()->SetCamera(GetCameraController()->GetCamera());
}

void CGameState_World::OnPostRender(RenderEventArgs& e)
{
	base::OnPostRender(e);


}

void CGameState_World::OnRenderUI(RenderEventArgs& e)
{
	base::OnRenderUI(e);
}

//
//
//

void CGameState_World::Load3D()
{   
	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

	std::shared_ptr<IPipelineState> UIPipeline = _RenderDevice->CreatePipelineState();
	UIPipeline->GetBlendState()->SetBlendMode(disableBlending);
	UIPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	UIPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	UIPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	UIPipeline->SetRenderTarget(GetRenderWindow()->GetRenderTarget());
	UIPipeline->GetRasterizerState()->SetViewport(GetRenderWindow()->GetViewport());

	std::shared_ptr<SceneNode3D> sceneNode = m_3DScene->CreateSceneNode<SceneNode3D>(m_3DScene->GetRootNode());

	std::shared_ptr<IMesh> mesh = _RenderDevice->GetPrimitiveCollection()->CreatePlane();

	std::shared_ptr<MaterialTextured> mat = std::make_shared<MaterialTextured>();
	mat->SetDiffuseColor(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	mat->SetTexture(0, _RenderDevice->CreateTexture2D("default.png"));
	mesh->SetMaterial(mat);

	sceneNode->GetComponent<CTransformComponent3D>()->SetScale(vec3(15, 15, 15));
	sceneNode->GetComponent<CMeshComponent3D>()->AddMesh(mesh);


	//CFBX fbx(m_3DScene->GetRootNode());


    m_3DTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(GetRenderWindow()->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));
	m_3DTechnique.AddPass(std::make_shared<BasePass>(m_3DScene, UIPipeline));
}

void CGameState_World::LoadUI()
{
    AddUIPasses(_RenderDevice, GetRenderWindow()->GetRenderTarget(), &m_UITechnique, GetRenderWindow()->GetViewport(), m_UIScene);
}
