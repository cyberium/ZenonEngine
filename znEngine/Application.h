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
	IBaseManager&					GetBaseManager() const override;
	IRenderDevice&                  GetRenderDevice() const override;

	// IApplication_WindowsSpecific
	HINSTANCE                       GetHInstance() const override;

private:
	bool m_bIsInitialized;
	bool m_bIsRunning;

	IBaseManager& m_BaseManager;
	std::unique_ptr<IRenderDevice> m_pRenderDevice;
	std::vector<std::shared_ptr<IRenderWindow>> m_Windows;

	HINSTANCE m_HInstance;
};
