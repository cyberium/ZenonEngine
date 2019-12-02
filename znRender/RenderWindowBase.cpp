#include "stdafx.h"

// General
#include "RenderWindowBase.h"

// Additional
#include "Camera.h"

RenderWindowBase::RenderWindowBase(std::shared_ptr<IRenderDevice> RenderDevice, IWindowObject * WindowObject, bool vSync)
	: m_Device(RenderDevice)
    , m_WindowObject(WindowObject)
	, m_vSync(vSync)

    , m_bResizePending(false)

	, m_PreviousMousePosition(0, 0)
	, m_InClientRect(false)
	, m_IsMouseTracking(false)

	, m_bHasKeyboardFocus(false)
{

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
    return m_Device.lock();
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
// Engine events
//
Event & RenderWindowBase::Initialize()
{
	return m_Initialize;
}
void RenderWindowBase::OnInitialize(EventArgs& e)
{
	m_Initialize(e);
}

UpdateEvent & RenderWindowBase::Update()
{
	return m_Update;
}
void RenderWindowBase::OnUpdate(UpdateEventArgs& e)
{
	m_Update(e);
}

RenderEvent & RenderWindowBase::PreRender()
{
	return m_PreRender;
}
void RenderWindowBase::OnPreRender(RenderEventArgs& e)
{
    if (m_bResizePending)
    {
        ResizeSwapChainBuffers(GetWindowWidth(), GetWindowHeight());
        m_bResizePending = false;
    }

    m_RenderTarget->Bind();

	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	m_PreRender(renderArgs);
}

RenderEvent & RenderWindowBase::Render()
{
	return m_Render;
}
void RenderWindowBase::OnRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	m_Render(renderArgs);
}

RenderEvent & RenderWindowBase::PostRender()
{
	return m_PostRender;
}
void RenderWindowBase::OnPostRender(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	m_PostRender(renderArgs);

    //m_RenderTarget->UnBind();
}

RenderEvent & RenderWindowBase::RenderUI()
{
	return m_RenderUI;
}
void RenderWindowBase::OnRenderUI(RenderEventArgs& e)
{
	RenderEventArgs renderArgs(*this, e.ElapsedTime, e.TotalTime, e.FrameCounter, e.Camera, e.PipelineState, e.Node);
	m_RenderUI(renderArgs);
}

Event & RenderWindowBase::Terminate()
{
	return m_Terminate;
}
void RenderWindowBase::OnTerminate(EventArgs& e)
{
	m_Terminate(e);
}


//
// Window events
//
Event & RenderWindowBase::InputFocus()
{
	return m_InputFocus;
}
void RenderWindowBase::OnInputFocus(EventArgs& e) // The window has received focus
{
	m_InputFocus(e);
}

Event & RenderWindowBase::InputBlur()
{
	return m_InputBlur;
}
void RenderWindowBase::OnInputBlur(EventArgs& e) // The RenderWindowBase window has lost focus
{
	m_InputBlur(e);
}

Event & RenderWindowBase::Minimize()
{
	return m_Minimize;
}
void RenderWindowBase::OnMinimize(EventArgs& e) // The RenderWindowBase window has been minimized
{
	m_Minimize(e);
}

Event & RenderWindowBase::Restore()
{
	return m_Restore;
}
void RenderWindowBase::OnRestore(EventArgs& e) // The RenderWindowBase window has been restored
{
	m_Restore(e);
}

ResizeEvent & RenderWindowBase::Resize()
{
	return m_Resize;
}
void RenderWindowBase::OnResize(ResizeEventArgs& e) // The RenderWindowBase window has be resized
{
	m_Viewport.SetWidth(e.Width);
	m_Viewport.SetHeight(e.Height);

	m_Resize(e);

	//HideWindow();
	//ShowWindow();

    m_bResizePending = true;
}

Event & RenderWindowBase::Expose()
{
	return m_Expose;
}
void RenderWindowBase::OnExpose(EventArgs& e) // The window contents should be repainted
{
	m_Expose(e);
}

WindowCloseEvent & RenderWindowBase::Close()
{
	return m_Close;
}
void RenderWindowBase::OnClose(WindowCloseEventArgs& e)
{
	m_Close(e);
}



//
// Keyboard events
//
KeyboardEvent & RenderWindowBase::KeyPressed()
{
	return m_KeyPressed;
}
void RenderWindowBase::OnKeyPressed(KeyEventArgs& e) // A keyboard key was pressed
{
	m_KeyPressed(e);
}

KeyboardEvent & RenderWindowBase::KeyReleased()
{
	return m_KeyReleased;
}
void RenderWindowBase::OnKeyReleased(KeyEventArgs& e) // A keyboard key was released
{
	m_KeyReleased(e);
}

Event & RenderWindowBase::KeyboardFocus()
{
	return m_KeyboardFocus;
}
void RenderWindowBase::OnKeyboardFocus(EventArgs& e) // Window gained keyboard focus
{
	m_bHasKeyboardFocus = true;
	m_KeyboardFocus(e);
}

Event & RenderWindowBase::KeyboardBlur()
{
	return m_KeyboardBlur;
}
void RenderWindowBase::OnKeyboardBlur(EventArgs& e) // Window lost keyboard focus
{
	m_bHasKeyboardFocus = false;
	m_KeyboardBlur(e);
}


//
// The mouse events
//

MouseMotionEvent & RenderWindowBase::MouseMoved()
{
	return m_MouseMoved;
}
void RenderWindowBase::OnMouseMoved(MouseMotionEventArgs& e)
{
	if (!m_IsMouseTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = m_WindowObject->GetHWnd();
		if (::TrackMouseEvent(&tme))
		{
			m_IsMouseTracking = true;
		}
	}

	if (!m_InClientRect)
	{
		m_PreviousMousePosition = glm::ivec2(e.X, e.Y);
		m_InClientRect = true;
	}

	e.RelX = e.X - m_PreviousMousePosition.x;
	e.RelY = e.Y - m_PreviousMousePosition.y;

	m_PreviousMousePosition = glm::ivec2(e.X, e.Y);

	m_MouseMoved(e);
}

MouseButtonEvent & RenderWindowBase::MouseButtonPressed()
{
	return m_MouseButtonPressed;
}
void RenderWindowBase::OnMouseButtonPressed(MouseButtonEventArgs& e)
{
	m_MouseButtonPressed(e);
}

MouseButtonEvent & RenderWindowBase::MouseButtonReleased()
{
	return m_MouseButtonReleased;
}
void RenderWindowBase::OnMouseButtonReleased(MouseButtonEventArgs& e)
{
	m_MouseButtonReleased(e);
}

MouseWheelEvent & RenderWindowBase::MouseWheel()
{
	return m_MouseWheel;
}
void RenderWindowBase::OnMouseWheel(MouseWheelEventArgs& e)
{
	m_MouseWheel(e);
}

Event & RenderWindowBase::MouseLeave()
{
	return m_MouseLeave;
}
void RenderWindowBase::OnMouseLeave(EventArgs& e)
{
	m_IsMouseTracking = false;
	m_InClientRect = false;

	m_MouseLeave(e);
}

Event & RenderWindowBase::MouseFocus()
{
	return m_MouseFocus;
}
void RenderWindowBase::OnMouseFocus(EventArgs& e) // The window has received mouse focus
{
	m_MouseFocus(e);
}

Event & RenderWindowBase::MouseBlur()
{
	return m_MouseBlur;
}
void RenderWindowBase::OnMouseBlur(EventArgs& e) // The window has lost mouse focus
{
	m_MouseBlur(e);
}



void RenderWindowBase::CreateSwapChain()
{
	m_RenderTarget = GetRenderDevice()->CreateRenderTarget();
}


