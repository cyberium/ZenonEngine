#pragma once

#ifdef OW_RENDER_OPENGL_EXPORTS
#define OW_RENDER_OPENGL_API __declspec(dllexport) 
#else
#define OW_RENDER_OPENGL_API __declspec(dllimport) 
#endif