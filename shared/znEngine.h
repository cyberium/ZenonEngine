#pragma once

#include "../znEngine/common.h"

#include "../znEngine/Application.h"
#include "../znEngine/ApplicationConsole.h"
#include "../znEngine/ApplicationNative.h"

// Loader
#include "../znEngine/AssyncLoader/LoadableObject.h"


// Camera controllers
#include "../znEngine/Scene/Camera/CameraControllerBase.h"
#include "../znEngine/Scene/Camera/ArcBallCameraController.h"
#include "../znEngine/Scene/Camera/FreeCameraController.h"
#include "../znEngine/Scene/Camera/RTSCameraController.h"


// Scene nodes components
#include "../znEngine/Scene/Components/ColliderComponent.h"

#include "../znEngine/Scene/Components/CameraComponent.h"

#include "../znEngine/Scene/Components/LightComponent/LightComponent.h"
#include "../znEngine/Scene/Components/LightComponent/Light.h"

#include "../znEngine/Scene/Components/Particles/ParticlesComponent.h"
#include "../znEngine/Scene/Components/Particles/ParticleSystem.h"

#include "../znEngine/Scene/Components/Animations/AnimatedValueSingle.h"
#include "../znEngine/Scene/Components/Animations/Animation.h"

#include "../znEngine/Scene/Components/ModelComponent/ModelComponent.h"
#include "../znEngine/Scene/Components/ModelComponent/Skeleton.h"
#include "../znEngine/Scene/Components/ModelComponent/SkeletonAnimation.h"
#include "../znEngine/Scene/Components/ModelComponent/SkeletonBone.h"


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
#include "../znEngine/UIControls/Common/UIControlCommonMaterial.h"

// Passes
#include "../znEngine/Passes/UI/UIFontPass.h"
#include "../znEngine/Passes/UI/UIControlPass.h"



#include "../znEngine/Passes/ForwardRendering/RendererForward.h"
#include "../znEngine/Passes/DefferedRendering/RendererDeffered.h"

#include "../znEngine/Passes/Scene/SceneCreateTypedListsPass.h"
#include "../znEngine/Passes/Scene/SceneCreateTypelessListPass.h"
#include "../znEngine/Passes/Scene/BaseTypedList3DPass.h"

// Technical passes
#include "../znEngine/Passes/Technical/BeginQueryPass.h"
#include "../znEngine/Passes/Technical/ClearRenderTargetPass.h"
#include "../znEngine/Passes/Technical/CopyBufferPass.h"
#include "../znEngine/Passes/Technical/SetShaderParameterPass.h"
#include "../znEngine/Passes/Technical/CopyTexturePass.h"
#include "../znEngine/Passes/Technical/DispatchPass.h"
#include "../znEngine/Passes/Technical/EndQueryPass.h"
#include "../znEngine/Passes/Technical/GenerateMipMapsPass.h"
#include "../znEngine/Passes/Technical/InvokeFunctionPass.h"

// Debug passes
#include "../znEngine/Passes/Debug/DebugPass.h"
#include "../znEngine/Passes/Debug/DrawBonesPass.h"
#include "../znEngine/Passes/Debug/DrawBoundingBoxPass.h"
#include "../znEngine/Passes/Debug/DrawLightFrustumPass.h"
#include "../znEngine/Passes/Debug/DrawNormals.h"

#include "../znEngine/Formats/Fonts/FontsManager.h"
#include "../znEngine/Formats/Images/DDSFormat.h"
#include "../znEngine/Formats/Images/ImageLoaderTemplate.h"
