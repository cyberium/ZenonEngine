#include "stdafx.h"

// Genreal
#include "Application.h"

float g_GameDeltaTime = 0.0f;
float g_ApplicationTime = 0.0f;
int64_t g_FrameCounter = 0L;

Application::Application(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
	, m_bIsInitialized(false)
	, m_bIsRunning(false)
{
	m_HInstance = ::GetModuleHandle(NULL);

	dynamic_cast<IBaseManagerInternal*>(&m_BaseManager)->SetApplicationInternal(this);
}

Application::Application(IBaseManager& BaseManager, HINSTANCE hInstance)
	: m_BaseManager(BaseManager)
	, m_bIsInitialized(false)
	, m_bIsRunning(false)
{
	m_HInstance = hInstance;

	dynamic_cast<IBaseManagerInternal*>(&m_BaseManager)->SetApplicationInternal(this);
}

Application::~Application()
{
	dynamic_cast<IBaseManagerInternal*>(&m_BaseManager)->SetApplicationInternal(nullptr);
}


int Application::Run()
{
	DoBeforeRun();

	int runResult = -1;
	while (m_bIsRunning)
	{
		runResult = DoRun();
	}

	DoAfterRun();

	return runResult;
}

void Application::Stop()
{
	PostQuitMessage(0);
}


IRenderDevice& Application::CreateRenderDevice(RenderDeviceType DeviceType)
{
	m_pRenderDevice = m_BaseManager.GetManager<IznRenderDeviceFactory>()->GetRenderDeviceCreator(DeviceType).CreateRenderDevice();
	return GetRenderDevice();
}

void Application::AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	std::dynamic_pointer_cast<IApplicationEventsConnection>(RenderWindow)->Connect(this);

	m_Windows.push_back(RenderWindow);
}

void Application::DeleleRenderWindow(IRenderWindow* RenderWindow)
{
	m_Windows.clear();
}



//
// IApplication
//
void Application::DoBeforeRun()
{
	if (m_bIsInitialized)
		return;

	m_Initialize(EventArgs(this));

	m_bIsInitialized = true;
	m_bIsRunning = true;
}

int Application::DoRun()
{
	static Timer elapsedTime;

	MSG msg = { 0 };
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			m_Exit(EventArgs(this));

			// Destroy any windows that are still hanging around.
			for (const auto& it : m_Windows)
			{
				it.lock()->Close();
			}

			// Setting this to false will cause the main application's message pump to stop.
			m_bIsRunning = false;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	g_GameDeltaTime = elapsedTime.GetElapsedTime();
	g_ApplicationTime += g_GameDeltaTime;
	g_FrameCounter++;

	//try
	{
		UpdateEventArgs updateArgs(this, g_GameDeltaTime * 166.0f, g_ApplicationTime * 166.0f, g_FrameCounter, nullptr, nullptr);
		m_Update(updateArgs);
	}
	//catch (const std::exception& e)
	//{
	//	Log::Error("Exception: '%s'", e.what());
	//	throw e;
	//}

	return static_cast<int>(msg.wParam);
}

void Application::DoAfterRun()
{
	m_Terminate(EventArgs(this));
	m_Terminated(EventArgs(this));
}

IBaseManager& Application::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice& Application::GetRenderDevice() const
{
	_ASSERT(m_pRenderDevice != nullptr);
	return *m_pRenderDevice;
}



//
// IApplication_WindowsSpecific
//
HINSTANCE Application::GetHInstance() const
{
	return m_HInstance;
}



//
// IApplicationEvents
//
Event& Application::ApplicationInitialize()
{
	return m_Initialize;
}

UpdateEvent& Application::ApplicationUpdate()
{
	return m_Update;
}

Event& Application::ApplicationTerminate()
{
	return m_Terminate;
}

Event& Application::ApplicationTerminated()
{
	return m_Terminated;
}

Event& Application::ApplicationExit()
{
	return m_Exit;
}

UserEvent& Application::ApplicationUserEvent()
{
	return m_UserEvent;
}
