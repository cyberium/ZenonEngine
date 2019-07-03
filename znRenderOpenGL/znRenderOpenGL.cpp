#include "stdafx.h"

// General
#include "znRenderOpenGL.h"

// Additional
#include "RenderDeviceOGL.h"
#include "RenderWindowOGL.h"

std::shared_ptr<IRenderDevice> CreateRenderDeviceOGL(std::shared_ptr<CBaseManager> _baseManager)
{
	return std::make_shared<RenderDeviceOGL>();
}

std::shared_ptr<RenderWindow> CreateRenderWindowOGL(std::shared_ptr<IRenderDevice> device, IWindowObject * WindowObject, bool vSync)
{
	std::shared_ptr<RenderDeviceOGL> pDevice = std::dynamic_pointer_cast<RenderDeviceOGL, IRenderDevice>(device);
	_ASSERT(pDevice != NULL);

	return std::make_shared<RenderWindowOGL>(pDevice, WindowObject, vSync);
}