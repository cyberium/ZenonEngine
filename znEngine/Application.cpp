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
	const auto& it = std::find_if(m_Windows.begin(), m_Windows.end(), [&RenderWindow](const std::shared_ptr<IRenderWindow>& Value) -> bool {
		return Value/*.lock()*/ == RenderWindow;
	});
	if (it != m_Windows.end())
	{
		Log::Error("Application: RenderWindow already exists.");
		return;
	}
	m_Windows.push_back(RenderWindow);
}

void Application::CloseRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	const auto& it = std::find_if(m_Windows.begin(), m_Windows.end(), [&RenderWindow](const std::shared_ptr<IRenderWindow>& Value) -> bool {
		return Value/*.lock()*/ == RenderWindow;
	});
	if (it == m_Windows.end())
	{
		Log::Error("Application: Unable find RenderWindow.");
		return;
	}
	m_Windows.erase(it);
}



//
// IApplication
//
void Application::DoBeforeRun()
{
	if (m_bIsInitialized)
		return;

	for (const auto& w : m_Windows)
		//if (auto wLocked = w.lock())
			if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
				applicationEventsListener->OnInitialize(EventArgs());

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
			// Destroy any windows that are still hanging around.
			//for (const auto& it : m_Windows)
			//	it->Close();

			// Setting this to false will cause the main application's message pump to stop.
			m_bIsRunning = false;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	g_GameDeltaTime = elapsedTime.GetElapsedTime();
	g_ApplicationTime += g_GameDeltaTime;
	g_FrameCounter++;

	try
	{
		UpdateEventArgs updateArgs(g_GameDeltaTime, g_ApplicationTime, g_FrameCounter, nullptr, nullptr);
		for (const auto& w : m_Windows)
			//if (auto wLocked = w.lock())
				if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
					applicationEventsListener->OnUpdate(updateArgs);
	}
	catch (const CznRenderException& e)
	{
		Log::Fatal("Exception: '%s'", e.MessageCStr());
	}

	return static_cast<int>(msg.wParam);
}

void Application::DoAfterRun()
{
	for (const auto& w : m_Windows)
		//if (auto wLocked = w.lock())
			if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
				applicationEventsListener->OnExit(EventArgs());
	m_Windows.clear();
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
