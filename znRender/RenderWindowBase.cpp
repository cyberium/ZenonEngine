#include "stdafx.h"

// General
#include "RenderWindowBase.h"

RenderWindowBase::RenderWindowBase(std::unique_ptr<IznNativeWindow> WindowObject, bool vSync)
	: m_NativeWindow(std::move(WindowObject))
    , m_ResizePending(false)
{
	m_Viewport.SetWidth(m_NativeWindow->GetWindowWidth());
	m_Viewport.SetHeight(m_NativeWindow->GetWindowHeight());
}

RenderWindowBase::~RenderWindowBase()
{
	Log::Info("~RenderWindowBase");
}



//
// IznNativeWindow
//
void RenderWindowBase::SetWindowTitle(const std::string& WindowName)
{
	m_NativeWindow->SetWindowTitle(WindowName);
}

std::string RenderWindowBase::GetWindowTitle() const
{
	return m_NativeWindow->GetWindowTitle();
}

size_t RenderWindowBase::GetWindowWidth() const
{
	return m_NativeWindow->GetWindowWidth();
}

size_t RenderWindowBase::GetWindowHeight() const
{
    return m_NativeWindow->GetWindowHeight();
}

void RenderWindowBase::SetCursorPosition(const glm::ivec2 & CursorPosition)
{
	m_NativeWindow->SetCursorPosition(CursorPosition);
}

glm::ivec2 RenderWindowBase::GetCursorPosition() const
{
	return m_NativeWindow->GetCursorPosition();
}

void RenderWindowBase::ShowCursor()
{
	m_NativeWindow->ShowCursor();
}

void RenderWindowBase::HideCursor()
{
	m_NativeWindow->HideCursor();
}

void RenderWindowBase::Close()
{
	m_NativeWindow->Close();
}

void RenderWindowBase::SetEventsListener(IznNativeWindowEventListener* WindowEventsListener)
{
	throw CException("Not implemented!");
}

void RenderWindowBase::ResetEventsListener()
{
	throw CException("Not implemented!");
}




//
// IRenderWindow
//
void RenderWindowBase::SetRenderWindowEventListener(std::shared_ptr<IRenderWindowEventListener> RenderWindowEventListener)
{
	_ASSERT(m_RenderWindowEventListener == nullptr);
	m_RenderWindowEventListener = RenderWindowEventListener;
}

void RenderWindowBase::SetNativeWindowEventListener(std::shared_ptr<IznNativeWindowEventListener> NativeWindowEventListener)
{
	_ASSERT(m_NativeWindowEventListener == nullptr);
	m_NativeWindowEventListener = NativeWindowEventListener;
}

const std::shared_ptr<IRenderTarget>& RenderWindowBase::GetRenderTarget() const
{
    return m_RenderTarget;
}

const Viewport& RenderWindowBase::GetViewport() const
{
	return m_Viewport;
}

float RenderWindowBase::GetUpdateDeltaTime() const
{
	return m_UpdateDeltaTime;
}

float RenderWindowBase::GetRenderDeltaTime() const
{
	return m_RenderDeltaTime;
}

float RenderWindowBase::GetSummaDeltaTime() const
{
	return m_SummaDeltaTime;
}



//
// IznNativeWindowEventListener
//
void RenderWindowBase::OnWindowInputFocus(EventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowInputFocus(Args);
}

void RenderWindowBase::OnWindowInputBlur(EventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowInputBlur(Args);
}

void RenderWindowBase::OnWindowMinimize(EventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowMinimize(Args);
}

void RenderWindowBase::OnWindowRestore(EventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowRestore(Args);
}

void RenderWindowBase::OnWindowResize(ResizeEventArgs& Args) // The RenderWindowBase window has be resized
{
	m_Viewport.SetWidth(Args.Width);
	m_Viewport.SetHeight(Args.Height);
	m_ResizePending = true;

	m_NativeWindowEventListener->OnWindowResize(Args);
}

void RenderWindowBase::OnWindowClose(WindowCloseEventArgs& Args)
{
	m_NativeWindowEventListener->OnWindowClose(Args);
}

bool RenderWindowBase::OnWindowKeyPressed(KeyEventArgs & Args)
{
	return m_NativeWindowEventListener->OnWindowKeyPressed(Args);
}

void RenderWindowBase::OnWindowKeyReleased(KeyEventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowKeyReleased(Args);
}

void RenderWindowBase::OnWindowKeyboardFocus(EventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowKeyboardFocus(Args);
}

void RenderWindowBase::OnWindowKeyboardBlur(EventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowKeyboardBlur(Args);
}

void RenderWindowBase::OnWindowMouseMoved(MouseMotionEventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowMouseMoved(Args);
}

bool RenderWindowBase::OnWindowMouseButtonPressed(MouseButtonEventArgs & Args)
{
	return m_NativeWindowEventListener->OnWindowMouseButtonPressed(Args);
}

void RenderWindowBase::OnWindowMouseButtonReleased(MouseButtonEventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowMouseButtonReleased(Args);
}

bool RenderWindowBase::OnWindowMouseWheel(MouseWheelEventArgs & Args)
{
	return m_NativeWindowEventListener->OnWindowMouseWheel(Args);
}

void RenderWindowBase::OnWindowMouseLeave(EventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowMouseLeave(Args);
}

void RenderWindowBase::OnWindowMouseFocus(EventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowMouseFocus(Args);
}

void RenderWindowBase::OnWindowMouseBlur(EventArgs & Args)
{
	m_NativeWindowEventListener->OnWindowMouseBlur(Args);
}



//
// IApplicationEventsListener
//
void RenderWindowBase::OnInitialize(EventArgs & Args)
{
}

void RenderWindowBase::OnUpdate(UpdateEventArgs& Args)
{
	HighResolutionTimer timer;

	UpdateEventArgs updateArgs(Args);
	RaiseUpdate(updateArgs);

	//GetRenderDevice()->Lock();
	{
		if (m_ResizePending)
		{
			m_RenderTarget->Resize(GetWindowWidth(), GetWindowHeight());
			ResizeSwapChainBuffers(GetWindowWidth(), GetWindowHeight());
			m_ResizePending = false;
		}

		RenderEventArgs renderArgs(Args, nullptr);

		m_RenderTarget->Bind();
		{
			RaiseRender(renderArgs);
			Present();
		}
		//m_RenderTarget->UnBind();
	}
	//GetRenderDevice()->Unlock();

	m_SummaDeltaTime = timer.GetElapsedMilliSeconds();
}

void RenderWindowBase::OnExit(EventArgs & Args)
{
	Close();
	m_RenderWindowEventListener.reset();
	m_NativeWindowEventListener.reset();
}

void RenderWindowBase::OnUserEvent(UserEventArgs & Args)
{
}



//
// Protected
//
void RenderWindowBase::RaiseUpdate(UpdateEventArgs& e)
{
	HighResolutionTimer timer;
	m_RenderWindowEventListener->OnUpdate(e);
	m_UpdateDeltaTime = timer.GetElapsedMilliSeconds();
}

void RenderWindowBase::RaiseRender(RenderEventArgs & e)
{
	HighResolutionTimer timer;
	m_RenderWindowEventListener->OnRender(e);
	m_RenderDeltaTime = timer.GetElapsedMilliSeconds();
}

IznNativeWindow & RenderWindowBase::GetNativeWindow()
{
	return *m_NativeWindow.get();
}

void RenderWindowBase::CreateSwapChain()
{
	m_RenderTarget = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
}
