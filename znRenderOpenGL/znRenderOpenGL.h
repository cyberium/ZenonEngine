#pragma once

// Common includes
#include "Common.h"

OW_ENGINE_API std::shared_ptr<IRenderDevice> CreateRenderDeviceOGL(std::shared_ptr<CBaseManager> _BaseManager);
OW_ENGINE_API std::shared_ptr<RenderWindow>  CreateRenderWindowOGL(std::shared_ptr<IRenderDevice> device, IWindowObject * WindowObject, bool vSync);