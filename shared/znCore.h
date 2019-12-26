#pragma once

#include "../znCore/common.h"

// Ref manager Sync
#include "../znCore/RefManager1Dim.h"
#include "../znCore/RefManager1DimAssync.h"

// Files
#include "../znCore/LocalFilesStorage.h"
#include "../znCore/LibraryResourceFileStotage.h"
#include "../znCore/FilesManager.h"
#include "../znCore/File.h"
#include "../znCore/ByteBufferOnlyPointer.h"

// Log outputs
#include "../znCore/Console.h"



#include "../znCore/Passes/RenderTechnique.h"
#include "../znCore/Passes/AbstractPass.h"
#include "../znCore/Passes/Base3DPass.h"
#include "../znCore/Passes/BaseUIPass.h"


// Camera
#include "../znCore/CameraControllers/Camera.h"
#include "../znCore/CameraControllers/CameraControllerBase.h"
#include "../znCore/CameraControllers/ArcBallCameraController.h"
#include "../znCore/CameraControllers/FreeCameraController.h"
#include "../znCore/CameraControllers/RTSCameraController.h"





#include "../znCore/Scene/LoadableObject.h"

#include "../znCore/Scene/Light3D.h"

#include "../znCore/GameStateBase.h"

// Base
#include "../znCore/Scene/SceneBase.h"
#include "../znCore/Scene/SceneNodeBase.h"
#include "../znCore/Scene/ComponentBase.h"
#include "../znCore/Scene/Actions.h"
#include "../znCore/Scene/Properties.h"

// 3D
#include "../znCore/Scene/3D/SceneNode3D.h"
#include "../znCore/Scene/3D/ColliderComponent3D.h"
#include "../znCore/Scene/3D/LightComponent3D.h"
#include "../znCore/Scene/3D/MeshComponent3D.h"

// UI
#include "../znCore/Scene/UI/SceneNodeUI.h"

#include "../znCore/XML/XMLManager.h"
