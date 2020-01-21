#pragma once

class ZN_API Application :
	public Object, 
	public IApplication, 
	public IApplicationEvents,
	public IGameStateManager
{
public:
	Application(IBaseManager* BaseManager);
	Application(IBaseManager* BaseManager, HINSTANCE hInstance);
	virtual ~Application();

	// Default query
	int                             Run();
	void                            Stop();

	// Creators
	IRenderDevice*  CreateRenderDevice(RenderDeviceType DeviceType);
	void                            AddRenderWindow(IRenderWindow* RenderWindow);
	void                            DeleleRenderWindow(IRenderWindow* RenderWindow);

	// IApplication
	void                            DoBeforeRun() override;
	int                             DoRun() override;
	void                            DoAfterRun() override;

	IBaseManager*					GetBaseManager() const override;
	IRenderDevice*  GetRenderDevice() const override;
	void                            SetRenderDevice(IRenderDevice* _renderDevice) override;
	HINSTANCE                       GetHINSTANCE() override;

	// IApplicationEvents
	Event&                          Initialize();
	UpdateEvent&                    Update();
	Event&			                Terminate();
	Event&                          Terminated();
	Event&                          Exit();
	Delegate<UserEventArgs>&        UserEvent();

	// IGameStateManager
	void                            AddGameState(GameStatesNames _name, std::shared_ptr<IGameState> _gameState) override;
	bool                            SetGameState(GameStatesNames _name) override;
	bool                            SetGameState(std::shared_ptr<IGameState> _newGameState) override;
	std::shared_ptr<IGameState>     GetGameState() override;

private:
	IBaseManager*	m_BaseManager;
	bool                            m_bIsInitialized;
	bool                            m_bIsRunning;

	HINSTANCE                       m_HINSTANCE;

	IRenderDevice*  m_pRenderDevice;
	std::vector<IRenderWindow*> m_Windows;

	// IGameStateManager
	std::shared_ptr<IGameState>                                     m_CurrentGameState;
	std::map<GameStatesNames, std::shared_ptr<IGameState>>    m_GameStatesCollection;

private: // IApplicationEvents
	Event									 m_Initialize;
	UpdateEvent								 m_Update;
	Event									 m_Terminate;
	Event									 m_Terminated;
	Event									 m_Exit;
	Delegate<UserEventArgs>					 m_UserEvent;
};
