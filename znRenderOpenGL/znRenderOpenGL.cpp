#include "stdafx.h"

// General
#include "znRenderOpenGL.h"

// Additional
#include "RenderDeviceOGL.h"
#include "RenderWindowOGL.h"

std::shared_ptr<CBaseManager> _BaseManager = nullptr;

std::shared_ptr<IRenderDevice> CreateRenderDeviceOGL(std::shared_ptr<CBaseManager> _baseManager)
{
	_BaseManager = _baseManager;
	return std::make_shared<RenderDeviceOGL>();
}

std::shared_ptr<RenderWindow> CreateRenderWindowOGL(HWND hWnd, std::shared_ptr<IRenderDevice> device, cstring windowName, int windowWidth, int windowHeight, bool vSync)
{
	std::shared_ptr<RenderDeviceOGL> pDevice = std::dynamic_pointer_cast<RenderDeviceOGL, IRenderDevice>(device);
	assert1(pDevice != NULL);

	return std::make_shared<RenderWindowOGL>(hWnd, pDevice, windowName, windowWidth, windowHeight, vSync);
}