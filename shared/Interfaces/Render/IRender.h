#pragma once

ZN_INTERFACE ZN_API IznRenderDeviceCreator
{
	virtual ~IznRenderDeviceCreator() {}

	virtual RenderDeviceType GetRenderDeviceType() const = 0;
	virtual std::unique_ptr<IRenderDevice> CreateRenderDevice() const = 0;
};

ZN_INTERFACE ZN_API IznRenderDeviceFactory 
	: public IManager
{
	ZN_OBJECTCLASS(cRenderDeviceFactory)

	virtual ~IznRenderDeviceFactory() {}

	virtual void RegisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator) = 0;
	virtual void UnregisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator) = 0;
	virtual const IznRenderDeviceCreator& GetRenderDeviceCreator(RenderDeviceType RenderDeviceType) const = 0;
};