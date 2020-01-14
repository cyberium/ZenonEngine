#include "stdafx.h"

// General
#include "GameStateBase.h"

// Additional
#include "XML/XMLManager.h"

CGameState::CGameState(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents)
	: m_BaseManager(BaseManager)
	, m_RenderWindow(RenderWindow)
	, m_WindowEvents(WindowEvents)
	, m_IsInited(false)
	, m_IsCurrent(false)
{
	m_RenderDevice = m_BaseManager->GetManager<IRenderDevice>();
}

CGameState::~CGameState()
{}



//
// IGameState
//
bool CGameState::Init()
{
	m_VideoSettings = GetBaseManager()->GetManager<ISettings>()->GetGroup("Video");

	m_FrameQuery = GetRenderDevice()->CreateQuery(IQuery::QueryType::Timer, 1);
	m_TestQuery = GetRenderDevice()->CreateQuery(IQuery::QueryType::CountSamples, 1);

	m_Scene3D = GetBaseManager()->GetManager<IScenesFactory>()->CreateScene("SceneBase");
	m_SceneUI = GetBaseManager()->GetManager<IScenesFactory>()->CreateScene("SceneBase");

	{
		m_CameraPosText = GetBaseManager()->GetManager<ISceneNodesFactory>()->CreateSceneNode(m_SceneUI->GetRootNode(), "TextUI");
		std::dynamic_pointer_cast<ISceneNodeUI>(m_CameraPosText)->SetTranslate(vec2(0.0f, 0.0f));

		m_CameraRotText = GetBaseManager()->GetManager<ISceneNodesFactory>()->CreateSceneNode(m_SceneUI->GetRootNode(), "TextUI");
		std::dynamic_pointer_cast<ISceneNodeUI>(m_CameraRotText)->SetTranslate(vec2(0.0f, 20.0f));

		m_CameraRot2Text = GetBaseManager()->GetManager<ISceneNodesFactory>()->CreateSceneNode(m_SceneUI->GetRootNode(), "TextUI");
		std::dynamic_pointer_cast<ISceneNodeUI>(m_CameraRot2Text)->SetTranslate(vec2(0.0f, 40.0f));

		m_FPSText = GetBaseManager()->GetManager<ISceneNodesFactory>()->CreateSceneNode(m_SceneUI->GetRootNode(), "TextUI");
		std::dynamic_pointer_cast<ISceneNodeUI>(m_FPSText)->SetTranslate(vec2(0.0f, 60.0f));
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

	// RenderWindowEvents
    OnUpdateConnection              = renderWindow->Update().connect(&CGameState::OnUpdate, this, std::placeholders::_1);
    OnPreRenderConnection           = renderWindow->PreRender().connect(&CGameState::OnPreRender, this, std::placeholders::_1);
    OnRenderConnection              = renderWindow->Render().connect(&CGameState::OnRender, this, std::placeholders::_1);
    OnPostRenderConnection          = renderWindow->PostRender().connect(&CGameState::OnPostRender, this, std::placeholders::_1);
    OnRenderUIConnection            = renderWindow->RenderUI().connect(&CGameState::OnRenderUI, this, std::placeholders::_1);


	// Window events connections
	OnResizeConnection = m_WindowEvents->Resize().connect(&CGameState::OnResize, this, std::placeholders::_1);

	// Keyboard
	OnKeyPressedConnection = m_WindowEvents->KeyPressed().connect(&CGameState::OnKeyPressed, this, std::placeholders::_1);
	OnKeyReleasedConnection = m_WindowEvents->KeyReleased().connect(&CGameState::OnKeyReleased, this, std::placeholders::_1);

	// Mouse
	OnMouseButtonPressedConnection = m_WindowEvents->MouseButtonPressed().connect(&CGameState::OnMouseButtonPressed, this, std::placeholders::_1);
	OnMouseButtonReleasedConnection = m_WindowEvents->MouseButtonReleased().connect(&CGameState::OnMouseButtonReleased, this, std::placeholders::_1);
	OnMouseMovedConnection = m_WindowEvents->MouseMoved().connect(&CGameState::OnMouseMoved, this, std::placeholders::_1);
	OnMouseWheelConnection = m_WindowEvents->MouseWheel().connect(&CGameState::OnMouseWheel, this, std::placeholders::_1);

    return true;
}

void CGameState::Unset()
{
    std::shared_ptr<IRenderWindowEvents> renderWindow = std::dynamic_pointer_cast<IRenderWindowEvents>(m_RenderWindow);
    _ASSERT(renderWindow);

    renderWindow->Update().disconnect(OnUpdateConnection);
	renderWindow->PreRender().disconnect(OnPreRenderConnection);
	renderWindow->Render().disconnect(OnRenderConnection);
	renderWindow->PostRender().disconnect(OnPostRenderConnection);
	renderWindow->RenderUI().disconnect(OnRenderUIConnection);

	m_WindowEvents->KeyPressed().disconnect(OnKeyPressedConnection);
	m_WindowEvents->KeyReleased().disconnect(OnKeyReleasedConnection);

	m_WindowEvents->MouseButtonPressed().disconnect(OnMouseButtonPressedConnection);
	m_WindowEvents->MouseButtonReleased().disconnect(OnMouseButtonReleasedConnection);
	m_WindowEvents->MouseMoved().disconnect(OnMouseMovedConnection);
	m_WindowEvents->MouseWheel().disconnect(OnMouseWheelConnection);
	
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




void CGameState::OnRayIntersected(const glm::vec3& Point)
{
}

//
// Engine events
//
void CGameState::OnUpdate(UpdateEventArgs& e)
{
	if (m_Scene3D)
		m_Scene3D->OnUpdate(e);

	if (m_SceneUI)
		m_SceneUI->OnUpdate(e);

    if (m_DefaultCameraController)
        m_DefaultCameraController->OnUpdate(e);
}

void CGameState::OnPreRender(RenderEventArgs& e)
{
	m_FrameQuery->Begin(e.FrameCounter);
	//m_TestQuery->Begin(e.FrameCounter);
}

void CGameState::OnRender(RenderEventArgs& e)
{
	e.Camera = GetCameraController()->GetCamera().get();
	
	m_Technique3D.Render(e);
}

void CGameState::OnPostRender(RenderEventArgs& e)
{
	//m_TestQuery->End(e.FrameCounter);
	m_FrameQuery->End(e.FrameCounter);

	vec3 cameraTrans = GetCameraController()->GetCamera()->GetTranslation();
	m_CameraPosText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: x = " + std::to_string(cameraTrans.x) + ", y = " + std::to_string(cameraTrans.y) + ", z = " + std::to_string(cameraTrans.z));
	m_CameraRotText->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: yaw = " + std::to_string(GetCameraController()->GetCamera()->GetYaw()) + ", pitch = " + std::to_string(GetCameraController()->GetCamera()->GetPitch()));
	m_CameraRot2Text->GetProperties()->GetPropertyT<std::string>("Text")->Set("Rot: [" + std::to_string(GetCameraController()->GetCamera()->GetDirection().x) + ", " + std::to_string(GetCameraController()->GetCamera()->GetDirection().y) + ", " + std::to_string(GetCameraController()->GetCamera()->GetDirection().z) + "].");
	
	IQuery::QueryResult frameResult = m_FrameQuery->GetQueryResult(e.FrameCounter);
	if (frameResult.IsValid)
	{
		if (GetRenderDevice()->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
			m_FrameTime = frameResult.ElapsedTime * 1000.0;
		else
			m_FrameTime = frameResult.ElapsedTime / 1000000.0;

		if (m_FrameTime < 1.0f)
			m_FrameTime = 1.0f;
		m_FPSText->GetProperties()->GetPropertyT<std::string>("Text")->Set("FPS: " + std::to_string((1000 / uint64(m_FrameTime))));
	}
}

void CGameState::OnRenderUI(RenderEventArgs& e)
{
	e.Camera = GetCameraController()->GetCamera().get();

	m_TechniqueUI.Render(e);
}



//
// Window events
//
void CGameState::OnResize(ResizeEventArgs & e)
{
	m_DefaultCameraController->OnResize(e);

	m_Technique3D.UpdateViewport(m_RenderWindow->GetViewport());
	m_TechniqueUI.UpdateViewport(m_RenderWindow->GetViewport());
}



//
// Keyboard events
//
void CGameState::OnKeyPressed(KeyEventArgs & e)
{
    bool result = false;
    if (m_SceneUI)
        result = m_SceneUI->OnKeyPressed(e);

	if (m_DefaultCameraController && !result)
		m_DefaultCameraController->OnKeyPressed(e);

	if (e.Key == KeyCode::J)
	{
		CXMLManager xmlM;

		// Writer
		std::shared_ptr<IXMLWriter> writer = xmlM.CreateWriter();

		m_Scene3D->Save(writer);

		std::shared_ptr<IFile> xmlFile = writer->SaveToFile("Scene.xml");
		GetBaseManager()->GetManager<IFilesManager>()->GetFilesStorage("ZenonGamedata")->SaveFile(xmlFile);
	}
}

void CGameState::OnKeyReleased(KeyEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnKeyReleased(e);

	if (m_SceneUI)
		m_SceneUI->OnKeyReleased(e);
}

void CGameState::OnKeyboardFocus(EventArgs & e)
{
	if (m_SceneUI)
		m_SceneUI->OnKeyboardFocus(e);
}

void CGameState::OnKeyboardBlur(EventArgs & e)
{
	if (m_SceneUI)
		m_SceneUI->OnKeyboardBlur(e);
}



//
// Mouse events
//
void CGameState::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    bool result = false;
    if (m_SceneUI)
        result = m_SceneUI->OnMouseButtonPressed(e);

	if (m_DefaultCameraController && !result)
	{
		m_DefaultCameraController->OnMouseButtonPressed(e);

		if (e.LeftButton)
		{
			Ray cameraDownRay = Ray(m_DefaultCameraController->GetCamera()->GetTranslation(), glm::vec3(0.0f, -1.0f, 0.0f));
			Ray resultRay = m_DefaultCameraController->ScreenPointToRay(GetRenderWindow()->GetViewport(), glm::vec2(e.X, e.Y));

			float cosAlpha = (resultRay.GetDirection().x * cameraDownRay.GetDirection().x) + (resultRay.GetDirection().y * cameraDownRay.GetDirection().y) + (resultRay.GetDirection().z * cameraDownRay.GetDirection().z);
			cosAlpha /=
				(
					sqrt
					(
						(resultRay.GetDirection().x * resultRay.GetDirection().x) +
						(resultRay.GetDirection().y * resultRay.GetDirection().y) +
						(resultRay.GetDirection().z * resultRay.GetDirection().z)
					)
					*
					sqrt
					(
						(cameraDownRay.GetDirection().x * cameraDownRay.GetDirection().x) +
						(cameraDownRay.GetDirection().y * cameraDownRay.GetDirection().y) +
						(cameraDownRay.GetDirection().z * cameraDownRay.GetDirection().z)
					)
				);

			float d = m_DefaultCameraController->GetCamera()->GetTranslation().y / cosAlpha;
			if (d < 10000.0f)
			{
				glm::vec3 point = resultRay.GetPointOnRay(d);
				OnRayIntersected(point);
			}
		}
	}
}

void CGameState::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseButtonReleased(e);

	if (m_SceneUI)
		m_SceneUI->OnMouseButtonReleased(e);
}

void CGameState::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseMoved(e);

	if (m_SceneUI)
		m_SceneUI->OnMouseMoved(e);
}

void CGameState::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_DefaultCameraController)
		m_DefaultCameraController->OnMouseWheel(e);

	if (m_SceneUI)
		m_SceneUI->OnMouseWheel(e);
}

void CGameState::OnMouseLeave(EventArgs & e)
{
	if (m_SceneUI)
		m_SceneUI->OnMouseLeave(e);
}

void CGameState::OnMouseFocus(EventArgs & e)
{
	if (m_SceneUI)
		m_SceneUI->OnMouseFocus(e);
}

void CGameState::OnMouseBlur(EventArgs & e)
{
	if (m_SceneUI)
		m_SceneUI->OnMouseBlur(e);
}



//
// Protected
//

std::shared_ptr<IRenderDevice> CGameState::GetRenderDevice() const
{
	return m_RenderDevice;
}

std::shared_ptr<IRenderWindow> CGameState::GetRenderWindow() const
{
	return m_RenderWindow;
}

IBaseManager* CGameState::GetBaseManager() const
{
	return m_BaseManager;
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
