#pragma once

// Forward BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IRenderWindow;
// Forward END

ZN_INTERFACE ZN_API IApplication
{
	virtual ~IApplication() = 0 {};

	virtual void                            DoBeforeRun() = 0;
	virtual int                             DoRun() = 0;
	virtual void                            DoAfterRun() = 0;

	virtual IBaseManager&                   GetBaseManager() const = 0;
	virtual IRenderDevice&                  GetRenderDevice() const = 0;
};


ZN_INTERFACE ZN_API IApplication_WindowsSpecific
{
	virtual ~IApplication_WindowsSpecific() {}

	virtual HINSTANCE GetHInstance() const = 0;
};


ZN_INTERFACE ZN_API IApplicationEvents
{
	virtual ~IApplicationEvents() {}

	virtual Event&                          ApplicationInitialize() = 0;
	virtual UpdateEvent&                    ApplicationUpdate() = 0;
	virtual Event&			                ApplicationTerminate() = 0;
	virtual Event&                          ApplicationTerminated() = 0;
	virtual Event&                          ApplicationExit() = 0;
	virtual UserEvent&                      ApplicationUserEvent() = 0;
};

ZN_INTERFACE ZN_API IApplicationEventsConnection
{
	virtual ~IApplicationEventsConnection() {}

	virtual void Connect(IApplicationEvents* ApplicationEvents) = 0;
	virtual void Disconnect(IApplicationEvents* ApplicationEvents) = 0;
};