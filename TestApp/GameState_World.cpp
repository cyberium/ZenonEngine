#include "stdafx.h"

// General
#include "GameState_World.h"

// Additional

CGameState_World::CGameState_World(const IApplication * _application, std::shared_ptr<IRenderWindow> RenderWindow)
    : base(_application, RenderWindow)
{
}

CGameState_World::~CGameState_World()
{
}


//
// IGameState
//
bool CGameState_World::Init()
{
	base::Init();

    IApplication& app = Application::Get();
    std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();

    //
    // Camera controller
    //
    SetCameraController(std::make_shared<CFreeCameraController>());
    GetCameraController()->GetCamera()->SetViewport(GetRenderWindow()->GetViewport());
    GetCameraController()->GetCamera()->SetProjectionRH(45.0f, 1280.0f / 1024.0f, 0.5f, 4000.0f);

    m_FrameQuery = renderDevice->CreateQuery(IQuery::QueryType::Timer, 1);

    Load3D();
    LoadUI();

	GetCameraController()->GetCamera()->SetTranslate(vec3(-104, 105, 105));
	GetCameraController()->GetCamera()->SetYaw(-45);
	GetCameraController()->GetCamera()->SetPitch(-45);

	return true;
}

void CGameState_World::Destroy()
{
    base::Destroy();
}


//
//
//

void CGameState_World::OnResize(ResizeEventArgs& e)
{
    if (e.Width == 0 || e.Height == 0)
        return;

	base::OnResize(e);

    m_3DTechnique.UpdateViewport(dynamic_cast<const IRenderWindow*>(e.Caller)->GetViewport());
    m_UITechnique.UpdateViewport(dynamic_cast<const IRenderWindow*>(e.Caller)->GetViewport());
}

void CGameState_World::OnPreRender(RenderEventArgs& e)
{
    m_FrameQuery->Begin(e.FrameCounter);

}

void CGameState_World::OnRender(RenderEventArgs& e)
{
    e.Camera = GetCameraController()->GetCamera().get();
    Application::Get().GetLoader()->SetCamera(GetCameraController()->GetCamera());

    m_3DTechnique.Render(e);
}

void CGameState_World::OnPostRender(RenderEventArgs& e)
{
    m_FrameQuery->End(e.FrameCounter);

    vec3 cameraTrans = GetCameraController()->GetCamera()->GetTranslation();
    m_CameraPosText->SetText("Pos: " + std::to_string(cameraTrans.x) + ", " + std::to_string(cameraTrans.y) + ", " + std::to_string(cameraTrans.z));

    m_CameraRotText->SetText("Rot: yaw=" + std::to_string(GetCameraController()->GetCamera()->GetYaw()) + ", pitch=" + std::to_string(GetCameraController()->GetCamera()->GetPitch()));

    IQuery::QueryResult frameResult = m_FrameQuery->GetQueryResult(e.FrameCounter - (m_FrameQuery->GetBufferCount() - 1));
    if (frameResult.IsValid)
    {
        // Frame time in milliseconds
#ifdef  IS_DX11
        m_FrameTime = frameResult.ElapsedTime * 1000.0;
#else
        m_FrameTime = frameResult.ElapsedTime / 1000000.0;
#endif

        std::string title = std::to_string(m_FrameTime);

        //Application::Get().GetRenderWindow()->SetWindowName(title);
    }
}

void CGameState_World::OnRenderUI(RenderEventArgs& e)
{
	e.Camera = GetCameraController()->GetCamera().get();

    m_UITechnique.Render(e);
}

//
//
//

void CGameState_World::Load3D()
{
    IApplication& app = Application::Get();
    std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();
    
	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

	std::shared_ptr<IPipelineState> UIPipeline = renderDevice->CreatePipelineState();
	UIPipeline->GetBlendState()->SetBlendMode(disableBlending);
	UIPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	UIPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	UIPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
	UIPipeline->SetRenderTarget(GetRenderWindow()->GetRenderTarget());
	UIPipeline->GetRasterizerState()->SetViewport(GetRenderWindow()->GetViewport());

	std::shared_ptr<SceneNode3D> sceneNode = m_3DScene->CreateSceneNode<SceneNode3D>(m_3DScene->GetRootNode());

	std::shared_ptr<IMesh> mesh = renderDevice->GetPrimitiveCollection()->CreatePlane();

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
    IApplication& app = Application::Get();
    std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();

    // Font
    m_CameraPosText = m_UIScene->GetRootNode()->CreateSceneNode<CUITextNode>();
    m_CameraPosText->SetText("Camera position");
    m_CameraPosText->GetComponent<CTransformComponentUI>()->SetTranslate(vec2(0.0f, 0.0f));

    m_CameraRotText = m_UIScene->GetRootNode()->CreateSceneNode<CUITextNode>();
    m_CameraRotText->SetText("Camera rotation");
    m_CameraRotText->GetComponent<CTransformComponentUI>()->SetTranslate(vec2(0.0f, 20.0f));

    //
    // UI Passes
    //
    AddUIPasses(renderDevice, GetRenderWindow()->GetRenderTarget(), &m_UITechnique, GetRenderWindow()->GetViewport(), m_UIScene);
}
