#include "stdafx.h"

// General
#include "RenderWindowBase.h"

RenderWindowBase::RenderWindowBase(IRenderDevice* RenderDevice, INativeWindow * WindowObject, bool vSync)
	: m_RenderDevice(RenderDevice)
    , m_WindowObject(WindowObject)
	, m_vSync(vSync)

    , m_bResizePending(false)
{
	m_Viewport.SetWidth(WindowObject->GetWindowWidth());
	m_Viewport.SetHeight(WindowObject->GetWindowHeight());
}

RenderWindowBase::~RenderWindowBase()
{}




//
// RenderWindowBase
//
void RenderWindowBase::OnUpdate(UpdateEventArgs& e)
{
	UpdateEventArgs updateArgs(this, e.ElapsedTime, e.TotalTime, e.FrameCounter);
	m_Update(updateArgs);

	//GetRenderDevice()->Lock();
	{
		if (m_bResizePending)
		{
			ResizeSwapChainBuffers(GetWindowWidth(), GetWindowHeight());
			m_bResizePending = false;
		}

		RenderEventArgs renderArgs(this, e.ElapsedTime, e.TotalTime, e.FrameCounter, nullptr, nullptr, nullptr);

		m_RenderTarget->Bind();
		{
			OnPreRender(renderArgs);
			OnRender(renderArgs);
			OnPostRender(renderArgs);
			OnRenderUI(renderArgs);
		}
		//m_RenderTarget->UnBind(); ???

		Present();
	}
	//GetRenderDevice()->Unlock();
}

void RenderWindowBase::OnPreRender(RenderEventArgs & e)
{
	m_PreRender(e);
}

void RenderWindowBase::OnRender(RenderEventArgs & e)
{
	m_Render(e);
}

void RenderWindowBase::OnPostRender(RenderEventArgs & e)
{
	m_PostRender(e);
}

void RenderWindowBase::OnRenderUI(RenderEventArgs & e)
{
	m_RenderUI(e);
}



//
// IRenderWindow
//
size_t RenderWindowBase::GetWindowWidth() const
{
	return m_WindowObject->GetWindowWidth();
}

size_t RenderWindowBase::GetWindowHeight() const
{
    return m_WindowObject->GetWindowHeight();
}

glm::ivec2 RenderWindowBase::GetWindowSize() const
{
	return glm::ivec2(GetWindowWidth(), GetWindowHeight());
}

bool RenderWindowBase::IsVSync() const
{
	return m_vSync;
}

IRenderDevice* RenderWindowBase::GetRenderDevice() const
{
    return m_RenderDevice;
}

IRenderTarget* RenderWindowBase::GetRenderTarget() const
{
    return m_RenderTarget;
}

INativeWindow* RenderWindowBase::GetWindowObject() const
{
	return m_WindowObject;
}

const Viewport* RenderWindowBase::GetViewport() const
{
	return &m_Viewport;
}



//
// IRenderWindowEvents
//
UpdateEvent& RenderWindowBase::Update()
{
	return m_Update;
}

RenderEvent& RenderWindowBase::PreRender()
{
	return m_PreRender;
}

RenderEvent& RenderWindowBase::Render()
{
	return m_Render;
}

RenderEvent& RenderWindowBase::PostRender()
{
	return m_PostRender;
}

RenderEvent& RenderWindowBase::RenderUI()
{
	return m_RenderUI;
}

Event& RenderWindowBase::WindowInputFocus()
{
	return m_InputFocus;
}

Event& RenderWindowBase::WindowInputBlur()
{
	return m_InputBlur;
}

Event& RenderWindowBase::WindowMinimize()
{
	return m_Minimize;
}

Event& RenderWindowBase::WindowRestore()
{
	return m_Restore;
}

ResizeEvent& RenderWindowBase::WindowResize()
{
	return m_Resize;
}

WindowCloseEvent& RenderWindowBase::WindowClose()
{
	return m_Close;
}

KeyboardEvent& RenderWindowBase::WindowKeyPressed()
{
	return m_KeyPressed;
}

KeyboardEvent& RenderWindowBase::WindowKeyReleased()
{
	return m_KeyReleased;
}

Event& RenderWindowBase::WindowKeyboardFocus()
{
	return m_KeyboardFocus;
}

Event& RenderWindowBase::WindowKeyboardBlur()
{
	return m_KeyboardBlur;
}

MouseMotionEvent& RenderWindowBase::WindowMouseMoved()
{
	return m_MouseMoved;
}

MouseButtonEvent& RenderWindowBase::WindowMouseButtonPressed()
{
	return m_MouseButtonPressed;
}

MouseButtonEvent& RenderWindowBase::WindowMouseButtonReleased()
{
	return m_MouseButtonReleased;
}

MouseWheelEvent& RenderWindowBase::WindowMouseWheel()
{
	return m_MouseWheel;
}

Event& RenderWindowBase::WindowMouseLeave()
{
	return m_MouseLeave;
}

Event& RenderWindowBase::WindowMouseFocus()
{
	return m_MouseFocus;
}

Event& RenderWindowBase::WindowMouseBlur()
{
	return m_MouseBlur;
}



//
// INativeWindowEventListener
//
void RenderWindowBase::OnWindowInputFocus(EventArgs & Args)
{
	m_InputFocus(Args);
}

void RenderWindowBase::OnWindowInputBlur(EventArgs & Args)
{
	m_InputBlur(Args);
}

void RenderWindowBase::OnWindowMinimize(EventArgs & Args)
{
	m_Minimize(Args);
}

void RenderWindowBase::OnWindowRestore(EventArgs & Args)
{
	m_Restore(Args);
}

void RenderWindowBase::OnWindowResize(ResizeEventArgs& Args) // The RenderWindowBase window has be resized
{
	m_Viewport.SetWidth(Args.Width);
	m_Viewport.SetHeight(Args.Height);
	m_bResizePending = true;

	m_Resize(Args);
}

void RenderWindowBase::OnWindowClose(WindowCloseEventArgs& Args)
{
	m_Close(Args);
}

bool RenderWindowBase::OnWindowKeyPressed(KeyEventArgs & Args)
{
	m_KeyPressed(Args);
	return true;
}

void RenderWindowBase::OnWindowKeyReleased(KeyEventArgs & Args)
{
	m_KeyReleased(Args);
}

void RenderWindowBase::OnWindowKeyboardFocus(EventArgs & Args)
{
	m_KeyboardFocus(Args);
}

void RenderWindowBase::OnWindowKeyboardBlur(EventArgs & Args)
{
	m_KeyboardBlur(Args);
}

void RenderWindowBase::OnWindowMouseMoved(MouseMotionEventArgs & Args)
{
	m_MouseMoved(Args);
}

bool RenderWindowBase::OnWindowMouseButtonPressed(MouseButtonEventArgs & Args)
{
	m_MouseButtonPressed(Args);
	return true;
}

void RenderWindowBase::OnWindowMouseButtonReleased(MouseButtonEventArgs & Args)
{
	m_MouseButtonReleased(Args);
}

bool RenderWindowBase::OnWindowMouseWheel(MouseWheelEventArgs & Args)
{
	m_MouseWheel(Args);
	return true;
}

void RenderWindowBase::OnWindowMouseLeave(EventArgs & Args)
{
	m_MouseLeave(Args);
}

void RenderWindowBase::OnWindowMouseFocus(EventArgs & Args)
{
	m_MouseFocus(Args);
}

void RenderWindowBase::OnWindowMouseBlur(EventArgs & Args)
{
	m_MouseBlur(Args);
}







//
// IApplicationEventsConnection
//
void RenderWindowBase::Connect(IApplicationEvents * ApplicationEvents)
{
	m_UpdateConnection = ApplicationEvents->ApplicationUpdate().connect(&RenderWindowBase::OnUpdate, this, std::placeholders::_1);
}

void RenderWindowBase::Disconnect(IApplicationEvents * ApplicationEvents)
{
	ApplicationEvents->ApplicationUpdate().disconnect(m_UpdateConnection);
}



void RenderWindowBase::CreateSwapChain()
{
	m_RenderTarget = GetRenderDevice()->CreateRenderTarget();
}
