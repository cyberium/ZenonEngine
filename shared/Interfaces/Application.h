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

	virtual IBaseManager*                   GetBaseManager() const = 0;
	virtual std::shared_ptr<IRenderDevice>  GetRenderDevice() const = 0;
	virtual void                            SetRenderDevice(std::shared_ptr<IRenderDevice> RenderDevice) = 0;

	virtual HINSTANCE                       GetHINSTANCE() = 0;
};

ZN_INTERFACE ZN_API IApplicationEvents
{
	virtual ~IApplicationEvents() {}

	virtual Event&                          Initialize() = 0;
	virtual UpdateEvent&                    Update() = 0;
	virtual Event&			                Terminate() = 0;
	virtual Event&                          Terminated() = 0;
	virtual Event&                          Exit() = 0;
	virtual UserEvent&                      UserEvent() = 0;
};

ZN_INTERFACE ZN_API IApplicationEventsConnection
{
	virtual ~IApplicationEventsConnection() {}

	virtual void Connect(IApplicationEvents* ApplicationEvents) = 0;
	virtual void Disconnect(IApplicationEvents* ApplicationEvents) = 0;
};