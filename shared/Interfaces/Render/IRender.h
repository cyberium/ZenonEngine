#pragma once

enum class ZN_API RenderDeviceType 
{
	RenderDeviceType_None = 0,
	RenderDeviceType_DirectX11,
	RenderDeviceType_DirectX12,
	RenderDeviceType_OpenGL
};

ZN_INTERFACE ZN_API IznRenderDeviceCreator
{
	virtual ~IznRenderDeviceCreator() {}

	virtual RenderDeviceType GetRenderDeviceType() const = 0;
	virtual std::unique_ptr<IRenderDevice> CreateRenderDevice() const = 0;
};

ZN_INTERFACE ZN_API __declspec(uuid("E0313DD4-A169-4369-BA8C-2F10323832CB")) IznRenderDeviceFactory 
	: public IManager
{
	virtual ~IznRenderDeviceFactory() {}

	virtual void RegisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator) = 0;
	virtual void UnregisterRenderDeviceCreator(std::shared_ptr<IznRenderDeviceCreator> RenderDeviceCreator) = 0;
	virtual const IznRenderDeviceCreator& GetRenderDeviceCreator(RenderDeviceType RenderDeviceType) const = 0;
};