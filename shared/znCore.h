#pragma once

#include "../znCore/common.h"

// Files
#include "../znCore/Files/ByteBuffer.h"
#include "../znCore/Files/ByteBufferOnlyPointer.h"
#include "../znCore/Files/ChunkedFile.h"
#include "../znCore/Files/LocalFilesStorage.h"
#include "../znCore/Files/LibraryResourceFileStotage.h"
#include "../znCore/Files/FilesManager.h"

// Log outputs
#include "../znCore/Log/Console/Console.h"

// Passes
#include "../znCore/Passes/RenderTechnique.h"
#include "../znCore/Passes/RenderPass.h"
#include "../znCore/Passes/RenderPassPipelined.h"
#include "../znCore/Passes/ScenePasses/ScenePass.h"
#include "../znCore/Passes/ScenePasses/Pipelined/ScenePassPipelined.h"
#include "../znCore/Passes/ScenePasses/Pipelined/Base3DPass.h"
#include "../znCore/Passes/ScenePasses/Pipelined/BaseUIPass.h"

#include "../znCore/ObjectsFactories/ObjectsFactory.h"
#include "../znCore/ObjectsFactories/ObjectClassFactory.h"
#include "../znCore/ObjectsFactories/ObjectClassCreator.h"
#include "../znCore/ObjectsFactories/SceneNode3DFactory.h"
#include "../znCore/ObjectsFactories/SceneNodeUIFactory.h"
#include "../znCore/ObjectsFactories/ComponentsFactory.h"

// Scene
#include "../znCore/Scene/ComponentBase.h"
#include "../znCore/Scene/SceneFinder.h"
#include "../znCore/Scene/SceneNode3D.h"
#include "../znCore/Scene/SceneNodeUI.h"

#include "../znCore/XML/XMLManager.h"
