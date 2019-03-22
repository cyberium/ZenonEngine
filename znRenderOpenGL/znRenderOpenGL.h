#pragma once

// Common includes
#include "Common.h"

OW_RENDER_OPENGL_API std::shared_ptr<IRenderDevice> CreateRenderDeviceOGL(std::shared_ptr<CBaseManager> _BaseManager);
OW_RENDER_OPENGL_API std::shared_ptr<RenderWindow>  CreateRenderWindowOGL(HWND hWnd, std::shared_ptr<IRenderDevice> device, cstring windowName, int windowWidth, int windowHeight, bool vSync);