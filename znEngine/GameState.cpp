#include "stdafx.h"

// General
#include "GameState.h"

// Additional
#include "Application.h"
#include "SceneFunctional/Base/SceneBase.h"

CGameState::CGameState(const IApplication * _application, std::shared_ptr<IRenderWindow> RenderWindow)
	: m_Application(_application)
	, m_RenderWindow(RenderWindow)
	, m_IsInited(false)
	, m_IsCurrent(false)
{
}

CGameState::~CGameState()
{}



//
// IGameState
//
bool CGameState::Init()
{
	m_VideoSettings = GetManager<ISettings>(m_Application->GetBaseManager())->GetGroup("Video");

	m_FrameQuery = GetApplication()->GetRenderDevice()->CreateQuery(IQuery::QueryType::Timer, 1);

	m_Scene = std::make_shared<SceneBase>(m_Application->GetBaseManager());
	m_Scene->CreateRootNode();

	{
		m_CameraPosText = m_Scene->GetRootNode()->CreateSceneNode<CUITextNode>();
		m_CameraPosText->GetComponent<ITransformComponentUI>()->SetTranslate(vec2(0.0f, 0.0f));

		m_CameraRotText = m_Scene->GetRootNode()->CreateSceneNode<CUITextNode>();
		m_CameraRotText->GetComponent<ITransformComponentUI>()->SetTranslate(vec2(0.0f, 20.0f));

		m_FPSText = m_Scene->GetRootNode()->CreateSceneNode<CUITextNode>();
		m_FPSText->GetComponent<ITransformComponentUI>()->SetTranslate(vec2(0.0f, 40.0f));
	}

    m_IsInited = true;

    return true;
}

void CGameState::Destroy()
{

}

bool CGameState::Set()
{
	std::shared_ptr<IRenderWindowEvents> renderWindow = std::dynamic_pointer_cast<IRenderWindowEvents>(m_RenderWindow);
    _ASSERT(renderWindow);

	// Input events connections
    OnKeyPressedConnection          = renderWindow->KeyPressed().connect(&CGameState::OnKeyPressed, this, std::placeholders::_1);
    OnKeyReleasedConnection         = renderWindow->KeyReleased().connect(&CGameState::OnKeyReleased, this, std::placeholders::_1);
    OnMouseButtonPressedConnection  = renderWindow->MouseButtonPressed().connect(&CGameState::OnMouseButtonPressed, this, std::placeholders::_1);
    OnMouseButtonReleasedConnection = renderWindow->MouseButtonReleased().connect(&CGameState::OnMouseButtonReleased, this, std::placeholders::_1);
    OnMouseMovedConnection          = renderWindow->MouseMoved().connect(&CGameState::OnMouseMoved, this, std::placeholders::_1);
    OnMouseWheelConnection          = renderWindow->MouseWheel().connect(&CGameState::OnMouseWheel, this, std::placeholders::_1);

	// Window events connections
    OnResizeConnection              = renderWindow->Resize().connect(&CGameState::OnResize, this, std::placeholders::_1);

	// Update events connection
    OnUpdateConnection              = renderWindow->Update().connect(&CGameState::OnUpdate, this, std::placeholders::_1);

	// Render events connections
    OnPreRenderConnection           = renderWindow->PreRender().connect(&CGameState::OnPreRender, this, std::placeholders::_1);
    OnRenderConnection              = renderWindow->Render().connect(&CGameState::OnRender, this, std::placeholders::_1);
    OnPostRenderConnection          = renderWindow->PostRender().connect(&CGameState::OnPostRender, this, std::placeholders::_1);
    OnRenderUIConnection            = renderWindow->RenderUI().connect(&CGameState::OnRenderUI, this, std::placeholders::_1);

    return true;
}

void CGameState::Unset()
{
    std::shared_ptr<IRenderWindowEvents> renderWindow = std::dynamic_pointer_cast<IRenderWindowEvents>(m_RenderWindow);
    _ASSERT(renderWindow);

    renderWindow->Update().disconnect(OnUpdateConnection);
    renderWindow->MouseButtonPressed().disconnect(OnMouseButtonPressedConnection);
    renderWindow->MouseButtonReleased().disconnect(OnMouseButtonReleasedConnection);
    renderWindow->MouseMoved().disconnect(OnMouseMovedConnection);
    renderWindow->MouseWheel().disconnect(OnMouseWheelConnection);
    renderWindow->KeyPressed().disconnect(OnKeyPressedConnection);
    renderWindow->KeyReleased().disconnect(OnKeyReleasedConnection);
}

void CGameState::SetInited(bool _value)
{
	m_IsInited = _value;
}

bool CGameState::IsInited() const
{
	return m_IsInited;
}

void CGameState::SetCurrent(bool _value)
{
	m_IsCurrent = _value;
}

bool CGameState::IsCurrent() const
{
	return m_IsCurrent;
}




//
// Engine events
//
void CGameState::OnUpdate(UpdateEventArgs & e)
{
    if (m_DefaultCameraController)
        m_DefaultCameraController->OnUpdate(e);
}

void CGameState::OnPreRender(RenderEventArgs& e)
{
	m_FrameQuery->Begin(e.FrameCounter);
}

void CGameState::OnRender(RenderEventArgs& e)
{
	e.Camera = GetCameraController()->GetCamera().get();
	
	m_Technique.Render(e);
}

void CGameState::OnPostRender(RenderEventArgs& e)
{
	m_FrameQuery->End(e.FrameCounter);
}

void CGameState::OnRenderUI(RenderEventArgs& e)
{
	vec3 cameraTrans = GetCameraController()->GetCamera()->GetTranslation();
	m_CameraPosText->SetText("Pos: x = " + std::to_string(cameraTrans.x) + ", y = " + std::to_string(cameraTrans.y) + ", z = " + std::to_string(cameraTrans.z));
	m_CameraRotText->SetText("Rot: yaw = " + std::to_string(GetCameraController()->GetCamera()->GetYaw()) + ", pitch = " + std::to_string(GetCameraController()->GetCamera()->GetPitch()));

	IQuery::QueryResult frameResult = m_FrameQuery->GetQueryResult(e.FrameCounter - (m_FrameQuery->GetBufferCount() - 1));
	if (frameResult.IsValid)
	{
		if (_RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
			m_FrameTime = frameResult.ElapsedTime * 1000.0;
		else
			m_FrameTime = frameResult.ElapsedTime / 1000000.0;

		m_FPSText->SetText("FPS: " + std::to_string(1000.0 / m_FrameTime));
	}

	e.Camera = GetCameraController()->GetCamera().get();
	m_Technique.Render(e);
}



//
// Window events
//
void CGameState::OnResize(ResizeEventArgs & e)
{
	m_DefaultCameraController->OnResize(e);

	m_Technique.UpdateViewport(dynamic_cast<const IRenderWindow*>(e.Caller)->GetViewport());
}



//
// Keyboard events
//
void CGameState::OnKeyPressed(KeyEventArgs & e)
{
    bool result = false;
    if (m_Scene)
        result = m_Scene->OnKeyPressed(e);

	if (m_DefaultCameraController && !result)
		m_DefaultCameraController->OnKeyPressed(e);
}

void CGameState::OnKeyReleased(KeyEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnKeyReleased(e);

	if (m_Scene)
		m_Scene->OnKeyReleased(e);
}

void CGameState::OnKeyboardFocus(EventArgs & e)
{
	if (m_Scene)
		m_Scene->OnKeyboardFocus(e);
}

void CGameState::OnKeyboardBlur(EventArgs & e)
{
	if (m_Scene)
		m_Scene->OnKeyboardBlur(e);
}



//
// Mouse events
//
void CGameState::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    bool result = false;
    if (m_Scene)
        result = m_Scene->OnMouseButtonPressed(e);

	if (m_DefaultCameraController && !result)
		m_DefaultCameraController->OnMouseButtonPressed(e);
}

void CGameState::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseButtonReleased(e);

	if (m_Scene)
		m_Scene->OnMouseButtonReleased(e);
}

void CGameState::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseMoved(e);

	if (m_Scene)
		m_Scene->OnMouseMoved(e);
}

void CGameState::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseWheel(e);

	if (m_Scene)
		m_Scene->OnMouseWheel(e);
}

void CGameState::OnMouseLeave(EventArgs & e)
{
	if (m_Scene)
		m_Scene->OnMouseLeave(e);
}

void CGameState::OnMouseFocus(EventArgs & e)
{
	if (m_Scene)
		m_Scene->OnMouseFocus(e);
}

void CGameState::OnMouseBlur(EventArgs & e)
{
	if (m_Scene)
		m_Scene->OnMouseBlur(e);
}



//
// Protected
//
const IApplication* CGameState::GetApplication() const
{
    return m_Application;
}

const std::shared_ptr<IRenderWindow> CGameState::GetRenderWindow() const
{
	return m_RenderWindow;
}

IBaseManager* CGameState::GetBaseManager() const
{
	return m_Application->GetBaseManager();
}

void CGameState::SetCameraController(std::shared_ptr<ICameraController> CameraController)
{
	_ASSERT(CameraController != nullptr);
	m_DefaultCameraController = CameraController;
}

std::shared_ptr<ICameraController> CGameState::GetCameraController() const
{
    return m_DefaultCameraController;
}
