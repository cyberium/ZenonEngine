#pragma once

//---------------------------------------------------------//
//--                    Common                           --//
//---------------------------------------------------------//
#include "../znEngine/common.h"


// Base
#include "../znEngine/Application.h"
#include "../znEngine/CameraControllerBase.h"
#include "../znEngine/ArcBallCameraController.h"
#include "../znEngine/FreeCameraController.h"
#include "../znEngine/RTSCameraController.h"
#include "../znEngine/CreatePasses.h"
#include "../znEngine/EngineTime.h"
#include "../znEngine/Loader.h"
#include "../znEngine/UI/Fonts/FontsManager.h"
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
#include "../znEngine/UI/UIScene.h"
#include "../znEngine/UI/UIBaseNode.h"

// common nodes
#include "../znEngine/UI/UIButton.h"
#include "../znEngine/UI/UIColor.h"
#include "../znEngine/UI/UIBeizerLine.h"
#include "../znEngine/UI/UIText.h"
#include "../znEngine/UI/UITexture.h"

// additional nodes
#include "../znEngine/UI/UIContainerGarmoshka.h"

// materials
#include "../znEngine/UI/Materials/UI_Color_Material.h"
#include "../znEngine/UI/Materials/UI_Texture_Material.h"

// base passes
#include "../znEngine/UI/Passes/BaseUIPass.h"
#include "../znEngine/UI/Passes/RenderUITechnique.h"



//
// 3D
//

// base nodes
#include "../znEngine/3D/Scene3D.h"
#include "../znEngine/3D/SceneNode3D.h"
#include "../znEngine/3D/SceneNodeModel3D.h"
#include "../znEngine/3D/SceneNodeFont3D.h"

// additional nodes

// materials
#include "../znEngine/3D/Materials/MaterialDebug.h"
#include "../znEngine/3D/Materials/MaterialTextured.h"

// base passes
#include "../znEngine/3D/Passes/BasePass.h"
#include "../znEngine/3D/Passes/RenderTechnique.h"

// other passes
#include "../znEngine/3D/Passes/ClearRenderTargetPass.h"
#include "../znEngine/3D/Passes/CopyBufferPass.h"
#include "../znEngine/3D/Passes/CopyTexturePass.h"
#include "../znEngine/3D/Passes/GenerateMipMapsPass.h"
#include "../znEngine/3D/Passes/PostprocessPass.h"
#include "../znEngine/3D/Passes/BeginQueryPass.h"
#include "../znEngine/3D/Passes/EndQueryPass.h"
#include "../znEngine/3D/Passes/DispatchPass.h"
#include "../znEngine/3D/Passes/InvokeFunctionPass.h"

// Postprocess
#include "../znEngine/GBuffer.h"
#include "../znEngine/3D/Light.h"
#include "../znEngine/3D/Light3D.h"
#include "../znEngine/3D/Passes/DeferredLightingPass.h"