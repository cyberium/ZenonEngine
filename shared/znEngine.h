#pragma once

#include "../znEngine/common.h"

#include "../znEngine/Application.h"
#include "../znEngine/NativeWindowFactory.h"
#include "../znEngine/NativeWindow_WindowsSpecific.h"


// Camera controllers
#include "../znEngine/SceneFunctional/CameraControllers/CameraControllerBase.h"
#include "../znEngine/SceneFunctional/CameraControllers/ArcBallCameraController.h"
#include "../znEngine/SceneFunctional/CameraControllers/FreeCameraController.h"
#include "../znEngine/SceneFunctional/CameraControllers/RTSCameraController.h"

// Nodes
#include "../znEngine/SceneFunctional/UI/UIText.h"
#include "../znEngine/SceneFunctional/UI/UIButton.h"
#include "../znEngine/SceneFunctional/UI/UITexture.h"

// Materials
#include "../znEngine/Materials/MaterialDebug.h"
#include "../znEngine/Materials/MaterialTextured.h"
#include "../znEngine/Materials/MaterialModel.h"
#include "../znEngine/Materials/MaterialParticle.h"

// Passes
#include "../znEngine/Passes/UIFontPass.h"
#include "../znEngine/Passes/UIButtonPass.h"
#include "../znEngine/Passes/UIColorPass.h"
#include "../znEngine/Passes/UITexturePass.h"


#include "../znEngine/Passes/Pipelined/BaseTypedList3DPass.h"
#include "../znEngine/Passes/Scene/SceneCreateTypedListsPass.h"
#include "../znEngine/Passes/Scene/SceneCreateTypelessListPass.h"

#include "../znEngine/Passes/Technical/BeginQueryPass.h"
#include "../znEngine/Passes/Technical/ClearRenderTargetPass.h"
#include "../znEngine/Passes/Technical/CopyBufferPass.h"
#include "../znEngine/Passes/Technical/SetShaderParameterPass.h"
#include "../znEngine/Passes/Technical/CopyTexturePass.h"
#include "../znEngine/Passes/Technical/DispatchPass.h"
#include "../znEngine/Passes/Technical/EndQueryPass.h"
#include "../znEngine/Passes/Technical/GenerateMipMapsPass.h"
#include "../znEngine/Passes/Technical/InvokeFunctionPass.h"

#include "../znEngine/Passes/DrawBoundingBoxPass.h"
#include "../znEngine/Passes/MaterialParticlePass.h"

#include "../znEngine/Formats/Fonts/FontsManager.h"
#include "../znEngine/Formats/Images/DDSFormat.h"
#include "../znEngine/Formats/Images/ImageLoaderTemplate.h"

/*
 * Initialize improtant managers and plugins
 */
ZN_API IBaseManager* WINAPI InitializeEngine(std::vector<std::string> Arguments, std::string PathToPlugins);
