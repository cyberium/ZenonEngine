#pragma once

#include "../znEngine/common.h"

#include "../znEngine/Application.h"
#include "../znEngine/NativeWindows/NativeWindowFactory.h"


// Loader
#include "../znEngine/AssyncLoader/LoadableObject.h"


// Camera controllers
#include "../znEngine/Scene/Camera/CameraControllerBase.h"
#include "../znEngine/Scene/Camera/ArcBallCameraController.h"
#include "../znEngine/Scene/Camera/FreeCameraController.h"
#include "../znEngine/Scene/Camera/RTSCameraController.h"


// Scene nodes components
#include "../znEngine/Scene/Components/ModelComponent/AnimatedValue.h"
#include "../znEngine/Scene/Components/ModelComponent/Animation.h"
#include "../znEngine/Scene/Components/ModelComponent/SkeletonComponentBone.h"
#include "../znEngine/Scene/Components/ColliderComponent.h"
#include "../znEngine/Scene/Components/LightComponent.h"
#include "../znEngine/Scene/Components/ModelComponent/ModelComponent.h"
#include "../znEngine/Scene/Components/CameraComponent.h"
#include "../znEngine/Scene/Components/Particles/ParticlesComponent.h"


// SceneNodes
#include "../znEngine/EngineIntfs.h"
#include "../znEngine/Scene/SceneBase.h"


// UIControls
#include "../znEngine/UIControls/UIControlCommon.h"
#include "../znEngine/UIControls/UIControlText.h"

// UIControls: RTS
#include "../znEngine/RTS/UIControls/UIControlRTSResourcesPanel.h"
#include "../znEngine/RTS/UIControls/UIControlRTSTowersPanel.h"
#include "../znEngine/RTS/UIControls/UIControlRTSTowerBtn.h"



// Materials
#include "../znEngine/Materials/MaterialDebug.h"
#include "../znEngine/Materials/MaterialModel.h"


// Passes
#include "../znEngine/Passes/UI/UIFontPass.h"
#include "../znEngine/Passes/UI/UIControlPass.h"



#include "../znEngine/Passes/ForwardRendering/RendererForward.h"
#include "../znEngine/Passes/DefferedRendering/RendererDeffered.h"

#include "../znEngine/Passes/Scene/SceneCreateTypedListsPass.h"
#include "../znEngine/Passes/Scene/SceneCreateTypelessListPass.h"
#include "../znEngine/Passes/Scene/BaseTypedList3DPass.h"

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
#include "../znEngine/Passes/ParticlesPass.h"

#include "../znEngine/Formats/Fonts/FontsManager.h"
#include "../znEngine/Formats/Images/DDSFormat.h"
#include "../znEngine/Formats/Images/ImageLoaderTemplate.h"
