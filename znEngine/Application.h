#pragma once

class ZN_API CApplication
	: public IApplication
{
public:
	CApplication();
	virtual ~CApplication();

	int                             Run();
	void                            Stop();

	// IApplication
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
	void InitializeEngineInternal(std::initializer_list<std::string> Plugins);

	virtual void DoBeforeRunImpl() = 0;
	virtual void DoRunImpl(UpdateEventArgs& UpdateEventArgs) = 0;
	virtual void DoAfterRunImpl() = 0;

	virtual void OnStopAfterPluginManager() = 0;

	void SetStopped();
	
	virtual int ProcessEvents() = 0;
	virtual void ExitApplication() = 0;


protected:
	bool m_IsInitialized;
	bool m_IsRunning;

	double m_GameDeltaTime;
	double m_GameTime;
	size_t m_FrameCounter;

	std::unique_ptr<IBaseManager> m_BaseManager;
	std::shared_ptr<IznPluginsManager> m_PluginsManager;
};
