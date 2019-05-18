#pragma once

//---------------------------------------------------------//
//--                    Common                           --//
//---------------------------------------------------------//
#include "../znEngine/common.h"


// Base
#include "../znEngine/Application.h"
#include "../znEngine/CameraControllers/CameraControllerBase.h"
#include "../znEngine/CameraControllers/ArcBallCameraController.h"
#include "../znEngine/CameraControllers/FreeCameraController.h"
#include "../znEngine/CameraControllers/RTSCameraController.h"
#include "../znEngine/CreatePasses.h"
#include "../znEngine/EngineTime.h"
#include "../znEngine/Loader.h"
#include "../znEngine/SceneFunctional/UI/Fonts/FontsManager.h"
#include "../znEngine/WindowCreator.h"

//
// Wrappers
//
#include "../znEngine/MaterialWrapper.h"
#include "../znEngine/MeshWrapper.h"


//
// UI
//

// base nodes
#include "../znEngine/SceneFunctional/UI/SceneUI.h"
#include "../znEngine/SceneFunctional/UI/SceneNodeUI.h"

// common nodes
#include "../znEngine/SceneFunctional/UI/UIButton.h"
#include "../znEngine/SceneFunctional/UI/UIColor.h"
#include "../znEngine/SceneFunctional/UI/UIBeizerLine.h"
#include "../znEngine/SceneFunctional/UI/UIText.h"
#include "../znEngine/SceneFunctional/UI/UITexture.h"

// additional nodes
#include "../znEngine/SceneFunctional/UI/UIContainerGarmoshka.h"

// materials
#include "../znEngine/Materials/UI_Color_Material.h"
#include "../znEngine/Materials/UI_Texture_Material.h"

// base passes
#include "../znEngine/Passes/BaseUIPass.h"



//
// 3D
//

// base nodes
#include "../znEngine/SceneFunctional/3D/Scene3D.h"
#include "../znEngine/SceneFunctional/3D/SceneNode3D.h"

// additional nodes

// materials
#include "../znEngine/Materials/MaterialDebug.h"
#include "../znEngine/Materials/MaterialTextured.h"

// base passes
#include "../znEngine/Passes/BasePass.h"
#include "../znEngine/Passes/RenderTechnique.h"

// other passes
#include "../znEngine/Passes/ClearRenderTargetPass.h"
#include "../znEngine/Passes/CopyBufferPass.h"
#include "../znEngine/Passes/CopyTexturePass.h"
#include "../znEngine/Passes/GenerateMipMapsPass.h"
#include "../znEngine/Passes/PostprocessPass.h"
#include "../znEngine/Passes/BeginQueryPass.h"
#include "../znEngine/Passes/EndQueryPass.h"
#include "../znEngine/Passes/DispatchPass.h"
#include "../znEngine/Passes/InvokeFunctionPass.h"

// Postprocess
#include "../znEngine/Passes/GBuffer.h"
#include "../znEngine/SceneFunctional/Light.h"
#include "../znEngine/SceneFunctional/Light3D.h"
#include "../znEngine/Passes/DeferredLightingPass.h"