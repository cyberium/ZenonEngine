#include "stdafx.h"

// General
#include "RenderWindowBase.h"

RenderWindowBase::RenderWindowBase(std::shared_ptr<IRenderDevice> RenderDevice, IWindowObject * WindowObject, bool vSync)
	: m_RenderDevice(RenderDevice)
    , m_WindowObject(WindowObject)
	, m_vSync(vSync)

    , m_bResizePending(false)

	, m_PreviousMousePosition(0, 0)
	, m_InClientRect(false)
	, m_IsMouseTracking(false)

	, m_bHasKeyboardFocus(false)
{
	m_Viewport.SetWidth(WindowObject->GetWindowWidth());
	m_Viewport.SetHeight(WindowObject->GetWindowHeight());
}

RenderWindowBase::~RenderWindowBase()
{}


void RenderWindowBase::ShowWindow()
{
	m_WindowObject->ShowWindow(SW_SHOWDEFAULT);
	m_WindowObject->BringWindowToTop();
}

void RenderWindowBase::HideWindow()
{
	m_WindowObject->ShowWindow(SW_HIDE);
}

void RenderWindowBase::CloseWindow()
{
	m_WindowObject->DestroyWindow();

	PostQuitMessage(0);
}

void RenderWindowBase::SetMousePosition(vec2 _position)
{
	RECT rc;
	m_WindowObject->GetClientRect(&rc);

	m_WindowObject->ClientToScreen(reinterpret_cast<POINT*>(&rc.left));
	m_WindowObject->ClientToScreen(reinterpret_cast<POINT*>(&rc.right));

	::SetCursorPos(rc.left + _position.x, rc.top + _position.y);
}


int RenderWindowBase::GetWindowWidth() const
{
    RECT clientRect;
    m_WindowObject->GetClientRect(&clientRect);

	return clientRect.right - clientRect.left;
}

int RenderWindowBase::GetWindowHeight() const
{
    RECT clientRect;
    m_WindowObject->GetClientRect(&clientRect);

    return clientRect.bottom - clientRect.top;
}

glm::ivec2 RenderWindowBase::GetWindowSize() const
{
	return glm::ivec2(GetWindowWidth(), GetWindowHeight());
}

bool RenderWindowBase::IsVSync() const
{
	return m_vSync;
}

HWND RenderWindowBase::GetHWnd() const
{
	return m_WindowObject->GetHWnd();
}

std::shared_ptr<IRenderDevice> RenderWindowBase::GetRenderDevice() const
{
    return m_RenderDevice.lock();
}

std::shared_ptr<IRenderTarget> RenderWindowBase::GetRenderTarget() const
{
    return m_RenderTarget;
}

const Viewport * RenderWindowBase::GetViewport() const
{
	return &m_Viewport;
}




//
// IApplicationEventsConnection
//
void RenderWindowBase::Connect(IApplicationEvents * ApplicationEvents)
{
	m_InitializeConnection = ApplicationEvents->Initialize().connect(&RenderWindowBase::OnInitialize, this, std::placeholders::_1);
	m_UpdateConnection     = ApplicationEvents->Update().connect(&RenderWindowBase::OnUpdate, this, std::placeholders::_1);
	m_TerminateConnection  = ApplicationEvents->Terminate().connect(&RenderWindowBase::OnTerminate, this, std::placeholders::_1);
	//m_TerminatedConnection = ApplicationEvents->Terminated().connect(&OnTerminated, this, std::placeholders::_1);
	//m_ExitConnection       = ApplicationEvents->Exit().connect(&OnExit, this, std::placeholders::_1);
	//m_UserEventConnection  = ApplicationEvents->UserEvent().connect(&OnUserEvent, this, std::placeholders::_1);
}

void RenderWindowBase::Disconnect(IApplicationEvents * ApplicationEvents)
{
	ApplicationEvents->Initialize().disconnect(m_InitializeConnection);
	ApplicationEvents->Update().disconnect(m_UpdateConnection);
	ApplicationEvents->Terminate().disconnect(m_TerminateConnection);
	//ApplicationEvents->Terminated().disconnect(m_TerminatedConnection);
	//ApplicationEvents->Exit().disconnect(m_ExitConnection);
	//ApplicationEvents->UserEvent().disconnect(m_UserEventConnection);
}



void RenderWindowBase::CreateSwapChain()
{
	m_RenderTarget = GetRenderDevice()->CreateRenderTarget();
}


