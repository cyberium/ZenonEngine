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

#include "../znCore/DebugOutput.h"

// Log outputs
#include "../znCore/Console.h"



#include "../znCore/Passes/RenderTechnique.h"
#include "../znCore/Passes/AbstractPass.h"
#include "../znCore/Passes/Base3DPass.h"
#include "../znCore/Passes/BaseUIPass.h"



#include "../znCore/CameraControllers/Camera.h"
#include "../znCore/CameraControllers/CameraControllerBase.h"
#include "../znCore/CameraControllers/ArcBallCameraController.h"
#include "../znCore/CameraControllers/FreeCameraController.h"
#include "../znCore/CameraControllers/RTSCameraController.h"





#include "../znCore/Scene/LoadableObject.h"

#include "../znCore/Scene/Light3D.h"

#include "../znCore/GameState.h"

#include "../znCore/Scene/ComponentBase.h"
#include "../znCore/Scene/TransformComponentBase.h"

#include "../znCore/Scene/3D/ColliderComponent3D.h"
#include "../znCore/Scene/3D/LightComponent3D.h"
#include "../znCore/Scene/3D/MeshComponent3D.h"
#include "../znCore/Scene/3D/TransformComponent3D.h"

#include "../znCore/Scene/UI/TransformComponentUI.h"
