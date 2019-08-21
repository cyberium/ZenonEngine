#include "stdafx.h"

// Genreal
#include "Application.h"

// Additional
#include "EngineTime.h"

float g_GameDeltaTime = 0.0f;
float g_ApplicationTime = 0.0f;
int64_t g_FrameCounter = 0L;


std::shared_ptr<RenderWindow> gs_WindowHandle = nullptr;
IApplication * _ApplicationInstance = nullptr;
std::shared_ptr<CBaseManager> _BaseManager = nullptr;

Application::Application(std::shared_ptr<CBaseManager> BaseManager)
	: m_bIsInitialized(false)
	, m_bIsRunning(false)
{
    _BaseManager = BaseManager;
	m_HINSTANCE = ::GetModuleHandle(NULL);

	_ApplicationInstance = this;
}

Application::Application(std::shared_ptr<CBaseManager> BaseManager, HINSTANCE hInstance)
	: m_bIsInitialized(false)
	, m_bIsRunning(false)
{
    _BaseManager = BaseManager;
	m_HINSTANCE = hInstance;

	_ApplicationInstance = this;
}

Application::~Application()
{
	_ApplicationInstance = nullptr;
}

IApplication& Application::Get()
{
	assert(_ApplicationInstance != nullptr);
	return *_ApplicationInstance;
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


std::shared_ptr<IRenderDevice> Application::CreateRenderDevice(IRenderDevice::DeviceType DeviceType)
{
    switch (DeviceType)
    {
        case IRenderDevice::DeviceType::None:
            fail1();
            break;
        case IRenderDevice::DeviceType::DirectX:
            SetRenderDevice(CreateRenderDeviceDX11(_BaseManager));
            break;
        case IRenderDevice::DeviceType::OpenGL:
            SetRenderDevice(CreateRenderDeviceOGL(_BaseManager));
            break;
        default:
            break;
    }

	return GetRenderDevice();
}

std::shared_ptr<RenderWindow> Application::CreateRenderWindow(IWindowObject * WindowObject, bool vSync)
{
    switch (GetRenderDevice()->GetDeviceType())
    {
        case IRenderDevice::DeviceType::None:
            fail1();
            break;
        case IRenderDevice::DeviceType::DirectX:
            SetRenderWindow(CreateRenderWindowDX11(GetRenderDevice(), WindowObject, vSync));
            break;
        case IRenderDevice::DeviceType::OpenGL:
            SetRenderWindow(CreateRenderWindowOGL(GetRenderDevice(), WindowObject, vSync));
            break;
        default:
            break;
    }

	return GetRenderWindow();
}



//
// IApplication
//

void Application::DoBeforeRun()
{
	OnInitialize(EventArgs(*this));

	m_bIsRunning = true;
}

int Application::DoRun()
{
	static Timer elapsedTime;

	MSG msg;
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			EventArgs eventArgs(*this);
			OnExit(eventArgs);
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	{
		g_GameDeltaTime = elapsedTime.GetElapsedTime();
		g_ApplicationTime += g_GameDeltaTime;
		g_FrameCounter++;

		UpdateEventArgs updateArgs(*this, g_GameDeltaTime, g_ApplicationTime, g_FrameCounter);
		OnUpdate(updateArgs);

		m_pRenderDevice->Lock();
		{
			RenderEventArgs renderArgs(*this, g_GameDeltaTime * 166.0f, g_ApplicationTime * 166.0f, g_FrameCounter, nullptr, nullptr, nullptr);
			OnRender(renderArgs);

			RenderEventArgs renderUIArgs(*this, g_GameDeltaTime, g_ApplicationTime, g_FrameCounter, nullptr, nullptr, nullptr);
			OnRenderUI(renderUIArgs);

			m_pWindow->Present();
		}
		m_pRenderDevice->Unlock();
	}

	return static_cast<int>(msg.wParam);
}

void Application::DoAfterRun()
{
	OnTerminate(EventArgs(*this));
	OnTerminated(EventArgs(*this));
}

std::shared_ptr<IRenderDevice> Application::GetRenderDevice() const
{
	_ASSERT(m_pRenderDevice);
	return m_pRenderDevice;
}

void Application::SetRenderDevice(std::shared_ptr<IRenderDevice> _renderDevice)
{
	m_pRenderDevice = _renderDevice;
}

std::shared_ptr<RenderWindow> Application::GetRenderWindow() const
{
	_ASSERT(m_pWindow);
	return m_pWindow;
}

void Application::SetRenderWindow(std::shared_ptr<RenderWindow> _renderWindow)
{
	if (m_pWindow != nullptr)
	{
		m_pWindow->HideWindow();

		Initialize.disconnect(InitializeConnection);
		Update.disconnect(UpdateConnection);
		Terminate.disconnect(TerminateConnection);

		m_pWindow->CloseWindow();
		m_pWindow.reset();
	}

	m_pWindow = _renderWindow;

	InitializeConnection = Initialize.connect(&RenderWindow::OnInitialize, m_pWindow, std::placeholders::_1);
	UpdateConnection     = Update    .connect(&RenderWindow::OnUpdate,     m_pWindow, std::placeholders::_1);
	TerminateConnection  = Terminate .connect(&RenderWindow::OnTerminate,  m_pWindow, std::placeholders::_1);
	
	m_pWindow->ShowWindow();

	gs_WindowHandle = m_pWindow;

	if (m_bIsRunning)
	{
		DoBeforeRun();
	}
}

CLoader* Application::GetLoader()
{
    return &m_Loader;
}


//
// IGameStateManager
//

void Application::AddGameState(GameStatesNames::List _name, std::shared_ptr<IGameState> _gameState)
{
	_ASSERT(_gameState != nullptr);
	_ASSERT(m_GameStatesCollection.find(_name) == m_GameStatesCollection.end());

	m_GameStatesCollection.insert(std::make_pair(_name, _gameState));
}

bool Application::SetGameState(GameStatesNames::List _name)
{
	_ASSERT(m_GameStatesCollection.find(_name) != m_GameStatesCollection.end());

	std::shared_ptr<IGameState> gameState = m_GameStatesCollection[_name];
	return SetGameState(gameState);
}

bool Application::SetGameState(std::shared_ptr<IGameState> _newGameState)
{
	_ASSERT(_newGameState);

	Log::Print("GameStateManager[]: Setting new CGameState.");

	// 1. Unset current GameState
	if (m_CurrentGameState != nullptr)
	{
		m_CurrentGameState->Unset();
		m_CurrentGameState->SetCurrent(false);
	}

	// 2. If new GameState not inited, init them
	if (!_newGameState->IsInited())
	{
		Log::Warn("GameStateManager[]: New CGameState in not inited. Initializating.");
		if (_newGameState->Init())
		{
			Log::Green("GameStateManager[]: New CGameState is inited successfully.");
			_newGameState->SetInited(true);
		}
		else
		{
			Log::Error("GameStateManager[]: Error initing new CGameState.");
		}
	}

	// 3. Set new GameState
	m_CurrentGameState = std::dynamic_pointer_cast<CGameState, IGameState>(_newGameState);
	if (m_CurrentGameState->Set())
	{
		Log::Green("GameStateManager[]: New CGameState is current now.");
		m_CurrentGameState->SetCurrent(true);
	}
	else
	{
		Log::Error("GameStateManager[]: Error setting current new CGameState.");
	}

	return true;
}

std::shared_ptr<IGameState> Application::GetGameState()
{
	return m_CurrentGameState;
}





LRESULT CALLBACK Application::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (gs_WindowHandle == NULL)
	{
		if (message == WM_CREATE)
			return 0;

		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	
	return gs_WindowHandle->WndProc(hwnd, message, wParam, lParam);
}



//---------------------------------------------------------------
//-- EVENTS
//---------------------------------------------------------------

void Application::OnInitialize(EventArgs& e)
{
	if (m_bIsInitialized)
		return;

	Initialize(e);

	m_bIsInitialized = true;
}

void Application::OnUpdate(UpdateEventArgs& e)
{
	Update(e);
}

void Application::OnTerminate(EventArgs& e)
{
	Terminate(e);

	m_pWindow.reset();
}

void Application::OnTerminated(EventArgs& e)
{
	m_bIsInitialized = false;

	Terminated(e);
}

void Application::OnExit(EventArgs& e)
{
	Exit(e);

	// Destroy any windows that are still hanging around.
	if (m_pWindow)
		m_pWindow->CloseWindow();

	// Setting this to false will cause the main application's message pump to stop.
	m_bIsRunning = false;
}

void Application::OnUserEvent(UserEventArgs& e)
{
	UserEvent(e);
}

void Application::OnRender(RenderEventArgs& e)
{
	m_pWindow->OnPreRender(e);
	m_pWindow->OnRender(e);
	m_pWindow->OnPostRender(e);
}

void Application::OnRenderUI(RenderEventArgs & e)
{
	m_pWindow->OnRenderUI(e);
}
