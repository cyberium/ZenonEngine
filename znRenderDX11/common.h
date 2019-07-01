#pragma once

#ifdef OW_ENGINE_EXPORTS
#define OW_ENGINE_API __declspec(dllexport) 
#else
#define OW_ENGINE_API __declspec(dllimport) 
#endif