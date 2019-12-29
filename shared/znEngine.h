#pragma once

#include "../znEngine/Application.h"
#include "../znEngine/WindowCreator.h"

#include "../znEngine/Materials/MaterialDebug.h"
#include "../znEngine/Materials/MaterialTextured.h"

#include "../znEngine/Models/AssimpLoader.h"

#include "../znEngine/SceneFunctional/UI/Fonts/FontsManager.h"

/*
 * Initialize improtant managers and plugins
 */
ZN_API IBaseManager* WINAPI InitializeEngine(std::vector<std::string> Arguments);