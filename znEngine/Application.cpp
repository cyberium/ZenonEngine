#include "stdafx.h"

// Genreal
#include "Application.h"

// Additional
#include "EngineTime.h"
#include "GameState_Default.h"

float g_GameDeltaTime = 0.0f;
float g_ApplicationTime = 0.0f;
int64_t g_FrameCounter = 0L;

Application::Application(IBaseManager* BaseManager)
	: m_BaseManager(BaseManager)
	, m_bIsInitialized(false)
	, m_bIsRunning(false)
{
	m_HINSTANCE = ::GetModuleHandle(NULL);
}

Application::Application(IBaseManager* BaseManager, HINSTANCE hInstance)
	: m_BaseManager(BaseManager)
	, m_bIsInitialized(false)
	, m_bIsRunning(false)
{
	m_HINSTANCE = hInstance;
}

Application::~Application()
{
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


std::shared_ptr<IRenderDevice> Application::CreateRenderDevice(RenderDeviceType DeviceType)
{
	SetRenderDevice(GetManager<IznRenderDeviceFactory>(m_BaseManager)->GetRenderDeviceCreator(DeviceType)->CreateRenderDevice());
	return GetRenderDevice();
}

void Application::AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	std::dynamic_pointer_cast<IApplicationEventsConnection>(RenderWindow)->Connect(this);

	RenderWindow->ShowWindow();

	if (m_bIsRunning)
	{
		DoBeforeRun();
	}

	m_Windows.insert(std::make_pair(RenderWindow->GetHWnd(), RenderWindow));
}

void Application::DeleleRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow)
{
	
}



//
// IApplication
//

void Application::DoBeforeRun()
{
	OnInitialize(EventArgs(this));

	m_bIsRunning = true;
}

int Application::DoRun()
{
	static Timer elapsedTime;

	MSG msg;
	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			OnExit(EventArgs(this));
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	g_GameDeltaTime = elapsedTime.GetElapsedTime();
	g_ApplicationTime += g_GameDeltaTime;
	g_FrameCounter++;

	UpdateEventArgs updateArgs(this, g_GameDeltaTime* 166.0f, g_ApplicationTime* 166.0f, g_FrameCounter);
	OnUpdate(updateArgs);

	return static_cast<int>(msg.wParam);
}

void Application::DoAfterRun()
{
	OnTerminate(EventArgs(this));
	OnTerminated(EventArgs(this));
}

IBaseManager* Application::GetBaseManager() const
{
	return m_BaseManager;
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

HINSTANCE Application::GetHINSTANCE()
{
	return m_HINSTANCE;
}


//
// IGameStateManager
//
void Application::AddGameState(GameStatesNames _name, std::shared_ptr<IGameState> _gameState)
{
	_ASSERT(_gameState != nullptr);
	_ASSERT(m_GameStatesCollection.find(_name) == m_GameStatesCollection.end());

	m_GameStatesCollection.insert(std::make_pair(_name, _gameState));
}

bool Application::SetGameState(GameStatesNames _name)
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
	//if (m_CurrentGameState != nullptr)
	//{
	//	m_CurrentGameState->Unset();
	//	m_CurrentGameState->SetCurrent(false);
	//}

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
	m_CurrentGameState = std::dynamic_pointer_cast<CGameState>(_newGameState);
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
