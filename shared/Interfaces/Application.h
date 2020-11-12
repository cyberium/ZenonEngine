#pragma once

#include <Windows.h>

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

	virtual double                          GetGameDeltaTime() const = 0;
	virtual double                          GetGameTime() const = 0;
	virtual size_t                          GetFrameCounter() const = 0;

	virtual IBaseManager&                   GetBaseManager() const = 0;
	virtual IRenderDevice&                  GetRenderDevice() const = 0;
};


ZN_INTERFACE ZN_API IApplication_WindowsSpecific
{
	virtual ~IApplication_WindowsSpecific() {}

	virtual HINSTANCE GetHInstance() const = 0;
};

ZN_INTERFACE ZN_API IApplicationEventsListener
{
	virtual ~IApplicationEventsListener() {}

	virtual void OnInitialize(EventArgs& Args) = 0;
	virtual void OnUpdate(UpdateEventArgs& Args) = 0;
	virtual void OnExit(EventArgs& Args) = 0;
	virtual void OnUserEvent(UserEventArgs& Args) = 0;
};