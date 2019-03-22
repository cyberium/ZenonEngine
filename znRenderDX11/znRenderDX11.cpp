#include "stdafx.h"


// General
#include "znRenderDX11.h"


// Additional
#include "RenderDeviceDX11.h"
#include "RenderWindowDX11.h"


std::shared_ptr<IRenderDevice> CreateRenderDeviceDX11(std::shared_ptr<CBaseManager> _baseManager)
{
	return std::make_shared<RenderDeviceDX11>();
}

std::shared_ptr<RenderWindow> CreateRenderWindowDX11(std::shared_ptr<IRenderDevice> device, IWindowObject * WindowObject, bool vSync)
{
	std::shared_ptr<RenderDeviceDX11> pDevice = std::dynamic_pointer_cast<RenderDeviceDX11, IRenderDevice>(device);
	assert1(pDevice != NULL);

	return std::make_shared<RenderWindowDX11>(pDevice, WindowObject, vSync);
}