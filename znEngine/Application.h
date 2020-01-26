#pragma once

class ZN_API Application :
	public Object, 
	public IApplication, 
	public IApplicationEvents
{
public:
	Application(IBaseManager* BaseManager);
	Application(IBaseManager* BaseManager, HINSTANCE hInstance);
	virtual ~Application();

	// Default query
	int                             Run();
	void                            Stop();

	// Creators
	IRenderDevice*                  CreateRenderDevice(RenderDeviceType DeviceType);
	void                            AddRenderWindow(IRenderWindow* RenderWindow);
	void                            DeleleRenderWindow(IRenderWindow* RenderWindow);

	// IApplication
	void                            DoBeforeRun() override;
	int                             DoRun() override;
	void                            DoAfterRun() override;

	IBaseManager*					GetBaseManager() const override;
	IRenderDevice*                  GetRenderDevice() const override;
	void                            SetRenderDevice(IRenderDevice* _renderDevice) override;
	HINSTANCE                       GetHINSTANCE() override;

	// IApplicationEvents
	Event&                          ApplicationInitialize();
	UpdateEvent&                    ApplicationUpdate();
	Event&			                ApplicationTerminate();
	Event&                          ApplicationTerminated();
	Event&                          ApplicationExit();
	Delegate<UserEventArgs>&        ApplicationUserEvent();

private:
	IBaseManager*					m_BaseManager;
	bool                            m_bIsInitialized;
	bool                            m_bIsRunning;

	HINSTANCE                       m_HINSTANCE;

	IRenderDevice*					m_pRenderDevice;
	std::vector<IRenderWindow*>		m_Windows;

private: // IApplicationEvents
	Event									 m_Initialize;
	UpdateEvent								 m_Update;
	Event									 m_Terminate;
	Event									 m_Terminated;
	Event									 m_Exit;
	Delegate<UserEventArgs>					 m_UserEvent;
};
