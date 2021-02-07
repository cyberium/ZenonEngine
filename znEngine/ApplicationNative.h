#pragma once

#include "Application.h"

class ZN_API CApplicationNative
	: public CApplication
{
public:
	CApplicationNative();
	virtual ~CApplicationNative();

	IRenderDevice&                  CreateRenderDevice(RenderDeviceType DeviceType);
	void                            AddRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow);
	void                            RemoveRenderWindow(std::shared_ptr<IRenderWindow> RenderWindow);

	// IApplication
	IRenderDevice&                  GetRenderDevice() const override;

protected:
	void DoBeforeRunImpl() override;
	void DoRunImpl(UpdateEventArgs& UpdateEventArgs) override;
	void DoAfterRunImpl() override;

	void OnStopAfterPluginManager() override;

private:
	std::unique_ptr<IRenderDevice> m_RenderDevice;
	std::vector<std::shared_ptr<IRenderWindow>> m_Windows;
};
