#pragma once

class ZN_API Application 
	: public IApplication
	, public IApplication_WindowsSpecific
{
public:
	Application(IBaseManager& BaseManager);
	Application(IBaseManager& BaseManager, HINSTANCE hInstance);
	virtual ~Application();

	// Default query
	int                             Run();
	void                            Stop();

	// Creators
	IRenderDevice&                  CreateRenderDevice(RenderDeviceType DeviceType);

	void                            AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow);
	void                            CloseRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow);

	// IApplication
	void                            DoBeforeRun() override;
	int                             DoRun() override;
	void                            DoAfterRun() override;
	double                          GetGameDeltaTime() const override;
	double                          GetGameTime() const override;
	size_t                          GetFrameCounter() const override;
	IBaseManager&					GetBaseManager() const override;
	IRenderDevice&                  GetRenderDevice() const override;

	// IApplication_WindowsSpecific
	HINSTANCE                       GetHInstance() const override;

private:
	bool m_IsInitialized;
	bool m_IsRunning;

	double m_GameDeltaTime;
	double m_GameTime;
	size_t m_FrameCounter;

	IBaseManager& m_BaseManager;
	std::unique_ptr<IRenderDevice> m_RenderDevice;
	std::vector<std::shared_ptr<IRenderWindow>> m_Windows;

	HINSTANCE m_HInstance;
};
