#pragma once

#include "../znCore/common.h"


// Files
#include "../znCore/Files/ByteBuffer.h"
#include "../znCore/Files/ByteBufferOnlyPointer.h"
#include "../znCore/Files/File.h"
#include "../znCore/Files/ChunkedFile.h"
#include "../znCore/Files/LocalFilesStorage.h"
#include "../znCore/Files/LibraryResourceFileStotage.h"
#include "../znCore/Files/FilesManager.h"


// Log outputs
#include "../znCore/Console.h"


// Passes
#include "../znCore/Passes/RenderTechnique.h"
#include "../znCore/Passes/RenderPass.h"
#include "../znCore/Passes/RenderPassPipelined.h"

#include "../znCore/Passes/ScenePasses/ScenePass.h"
#include "../znCore/Passes/ScenePasses/SceneCreateTypedListsPass.h"
#include "../znCore/Passes/ScenePasses/SceneCreateTypelessListPass.h"

#include "../znCore/Passes/ScenePasses/Pipelined/ScenePassPipelined.h"
#include "../znCore/Passes/ScenePasses/Pipelined/Base3DPass.h"
#include "../znCore/Passes/ScenePasses/Pipelined/BaseTypedList3DPass.h"
#include "../znCore/Passes/ScenePasses/Pipelined/BaseUIPass.h"

#include "../znCore/Passes/Technical/BeginQueryPass.h"
#include "../znCore/Passes/Technical/ClearRenderTargetPass.h"
#include "../znCore/Passes/Technical/CopyBufferPass.h"
#include "../znCore/Passes/Technical/CopyTexturePass.h"
#include "../znCore/Passes/Technical/DispatchPass.h"
#include "../znCore/Passes/Technical/EndQueryPass.h"
#include "../znCore/Passes/Technical/GenerateMipMapsPass.h"
#include "../znCore/Passes/Technical/InvokeFunctionPass.h"
#include "../znCore/Passes/Technical/SetShaderParameterPass.h"

// Camera controllers
#include "../znCore/CameraControllers/CameraControllerBase.h"
#include "../znCore/CameraControllers/ArcBallCameraController.h"
#include "../znCore/CameraControllers/FreeCameraController.h"
#include "../znCore/CameraControllers/RTSCameraController.h"


#include "../znCore/Scene/LoadableObject.h"


#include "../znCore/ObjectsFactories/ObjectsFactory.h"
#include "../znCore/ObjectsFactories/ObjectClassFactory.h"
#include "../znCore/ObjectsFactories/ObjectClassCreator.h"
#include "../znCore/ObjectsFactories/SceneNode3DFactory.h"

// Base
#include "../znCore/Scene/SceneBase.h"
#include "../znCore/Scene/ComponentBase.h"
#include "../znCore/Scene/Actions.h"
#include "../znCore/Scene/Properties.h"

// 3D
#include "../znCore/Scene/3D/SceneNode3D.h"
#include "../znCore/Scene/3D/ColliderComponent3D.h"
#include "../znCore/Scene/3D/LightComponent3D.h"
#include "../znCore/Scene/3D/ModelsComponent3D.h"
#include "../znCore/Scene/3D/CameraComponent3D.h"
#include "../znCore/Scene/3D/ParticlesComponent3D.h"

// UI
#include "../znCore/Scene/UI/SceneNodeUI.h"


#include "../znCore/XML/XMLManager.h"
