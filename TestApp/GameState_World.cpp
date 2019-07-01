#include "stdafx.h"

// General
#include "GameState_World.h"

// Additional

CGameState_World::CGameState_World(const IApplication * _application)
    : base(_application)
{
    m_Viewport = Viewport(0, 0, 1280.0f, 1024.0f);
}

CGameState_World::~CGameState_World()
{
}


//
// IGameState
//
bool CGameState_World::Init()
{
    IApplication& app = Application::Get();
    std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();


    //
    // Camera controller
    //
    SetCameraController(std::make_shared<CFreeCameraController>());
    GetCameraController()->GetCamera()->SetViewport(m_Viewport);
    GetCameraController()->GetCamera()->SetProjectionRH(45.0f, 1280.0f / 1024.0f, 0.5f, 4000.0f);

    m_FrameQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 1);

    Load3D();
    LoadUI();

    return base::Init();
}

void CGameState_World::Destroy()
{
    base::Destroy();
}


//
//
//

void CGameState_World::OnResize(ResizeEventArgs & e)
{
    if (e.Width == 0 || e.Height == 0)
    {
        return;
    }

    GetCameraController()->GetCamera()->SetProjectionRH(45.0f, static_cast<float>(e.Width) / static_cast<float>(e.Height), 0.5f, 4000.0f);

    base::OnResize(e);

    GetCameraController()->GetCamera()->SetViewport(m_Viewport);

    m_3DTechnique.UpdateViewport(m_Viewport);
    m_UITechnique.UpdateViewport(m_Viewport);
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

    vec3 cameraRot = GetCameraController()->GetCamera()->GetDirection();
    m_CameraRotText->SetText("Rot: " + std::to_string(cameraRot.x) + ", " + std::to_string(cameraRot.y) + ", " + std::to_string(cameraRot.z));

    Query::QueryResult frameResult = m_FrameQuery->GetQueryResult(e.FrameCounter - (m_FrameQuery->GetBufferCount() - 1));
    if (frameResult.IsValid)
    {
        // Frame time in milliseconds
#ifdef  IS_DX11
        m_FrameTime = frameResult.ElapsedTime * 1000.0;
#else
        m_FrameTime = frameResult.ElapsedTime / 1000000.0;
#endif

        std::string title = std::to_string(m_FrameTime);

        Application::Get().GetRenderWindow()->SetWindowName(title);
    }
}

void CGameState_World::OnRenderUI(RenderEventArgs& e)
{
    e.Viewport = &m_Viewport;

    m_UITechnique.Render(e);
}

//
//
//

void CGameState_World::Load3D()
{
    IApplication& app = Application::Get();
    std::shared_ptr<IRenderDevice> renderDevice = app.GetRenderDevice();
    std::shared_ptr<RenderWindow> renderWindow = app.GetRenderWindow();
    

    m_3DTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(renderWindow->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));

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
    AddUIPasses(renderDevice, app.GetRenderWindow()->GetRenderTarget(), &m_UITechnique, m_Viewport, m_UIScene);
}
