#pragma once

//---------------------------------------------------------//
//--                   Base Types                        --//
//---------------------------------------------------------//
#include "MathTypes.h"
#include "BoundingBox.h"
#include "BoundingRect.h"
#include "BoundingSphere.h"
#include "Frustrum.h"
#include "Plane.h"
#include "Rect.h"
#include "Interfaces/Types/KeyCodes.h"
#include "Interfaces/Types/Object.h"
#include "Interfaces/Types/Events.h"


//---------------------------------------------------------//
//--                Base Interfaces                      --//
//---------------------------------------------------------//
#include "Interfaces/BaseManager.h"

#include "Interfaces/Application.h"
#include "Interfaces/CameraController.h"
#include "Interfaces/Console.h"
#include "Interfaces/DebugOutput.h"
#include "Interfaces/Files.h"
#include "Interfaces/GameState.h"
#include "Interfaces/GameStateManager.h"
#include "Interfaces/Loader.h"
#include "Interfaces/Log.h"
#include "Interfaces/Networking.h"
#include "Interfaces/Plugins.h"
#include "Interfaces/Settings.h"
#include "Interfaces/Visitor.h"
#include "Interfaces/WindowObject.h"


//---------------------------------------------------------//
//--                   Render Types                      --//
//---------------------------------------------------------//
#include "Interfaces/Render/Types/BufferBinding.h"
#include "Interfaces/Render/Types/ClearFlags.h"
#include "Interfaces/Render/Types/CPUAccess.h"
#include "Interfaces/Render/Types/InputSemantic.h"
#include "Interfaces/Render/Types/PrimitiveTopology.h"
#include "Interfaces/Render/Types/Viewport.h"
#include "Interfaces/Render/Types/CustomVertexElement.h"
#include "Interfaces/Render/Types/Ray.h"
#include "Interfaces/Render/Types/RaycastHit.h"

//---------------------------------------------------------//
//--                Render Interfaces                    --//
//---------------------------------------------------------//
#include "Interfaces/Render/IRender.h"

#include "Interfaces/Render/IShader.h"
#include "Interfaces/Render/IShaderParameter.h"
#include "Interfaces/Render/IShaderInputLayout.h"
#include "Interfaces/Render/ITexture.h"
#include "Interfaces/Render/ISamplerState.h"
#include "Interfaces/Render/IBuffer.h"
#include "Interfaces/Render/IQuery.h"
#include "Interfaces/Render/IMaterial.h"
#include "Interfaces/Render/IMesh.h"
#include "Interfaces/Render/IRenderTarget.h"
#include "Interfaces/Render/IRenderWindow.h"
#include "Interfaces/Render/IBlendState.h"
#include "Interfaces/Render/IDepthStencilState.h"
#include "Interfaces/Render/IRasterizerState.h"
#include "Interfaces/Render/IPipelineState.h"
#include "Interfaces/Render/IRenderDevice.h"
#include "Interfaces/Render/ICamera.h"
#include "Interfaces/Render/IRenderPass.h"



//---------------------------------------------------------//
//--                    Scene Types                      --//
//---------------------------------------------------------//


//---------------------------------------------------------//
//--                 Scene Interfaces                    --//
//---------------------------------------------------------//
#include "Interfaces/Scene/ISceneNodeComponents.h"
#include "Interfaces/Scene/ISceneNode.h"
#include "Interfaces/Scene/IScene.h"
#include "Interfaces/Scene/ISceneNodeProvider.h"
#include "Interfaces/Scene/ISceneCreator.h"