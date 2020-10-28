#include "stdafx.h"

// Genreal
#include "Application.h"

Application::Application(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
	, m_IsInitialized(false)
	, m_IsRunning(false)
	, m_GameDeltaTime(0.0f)
    , m_GameTime(0.0f)
    , m_FrameCounter(0L)
{
	m_HInstance = ::GetModuleHandle(NULL);

	dynamic_cast<IBaseManagerInternal*>(&m_BaseManager)->SetApplicationInternal(this);
}

Application::Application(IBaseManager& BaseManager, HINSTANCE hInstance)
	: m_BaseManager(BaseManager)
	, m_IsInitialized(false)
	, m_IsRunning(false)
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
	while (m_IsRunning)
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
	m_RenderDevice = m_BaseManager.GetManager<IznRenderDeviceFactory>()->GetRenderDeviceCreator(DeviceType).CreateRenderDevice();
	return GetRenderDevice();
}

void Application::AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	const auto& it = std::find(m_Windows.begin(), m_Windows.end(), RenderWindow);
	if (it != m_Windows.end())
	{
		Log::Error("Application: RenderWindow already exists.");
		return;
	}
	m_Windows.push_back(RenderWindow);
}

void Application::CloseRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	const auto& it = std::find(m_Windows.begin(), m_Windows.end(), RenderWindow);
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
	if (m_IsInitialized)
		return;

	for (const auto& w : m_Windows)
		if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
			applicationEventsListener->OnInitialize(EventArgs());

	m_IsInitialized = true;
	m_IsRunning = true;
}

int Application::DoRun()
{
	MSG msg = { 0 };
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			m_IsRunning = false;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	m_GameDeltaTime = m_GameTimer.GetElapsedTime();
	m_GameTime += m_GameDeltaTime;
	m_FrameCounter++;

	UpdateEventArgs updateArgs(m_GameDeltaTime, m_GameTime, m_FrameCounter, nullptr, nullptr);
	for (const auto& w : m_Windows)
		if (auto applicationEventsListener = std::dynamic_pointer_cast<IApplicationEventsListener>(w))
			applicationEventsListener->OnUpdate(updateArgs);

	return static_cast<int>(msg.wParam);
}

void Application::DoAfterRun()
{
	for (const auto& w : m_Windows)
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
	_ASSERT(m_RenderDevice != nullptr);
	return *m_RenderDevice;
}



//
// IApplication_WindowsSpecific
//
HINSTANCE Application::GetHInstance() const
{
	return m_HInstance;
}
