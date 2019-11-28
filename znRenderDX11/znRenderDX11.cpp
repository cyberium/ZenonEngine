#include "stdafx.h"


// General
#include "znRenderDX11.h"


// Additional
#include "RenderDeviceDX11.h"
#include "RenderWindowDX11.h"

std::shared_ptr<IRenderDevice> CreateRenderDeviceDX11(std::shared_ptr<IBaseManager> BaseManager)
{
	std::shared_ptr<IRenderDevice> renderDevice = std::make_shared<RenderDeviceDX11>(BaseManager);
	renderDevice->Initialize();
	return renderDevice;
}

std::shared_ptr<RenderWindow> CreateRenderWindowDX11(std::shared_ptr<IRenderDevice> device, IWindowObject * WindowObject, bool vSync)
{
	std::shared_ptr<RenderDeviceDX11> pDevice = std::dynamic_pointer_cast<RenderDeviceDX11, IRenderDevice>(device);
	_ASSERT(pDevice != NULL);

	return std::make_shared<RenderWindowDX11>(pDevice, WindowObject, vSync);
}