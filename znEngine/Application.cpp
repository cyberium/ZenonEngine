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


IRenderDevice* Application::CreateRenderDevice(RenderDeviceType DeviceType)
{
	SetRenderDevice(m_BaseManager->GetManager<IznRenderDeviceFactory>()->GetRenderDeviceCreator(DeviceType)->CreateRenderDevice());
	return GetRenderDevice();
}

void Application::AddRenderWindow(IRenderWindow* RenderWindow)
{
	dynamic_cast<IApplicationEventsConnection*>(RenderWindow)->Connect(this);

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
				if (it != nullptr)
					SendMessage(it->GetWindowObject()->GetHWnd(), WM_CLOSE, NULL, NULL); // TODO: Investigate me!

			// Setting this to false will cause the main application's message pump to stop.
			m_bIsRunning = false;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	g_GameDeltaTime = elapsedTime.GetElapsedTime();
	g_ApplicationTime += g_GameDeltaTime;
	g_FrameCounter++;

	UpdateEventArgs updateArgs(this, g_GameDeltaTime * 166.0f, g_ApplicationTime * 166.0f, g_FrameCounter);
	if (! m_Update(updateArgs))
	{
		m_bIsRunning = false;
	}

	return static_cast<int>(msg.wParam);
}

void Application::DoAfterRun()
{
	m_Terminate(EventArgs(this));
	m_Terminated(EventArgs(this));
}

IBaseManager* Application::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice* Application::GetRenderDevice() const
{
	_ASSERT(m_pRenderDevice);
	return m_pRenderDevice;
}

void Application::SetRenderDevice(IRenderDevice* _renderDevice)
{
	m_pRenderDevice = _renderDevice;
}

HINSTANCE Application::GetHINSTANCE()
{
	return m_HINSTANCE;
}



//
// IApplicationEvents
//
Event& Application::Initialize()
{
	return m_Initialize;
}

UpdateEvent& Application::Update()
{
	return m_Update;
}

Event& Application::Terminate()
{
	return m_Terminate;
}

Event& Application::Terminated()
{
	return m_Terminated;
}

Event& Application::Exit()
{
	return m_Exit;
}

UserEvent& Application::UserEvent()
{
	return m_UserEvent;
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
