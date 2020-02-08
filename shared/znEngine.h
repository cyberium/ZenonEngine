#pragma once

#include "../znEngine/Application.h"
#include "../znEngine/NativeWindowFactory.h"
#include "../znEngine/NativeWindow_WindowsSpecific.h"

#include "../znEngine/Materials/MaterialDebug.h"
#include "../znEngine/Materials/MaterialTextured.h"

#include "../znEngine/SceneFunctional/UI/Fonts/FontsManager.h"
#include "../znEngine/Passes/UIFontPass.h"

#include "../znEngine/Formats/ChunkedFile.h"
#include "../znEngine/Images/DDSFormat.h"
#include "../znEngine/Images/ImageLoaderTemplate.h"

/*
 * Initialize improtant managers and plugins
 */
ZN_API IBaseManager* WINAPI InitializeEngine(std::vector<std::string> Arguments, std::string PathToPlugins);