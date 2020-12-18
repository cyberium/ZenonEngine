#pragma once

// Forward BEGIN
ZN_INTERFACE IznNativeWindow;
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IRenderWindow;
ZN_INTERFACE IznPlugin;
// Forward END

ZN_INTERFACE ZN_API IApplication
{
	virtual ~IApplication() = 0 {};

	virtual std::unique_ptr<IznNativeWindow>         CreateNativeWindow(const std::string& WindowName, glm::ivec2 WindowSize) const = 0;
	virtual std::unique_ptr<IznNativeWindow>         CreateNativeWindowProxy(IznNativeWindow& SourceNativeWindow) const = 0;
	virtual IRenderDevice&                           CreateRenderDevice(RenderDeviceType DeviceType) = 0;
	virtual void                                     AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow) = 0;
	virtual void                                     CloseRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow) = 0;
	virtual void                                     DoBeforeRun() = 0;
	virtual int                                      DoRun() = 0;
	virtual void                                     DoAfterRun() = 0;

	virtual double                                   GetGameDeltaTime() const = 0;
	virtual double                                   GetGameTime() const = 0;
	virtual size_t                                   GetFrameCounter() const = 0;

	virtual IBaseManager&                            GetBaseManager() const = 0;
	virtual IRenderDevice&                           GetRenderDevice() const = 0;
};

ZN_INTERFACE ZN_API IApplicationEventsListener
{
	virtual ~IApplicationEventsListener() {}

	virtual void OnInitialize(EventArgs& Args) = 0;
	virtual void OnUpdate(UpdateEventArgs& Args) = 0;
	virtual void OnExit(EventArgs& Args) = 0;
	virtual void OnUserEvent(UserEventArgs& Args) = 0;
};

ZN_INTERFACE ZN_API IApplication_PlatformBase
{
	virtual ~IApplication_PlatformBase() {}

	virtual void ShowMessageBox(const std::string& Title, const std::string& Message) const = 0;
	virtual std::string GetExecutablePath() const = 0;
	virtual IznPlugin* LoadPlugin(const std::string& PluginFilename) const = 0;
	virtual void UnloadPlugin(const std::string& PluginFilename) const = 0;
};
