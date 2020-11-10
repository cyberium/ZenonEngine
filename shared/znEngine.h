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
#include "../znEngine/Scene/Components/Animation/AnimationComponent.h"
#include "../znEngine/Scene/Components/Skeleton/SkeletonComponent.h"
#include "../znEngine/Scene/Components/ColliderComponent3D.h"
#include "../znEngine/Scene/Components/LightComponent3D.h"
#include "../znEngine/Scene/Components/ModelsComponent3D.h"
#include "../znEngine/Scene/Components/CameraComponent3D.h"
#include "../znEngine/Scene/Components/Particles/ParticlesComponent.h"


// SceneNodes
#include "../znEngine/Scene/EngineSceneIntfs.h"
#include "../znEngine/Scene/SceneBase.h"
#include "../znEngine/Scene/Nodes/UITexture.h"
#include "../znEngine/Scene/Nodes/UIColor.h"
#include "../znEngine/Scene/Nodes/UIText.h"


// Materials
#include "../znEngine/Materials/MaterialDebug.h"
#include "../znEngine/Materials/MaterialTextured.h"
#include "../znEngine/Materials/MaterialModel.h"


// Passes
#include "../znEngine/Passes/UI/UIFontPass.h"
#include "../znEngine/Passes/UI/UIButtonPass.h"
#include "../znEngine/Passes/UI/UIColorPass.h"
#include "../znEngine/Passes/UI/UITexturePass.h"



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
#include "../znEngine/Passes/MaterialParticlePass.h"

#include "../znEngine/Formats/Fonts/FontsManager.h"
#include "../znEngine/Formats/Images/DDSFormat.h"
#include "../znEngine/Formats/Images/ImageLoaderTemplate.h"

/*
 * Initialize improtant managers and plugins
 */
ZN_API IBaseManager* WINAPI InitializeEngine(std::vector<std::string> Arguments, std::string PathToPlugins);
