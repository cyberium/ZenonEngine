#pragma once

// Forward BEGIN
ZN_INTERFACE IRenderDevice;
ZN_INTERFACE IRenderWindow;
class CLoader;
// Forward END

struct OW_ENGINE_API IApplication
{
	virtual ~IApplication() = 0 {};

	virtual void                            DoBeforeRun() = 0;
	virtual int                             DoRun() = 0;
	virtual void                            DoAfterRun() = 0;

	virtual std::shared_ptr<IBaseManager>   GetBaseManager() const = 0;
	virtual std::shared_ptr<IRenderDevice>  GetRenderDevice() const = 0;
	virtual void                            SetRenderDevice(std::shared_ptr<IRenderDevice> RenderDevice) = 0;
	virtual std::shared_ptr<IRenderWindow>   GetRenderWindow() const = 0;
	virtual void                            SetRenderWindow(std::shared_ptr<IRenderWindow>  RenderDevice) = 0;

    virtual CLoader*                        GetLoader() = 0;
};
