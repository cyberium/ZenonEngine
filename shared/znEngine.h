#pragma once

#include "../znEngine/Application.h"
#include "../znEngine/NativeWindowFactory.h"
#include "../znEngine/NativeWindow_WindowsSpecific.h"

// Nodes
#include "../znEngine/SceneFunctional/UI/UIText.h"
#include "../znEngine/SceneFunctional/UI/UIButton.h"
#include "../znEngine/SceneFunctional/UI/UITexture.h"

// Materials
#include "../znEngine/Materials/MaterialDebug.h"
#include "../znEngine/Materials/MaterialTextured.h"
#include "../znEngine/Materials/MaterialParticle.h"

// Passes
#include "../znEngine/Passes/UIFontPass.h"
#include "../znEngine/Passes/UIButtonPass.h"
#include "../znEngine/Passes/UITexturePass.h"

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
