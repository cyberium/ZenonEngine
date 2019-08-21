#pragma once

// Common includes
#include "Common.h"

OW_ENGINE_API std::shared_ptr<IRenderDevice> CreateRenderDeviceDX11(std::shared_ptr<CBaseManager> _BaseManager);
OW_ENGINE_API std::shared_ptr<RenderWindow>  CreateRenderWindowDX11(std::shared_ptr<IRenderDevice> device, IWindowObject * WindowObject, bool vSync);