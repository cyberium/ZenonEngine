#pragma once

#include "../znEngine/Application.h"

#include "../znEngine/WindowClassRegistrator.h"
#include "../znEngine/WindowCreator.h"

#include "../znEngine/SceneFunctional/UI/Fonts/FontsManager.h"

/*
 * Initialize improtant managers and plugins
 */
OW_ENGINE_API IBaseManager* WINAPI InitializeEngine(std::vector<std::string> Arguments);