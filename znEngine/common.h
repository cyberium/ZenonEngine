#pragma once

#define ONLY_SIMPLE_UI

#ifdef OW_ENGINE_EXPORTS
#define OW_ENGINE_API __declspec(dllexport) 
#else
#define OW_ENGINE_API __declspec(dllimport) 
#endif

#include "MeshWrapper.h"