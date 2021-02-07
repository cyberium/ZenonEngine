#pragma once

//---------------------------------------------------------//
//--                       BASIC                         --//
//---------------------------------------------------------//

#ifdef OW_ENGINE_EXPORTS
	#define ZN_API __declspec(dllexport) 
#else
	#define ZN_API __declspec(dllimport) 
#endif

#define ZN_INTERFACE struct __declspec(novtable)

#define ZN_GET_OBJECTCLASS(x) (UINT16_MAX - x##u)
#define ZN_MAKE_OBJECTCLASS(x, y) const ObjectClass x = UINT16_MAX - y##u;
#define ZN_OBJECTCLASS(x) static ObjectClass GetClassT() { return x; }

#define _CRT_SECURE_NO_WARNINGS


// General types
typedef signed char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
typedef long long int64;
typedef unsigned long long uint64;

// GLM
#pragma warning( push )
#pragma warning( disable : 4201) // warning C4201: nonstandard extension used : nameless struct/union
#define GLM_FORCE_SWIZZLE
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/constants.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtc/quaternion.hpp>
#include <glm/glm/gtc/random.hpp>
#include <glm/glm/gtx/vector_angle.hpp>
#include <glm/glm/gtx/compatibility.hpp>
#include <glm/glm/gtx/matrix_operation.hpp>
#include <glm/glm/gtx/matrix_decompose.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtx/quaternion.hpp>
#include <glm/glm/gtx/euler_angles.hpp>
#include <glm/glm/gtx/norm.hpp>
#include <glm/glm/gtx/vector_angle.hpp>
#pragma warning( pop ) 

// STL Types
#include <map>
#include <set>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <unordered_map>
#include <mutex>
#include <algorithm>
#include <codecvt>
#include <functional>
#include <fstream>
#include <cmath>
#include <memory>
#include <atomic>
#include <future>
#include <chrono>

// Usefull macros
#define __PACK_BEGIN  "../shared/pack_begin.h"
#define __PACK_END  "../shared/pack_end.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>	

	#define ZN_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define MakeShared(CLASS, ...) std::shared_ptr<CLASS>(ZN_NEW CLASS(__VA_ARGS__))

#else
	#define ZN_NEW new
	#define MakeShared(CLASS, ...) std::make_shared<CLASS>(__VA_ARGS__)
#endif

//---------------------------------------------------------//
//--                   Base Types                        --//
//---------------------------------------------------------//
class BoundingBox;
class BoundingRect;
class BoundingSphere;
class Frustum;
class Plane;
class Rect;

#include "Interfaces/Types/Color.h"
#include "Interfaces/Types/KeyCodes.h"
#include "Interfaces/Types/Object.h"
#include "Interfaces/Types/Events.h"
#include "Interfaces/Types/ObjectClasses.h"


//---------------------------------------------------------//
//--                   Render Types                      --//
//---------------------------------------------------------//
#include "Interfaces/Render/Types/RenderTypes.h"
#include "Interfaces/Render/Types/CustomVertexElement.h"
#include "Interfaces/Render/Types/Ray.h"
#include "Interfaces/Render/Types/RaycastHit.h"
#include "Interfaces/Render/Types/Viewport.h"

//---------------------------------------------------------//
//--                    Scene Types                      --//
//---------------------------------------------------------//
#include "Interfaces/Scene/Types/Light.h"




//---------------------------------------------------------//
//--                Base Interfaces                      --//
//---------------------------------------------------------//
#include "Interfaces/BaseManager.h"
#include "Interfaces/Application.h"
#include "Interfaces/CameraController.h"
#include "Interfaces/Console.h"
#include "Interfaces/Files.h"
#include "Interfaces/Loader.h"
#include "Interfaces/Log.h"
#include "Interfaces/Networking.h"
#include "Interfaces/Types/Object.h"
#include "Interfaces/Types/ObjectsFactory.h"
#include "Interfaces/Plugins.h"
#include "Interfaces/Visitor.h"
#include "Interfaces/WindowObject.h"
#include "Interfaces/XML.h"

#include "Interfaces/Managers/FontsManager.h"
#include "Interfaces/Managers/ModelsFactory.h"
#include "Interfaces/Managers/ImagesFactory.h"
#include "Interfaces/Managers/TexturesFactory.h"
#include "Interfaces/Managers/SettingsManager.h"


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
#include "Interfaces/Render/IGeometry.h"
#include "Interfaces/Render/IModel.h"
#include "Interfaces/Render/IRenderTarget.h"
#include "Interfaces/Render/IRenderWindow.h"
#include "Interfaces/Render/IBlendState.h"
#include "Interfaces/Render/IDepthStencilState.h"
#include "Interfaces/Render/IRasterizerState.h"
#include "Interfaces/Render/IPipelineState.h"
#include "Interfaces/Render/IRenderDevice.h"
#include "Interfaces/Render/ICamera.h"
#include "Interfaces/Render/IRenderPass.h"
#include "Interfaces/Render/IRenderTechnique.h"
#include "Interfaces/Render/ISkeleton.h"


//---------------------------------------------------------//
//--                 Scene Interfaces                    --//
//---------------------------------------------------------//
#include "Interfaces/Scene/IActions.h"
//#include "Interfaces/Scene/IProperties.h"
#include "Interfaces/Scene/ISceneNodeComponents.h"
#include "Interfaces/Scene/ISceneNodeCameraComponent.h"
#include "Interfaces/Scene/ISceneNodeColliderComponent.h"
#include "Interfaces/Scene/ISceneNodeLightComponent.h"
#include "Interfaces/Scene/ISceneNodeModelComponent.h"
#include "Interfaces/Scene/ISceneNodeParticlesComponent.h"
#include "Interfaces/Scene/ISceneNodePortalComponent.h"
#include "Interfaces/Scene/ISceneNode.h"
#include "Interfaces/Scene/IUIControl.h"
#include "Interfaces/Scene/IScene.h"
#include "Interfaces/Scene/ISceneNodeTools.h"



//---------------------------------------------------------//
//--                Platform Interfaces                  --//
//---------------------------------------------------------//
#include "Interfaces/Platform/PlatformWindows.h"
