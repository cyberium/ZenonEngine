#pragma once

class ZN_API CApplicationBase
	: public IApplication
{
public:
	CApplicationBase();
	virtual ~CApplicationBase();

	int                             Run();
	void                            Stop();

	// IApplication
	IRenderDevice&                  CreateRenderDevice(RenderDeviceType DeviceType);
	void                            AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow);
	void                            CloseRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow);
	void                            DoBeforeRun() override;
	int                             DoRun() override;
	void                            DoAfterRun() override;
	double                          GetGameDeltaTime() const override;
	double                          GetGameTime() const override;
	size_t                          GetFrameCounter() const override;
	IBaseManager&					GetBaseManager() const override;
	IRenderDevice&                  GetRenderDevice() const override;

protected:
	void PreInitializeEngine();
	void InitializeEngineInternal();
	void SetStopped();
	
	virtual int ProcessEvents() = 0;
	virtual void ExitApplication() = 0;



private:
	bool m_IsInitialized;
	bool m_IsRunning;

	double m_GameDeltaTime;
	double m_GameTime;
	size_t m_FrameCounter;

	std::unique_ptr<IBaseManager> m_BaseManager;
	std::shared_ptr<IznPluginsManager> m_PluginsManager;
	std::unique_ptr<IRenderDevice> m_RenderDevice;
	std::vector<std::shared_ptr<IRenderWindow>> m_Windows;
};
