#pragma once

#include "GameState.h"
#include "Loader.h"

class OW_ENGINE_API Application :
	public Object, 
	public IApplication, 
	public IApplicationEvents,
	public IGameStateManager
{
public:
	Application(IBaseManager* BaseManager);
	Application(IBaseManager* BaseManager, HINSTANCE hInstance);
	virtual ~Application();

	static IApplication&            Get();

	// Default query
	int                             Run();
	void                            Stop();

	// Creators
	std::shared_ptr<IRenderDevice>  CreateRenderDevice(RenderDeviceType DeviceType);
	void                            AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow);
	void                            DeleleRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow);

	// IApplication
	void                            DoBeforeRun() override;
	int                             DoRun() override;
	void                            DoAfterRun() override;
	IBaseManager*   GetBaseManager() const override;
	std::shared_ptr<IRenderDevice>  GetRenderDevice() const override;
	void                            SetRenderDevice(std::shared_ptr<IRenderDevice> _renderDevice) override;
	HINSTANCE                       GetHINSTANCE() override;
    CLoader*						GetLoader() override;

	// IApplicationEvents
	Event&                          Initialize();
	bool                          OnInitialize(EventArgs& e);
	UpdateEvent&                    Update();
	bool                          OnUpdate(UpdateEventArgs& e);
	Event&			                Terminate();
	bool                          OnTerminate(EventArgs& e);
	Event&                          Terminated();
	bool                          OnTerminated(EventArgs& e);
	Event&                          Exit();
	bool                          OnExit(EventArgs& e);
	Delegate<UserEventArgs>&        UserEvent();
	bool                          OnUserEvent(UserEventArgs& e);
	LRESULT CALLBACK				WndProc(HWND, UINT, WPARAM, LPARAM);

	// IGameStateManager
	void                            AddGameState(GameStatesNames _name, std::shared_ptr<IGameState> _gameState) override;
	bool                            SetGameState(GameStatesNames _name) override;
	bool                            SetGameState(std::shared_ptr<IGameState> _newGameState) override;
	std::shared_ptr<IGameState>     GetGameState() override;

private:
	IBaseManager*	m_BaseManager;
	bool                            m_bIsInitialized;
	bool                            m_bIsRunning;

	// Handle to the module.
	CLoader                         m_Loader;
	HINSTANCE                       m_HINSTANCE;

	std::shared_ptr<IRenderDevice>  m_pRenderDevice;
	std::unordered_map<HWND, std::shared_ptr<IRenderWindow>> m_Windows;

	// IGameStateManager
	std::shared_ptr<IGameState>                                     m_CurrentGameState;
	std::unordered_map<GameStatesNames, std::shared_ptr<IGameState>>    m_GameStatesCollection;

private: // IApplicationEvents
	Event									 m_Initialize;
	UpdateEvent								 m_Update;
	Event									 m_Terminate;
	Event									 m_Terminated;
	Event									 m_Exit;
	Delegate<UserEventArgs>					 m_UserEvent;
};

extern IApplication* _ApplicationInstance;

#define _RenderDevice Application::Get().GetRenderDevice()
