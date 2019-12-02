#pragma once

// Forward BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IRenderWindow;
class CLoader;
// Forward END

ZN_INTERFACE OW_ENGINE_API IApplication
{
	virtual ~IApplication() = 0 {};

	virtual void                            DoBeforeRun() = 0;
	virtual int                             DoRun() = 0;
	virtual void                            DoAfterRun() = 0;

	virtual std::shared_ptr<IBaseManager>   GetBaseManager() const = 0;
	virtual std::shared_ptr<IRenderDevice>  GetRenderDevice() const = 0;
	virtual void                            SetRenderDevice(std::shared_ptr<IRenderDevice> RenderDevice) = 0;
	//virtual std::shared_ptr<IRenderWindow>  GetRenderWindow() const = 0;
	//virtual void                            SetRenderWindow(std::shared_ptr<IRenderWindow>  RenderDevice) = 0;

	virtual HINSTANCE                       GetHINSTANCE() = 0;
    virtual CLoader*                        GetLoader() = 0;
};

ZN_INTERFACE OW_ENGINE_API IApplicationEvents
{
	virtual Event&                          Initialize() = 0;
	virtual void                          OnInitialize(EventArgs& e) = 0;
	virtual UpdateEvent&                    Update() = 0;
	virtual void                          OnUpdate(UpdateEventArgs& e) = 0;
	virtual Event&			                Terminate() = 0;
	virtual void                          OnTerminate(EventArgs& e) = 0;
	virtual Event&                          Terminated() = 0;
	virtual void                          OnTerminated(EventArgs& e) = 0;
	virtual Event&                          Exit() = 0;
	virtual void                          OnExit(EventArgs& e) = 0;
	virtual UserEvent&                      UserEvent() = 0;
	virtual void                          OnUserEvent(UserEventArgs& e) = 0;

	virtual LRESULT CALLBACK				WndProc(HWND, UINT, WPARAM, LPARAM) = 0;
};

ZN_INTERFACE OW_ENGINE_API IApplicationEventsConnection
{
	virtual void Connect(IApplicationEvents* ApplicationEvents) = 0;
	virtual void Disconnect(IApplicationEvents* ApplicationEvents) = 0;
};