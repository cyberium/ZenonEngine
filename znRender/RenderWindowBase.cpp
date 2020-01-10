#include "stdafx.h"

// General
#include "RenderWindowBase.h"

RenderWindowBase::RenderWindowBase(std::shared_ptr<IRenderDevice> RenderDevice, IWindowObject * WindowObject, bool vSync)
	: m_RenderDevice(RenderDevice)
    , m_WindowObject(WindowObject)
	, m_vSync(vSync)

    , m_bResizePending(false)
{
	m_Viewport.SetWidth(WindowObject->GetWindowWidth());
	m_Viewport.SetHeight(WindowObject->GetWindowHeight());

	m_ResizeConnection = dynamic_cast<IWindowEvents*>(WindowObject)->Resize().connect(&RenderWindowBase::OnResize, this, std::placeholders::_1);
	m_WindowCloseConnection = dynamic_cast<IWindowEvents*>(WindowObject)->Close().connect(&RenderWindowBase::OnClose, this, std::placeholders::_1);
}

RenderWindowBase::~RenderWindowBase()
{}


int RenderWindowBase::GetWindowWidth() const
{
	return m_WindowObject->GetWindowWidth();
}

int RenderWindowBase::GetWindowHeight() const
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

std::shared_ptr<IRenderDevice> RenderWindowBase::GetRenderDevice() const
{
    return m_RenderDevice.lock();
}

std::shared_ptr<IRenderTarget> RenderWindowBase::GetRenderTarget() const
{
    return m_RenderTarget;
}

IWindowObject* RenderWindowBase::GetWindowObject() const
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

void RenderWindowBase::OnResize(ResizeEventArgs& e) // The RenderWindowBase window has be resized
{
	m_Viewport.SetWidth(e.Width);
	m_Viewport.SetHeight(e.Height);

	m_bResizePending = true;
}

void RenderWindowBase::OnClose(WindowCloseEventArgs& e)
{
}



//
// IApplicationEventsConnection
//
void RenderWindowBase::Connect(IApplicationEvents * ApplicationEvents)
{
	m_UpdateConnection = ApplicationEvents->Update().connect(&RenderWindowBase::OnUpdate, this, std::placeholders::_1);
}

void RenderWindowBase::Disconnect(IApplicationEvents * ApplicationEvents)
{
	ApplicationEvents->Update().disconnect(m_UpdateConnection);
}



void RenderWindowBase::CreateSwapChain()
{
	m_RenderTarget = GetRenderDevice()->CreateRenderTarget();
}
