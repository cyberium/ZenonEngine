#pragma once

#ifdef OW_RENDER_DX11_EXPORTS
#define OW_RENDER_DX11_API //__declspec(dllexport) 
#else
#define OW_RENDER_DX11_API //__declspec(dllimport) 
#endif