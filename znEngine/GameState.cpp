#include "stdafx.h"

// General
#include "GameState.h"

// Additional
#include "Application.h"

CGameState::CGameState(const IApplication * _application)
	: m_Application(_application)
	, m_IsInited(false)
	, m_IsCurrent(false)
	, m_QualitySettings(GetSettingsGroup<CGroupQuality>())
	, m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{
	m_3DScene = std::make_shared<Scene3D>();
	m_UIScene = std::make_shared<CUIScene>();
}

CGameState::~CGameState()
{}



//
// IGameState
//
bool CGameState::Init()
{
    m_IsInited = true;

    return true;
}

void CGameState::Destroy()
{

}

bool CGameState::Set()
{
	std::shared_ptr<RenderWindow> renderWindow = m_Application->GetRenderWindow();
    _ASSERT(renderWindow);

	// Input events connections
    OnKeyPressedConnection          = renderWindow->KeyPressed.connect(&CGameState::OnKeyPressed, this, std::placeholders::_1);
    OnKeyReleasedConnection         = renderWindow->KeyReleased.connect(&CGameState::OnKeyReleased, this, std::placeholders::_1);
    OnMouseButtonPressedConnection  = renderWindow->MouseButtonPressed.connect(&CGameState::OnMouseButtonPressed, this, std::placeholders::_1);
    OnMouseButtonReleasedConnection = renderWindow->MouseButtonReleased.connect(&CGameState::OnMouseButtonReleased, this, std::placeholders::_1);
    OnMouseMovedConnection          = renderWindow->MouseMoved.connect(&CGameState::OnMouseMoved, this, std::placeholders::_1);
    OnMouseWheelConnection          = renderWindow->MouseWheel.connect(&CGameState::OnMouseWheel, this, std::placeholders::_1);

	// Window events connections
    OnResizeConnection              = renderWindow->Resize.connect(&CGameState::OnResize, this, std::placeholders::_1);

	// Update events connection
    OnUpdateConnection              = renderWindow->Update.connect(&CGameState::OnUpdate, this, std::placeholders::_1);

	// Render events connections
    OnPreRenderConnection           = renderWindow->PreRender.connect(&CGameState::OnPreRender, this, std::placeholders::_1);
    OnRenderConnection              = renderWindow->Render.connect(&CGameState::OnRender, this, std::placeholders::_1);
    OnPostRenderConnection          = renderWindow->PostRender.connect(&CGameState::OnPostRender, this, std::placeholders::_1);
    OnRenderUIConnection            = renderWindow->RenderUI.connect(&CGameState::OnRenderUI, this, std::placeholders::_1);

    return true;
}

void CGameState::Unset()
{
    std::shared_ptr<RenderWindow> renderWindow = m_Application->GetRenderWindow();
    _ASSERT(renderWindow);

    renderWindow->Update.disconnect(OnUpdateConnection);
    renderWindow->MouseButtonPressed.disconnect(OnMouseButtonPressedConnection);
    renderWindow->MouseButtonReleased.disconnect(OnMouseButtonReleasedConnection);
    renderWindow->MouseMoved.disconnect(OnMouseMovedConnection);
    renderWindow->MouseWheel.disconnect(OnMouseWheelConnection);
    renderWindow->KeyPressed.disconnect(OnKeyPressedConnection);
    renderWindow->KeyReleased.disconnect(OnKeyReleasedConnection);
}



//
// Input events
//
void CGameState::OnKeyPressed(KeyEventArgs & e)
{
    bool result = false;
    if (m_UIScene)
        result = m_UIScene->OnKeyPressed(e);

	if (m_DefaultCameraController && !result)
		m_DefaultCameraController->OnKeyPressed(e);
}

void CGameState::OnKeyReleased(KeyEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnKeyReleased(e);

	if (m_UIScene)
		m_UIScene->OnKeyReleased(e);
}

void CGameState::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    bool result = false;
    if (m_UIScene)
        result = m_UIScene->OnMouseButtonPressed(e);

	if (m_DefaultCameraController && !result)
		m_DefaultCameraController->OnMouseButtonPressed(e);
}

void CGameState::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseButtonReleased(e);

	if (m_UIScene)
		m_UIScene->OnMouseButtonReleased(e);
}

void CGameState::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseMoved(e);

	if (m_UIScene)
		m_UIScene->OnMouseMoved(e);
}

void CGameState::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseWheel(e);

	if (m_UIScene)
		m_UIScene->OnMouseWheel(e);
}



//
// Window events
//
void CGameState::OnResize(ResizeEventArgs & e)
{
	m_Viewport.Width = e.Width;
	m_Viewport.Height = e.Height;
	m_Viewport.OrthoMatrix = glm::ortho(0.0f, m_Viewport.Width, m_Viewport.Height, 0.0f, -1.0f, 1.0f);
}



//
// Update event
//
void CGameState::OnUpdate(UpdateEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnUpdate(e);
}




void CGameState::SetCameraController(std::shared_ptr<ICameraController> CameraController)
{
	_ASSERT(CameraController != nullptr);
	m_DefaultCameraController = CameraController;
}

void CGameState::UnsetCameraController()
{
	m_DefaultCameraController = nullptr;
}
