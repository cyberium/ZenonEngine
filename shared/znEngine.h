#pragma once

#include "../znEngine/Application.h"
#include "../znEngine/NativeWindowFactory.h"
#include "../znEngine/NativeWindow_WindowsSpecific.h"

#include "../znEngine/Materials/MaterialDebug.h"
#include "../znEngine/Materials/MaterialTextured.h"
#include "../znEngine/Materials/MaterialParticle.h"

#include "../znEngine/Passes/UIFontPass.h"
#include "../znEngine/Passes/DrawBoundingBoxPass.h"
#include "../znEngine/Passes/MaterialParticlePass.h"

#include "../znEngine/Formats/ChunkedFile.h"
#include "../znEngine/Formats/Fonts/FontsManager.h"
#include "../znEngine/Formats/Images/DDSFormat.h"
#include "../znEngine/Formats/Images/ImageLoaderTemplate.h"

/*
 * Initialize improtant managers and plugins
 */
ZN_API IBaseManager* WINAPI InitializeEngine(std::vector<std::string> Arguments, std::string PathToPlugins);
