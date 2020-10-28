#include "stdafx.h"

// General
#include "RenderWindowBase.h"

RenderWindowBase::RenderWindowBase(INativeWindow& WindowObject, bool vSync)
	: m_NativeWindow(WindowObject)
    , m_bResizePending(false)
{
	m_Viewport.SetWidth(WindowObject.GetWindowWidth());
	m_Viewport.SetHeight(WindowObject.GetWindowHeight());
}

RenderWindowBase::~RenderWindowBase()
{}




//
// RenderWindowBase
//
void RenderWindowBase::OnUpdate(UpdateEventArgs& e)
{
	UpdateEventArgs updateArgs(e);
	m_RenderWindowEventListener->OnUpdate(updateArgs);

	//GetRenderDevice()->Lock();
	{
		if (m_bResizePending)
		{
			m_RenderTarget->Resize(GetWindowWidth(), GetWindowHeight());
			ResizeSwapChainBuffers(GetWindowWidth(), GetWindowHeight());
			m_bResizePending = false;
		}

		RenderEventArgs renderArgs(e, nullptr);

		m_RenderTarget->Bind();
		{
			OnPreRender(renderArgs);
			OnRender(renderArgs);
			OnPostRender(renderArgs);
			OnRenderUI(renderArgs);
		}
		

		Present();
		//m_RenderTarget->UnBind();
	}
	//GetRenderDevice()->Unlock();
}

void RenderWindowBase::OnPreRender(RenderEventArgs & e)
{
	m_RenderWindowEventListener->OnPreRender(e);
}

void RenderWindowBase::OnRender(RenderEventArgs & e)
{
	m_RenderWindowEventListener->OnRender(e);
}

void RenderWindowBase::OnPostRender(RenderEventArgs & e)
{
	m_RenderWindowEventListener->OnPostRender(e);
}

void RenderWindowBase::OnRenderUI(RenderEventArgs & e)
{
	m_RenderWindowEventListener->OnRenderUI(e);
}



//
// INativeWindow
//
void RenderWindowBase::SetWindowTitle(const std::string& WindowName)
{
	m_NativeWindow.SetWindowTitle(WindowName);
}

std::string RenderWindowBase::GetWindowTitle() const
{
	return m_NativeWindow.GetWindowTitle();
}

size_t RenderWindowBase::GetWindowWidth() const
{
	return m_NativeWindow.GetWindowWidth();
}

size_t RenderWindowBase::GetWindowHeight() const
{
    return m_NativeWindow.GetWindowHeight();
}

void RenderWindowBase::SetCursorPosition(const glm::ivec2 & CursorPosition)
{
	m_NativeWindow.SetCursorPosition(CursorPosition);
}

glm::ivec2 RenderWindowBase::GetCursorPosition() const
{
	return m_NativeWindow.GetCursorPosition();
}

void RenderWindowBase::ShowCursor()
{
	m_NativeWindow.ShowCursor();
}

void RenderWindowBase::HideCursor()
{
	m_NativeWindow.HideCursor();
}

void RenderWindowBase::Close()
{
	m_NativeWindow.Close();
}

void RenderWindowBase::SetEventsListener(INativeWindowEventListener* WindowEventsListener)
{
	_ASSERT_EXPR(false, L"Not implemented!");
}

void RenderWindowBase::ResetEventsListener()
{
	_ASSERT_EXPR(false, L"Not implemented!");
}




//
// IRenderWindow
//
void RenderWindowBase::SetRenderWindowEventListener(std::shared_ptr<IRenderWindowEventListener> RenderWindowEventListener)
{
	_ASSERT(m_RenderWindowEventListener == nullptr);
	m_RenderWindowEventListener = RenderWindowEventListener;
}

void RenderWindowBase::SetNativeWindowEventListener(std::shared_ptr<INativeWindowEventListener> NativeWindowEventListener)
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



//
// INativeWindowEventListener
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
	m_bResizePending = true;

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



//
// Protected
//
void RenderWindowBase::CreateSwapChain()
{
	m_RenderTarget = GetRenderDevice().GetObjectsFactory().CreateRenderTarget();
}
