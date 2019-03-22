#pragma once

// Forward BEGIN
class IRenderDevice;
class RenderWindow;
// Forward END

struct IApplication
{
	virtual ~IApplication() = 0 {};

	virtual void                            DoBeforeRun() = 0;
	virtual int                             DoRun() = 0;
	virtual void                            DoAfterRun() = 0;

	virtual std::shared_ptr<IRenderDevice>  GetRenderDevice() const = 0;
	virtual void                            SetRenderDevice(std::shared_ptr<IRenderDevice> RenderDevice) = 0;
	virtual std::shared_ptr<RenderWindow>   GetRenderWindow() const = 0;
	virtual void                            SetRenderWindow(std::shared_ptr<RenderWindow>  RenderDevice) = 0;
};
