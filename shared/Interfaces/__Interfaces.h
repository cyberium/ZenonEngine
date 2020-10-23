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

typedef glm::vec3 ColorRBG;

#include <guiddef.h>

#ifndef WIN32_LEAN_AND_MEAN
#   define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#   define NOMINMAX
#endif

#ifndef VC_EXTRALEAN
#	define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif


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

//#ifdef _DEBUG
//	#define _CRTDBG_MAP_ALLOC
//	#include <crtdbg.h>	

	//template <typename T, typename... Args>
	//inline T* ZNAllocate(int _BlockUse, char const* _FileName, int _LineNumber, Args &&... _Args)
	//{
	//	if (void* ptr = _malloc_dbg(sizeof(T), _BlockUse, _FileName, _LineNumber))
	//	{
	//		*((T*)ptr) = T(std::forward<Args>(_Args)...);
	//		return (T*)ptr;
	//	}
	//
	//	return nullptr;
	//}

	/*inline void* operator new(size_t sz, int _BlockUse, char const* _FileName, int _LineNumber)
	{
		if (void *ptr = _malloc_dbg(sz, _BlockUse, _FileName, _LineNumber))
			return ptr;
		return nullptr;
	}

	inline void operator delete(void* ptr, int _BlockUse) noexcept
	{
		_free_dbg(ptr, _BlockUse);
	}*/

/*
	template <class T>
	struct custom_allocator 
	{
		typedef T value_type;
		int BlockUse;
		char const* FileName;
		int LineNumber;

		custom_allocator(int _BlockUse, char const* _FileName, int _LineNumber) noexcept
			: BlockUse(_BlockUse)
			, FileName(_FileName)
			, LineNumber(_LineNumber)
		{}

		template <class U> 
		custom_allocator(const custom_allocator<U>& Other) noexcept 
		{
			BlockUse = Other.BlockUse;
			FileName = Other.FileName;
			LineNumber = Other.LineNumber;
			//std::cout << "custom_allocator<T>::custom_allocator(const custom_allocator<U>&): "
			//	<< "\n\tT: " << get_class_name<T>()
			//	<< "\n\tU: " << get_class_name<U>() << std::endl;
		}

		T* allocate(std::size_t n) 
		{
			//std::cout << "Allocating: " << get_class_name<T>() << std::endl;
			return (T*)(_malloc_dbg(n, BlockUse, FileName, LineNumber)); //reinterpret_cast<T*>(::operator new(n * sizeof(T)));
		}
		void deallocate(T* p, std::size_t n) 
		{
			_free_dbg(p, BlockUse);
			//std::cout << "Deallocating: " << get_class_name<T>() << std::endl;
			//::operator delete(p);
		}
	};

	//#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	//#define new DEBUG_NEW

	//#define MakeShared(CLASS, ...) std::shared_ptr<CLASS>(new CLASS(__VA_ARGS__), [=](CLASS* ___Object) { delete ___Object; })
	//#define MakeShared(CLASS, ...) std::allocate_shared<CLASS>(custom_allocator<CLASS>(_NORMAL_BLOCK, __FILE__, __LINE__), __VA_ARGS__)

	#define MakeShared(CLASS, ...) std::shared_ptr<CLASS>(std::default_delete<CLASS>(), custom_allocator<CLASS>()

	//#undef new
	*/
//#else
	#define MakeShared(CLASS, ...) std::make_shared<CLASS>(__VA_ARGS__)
//#endif

//---------------------------------------------------------//
//--                   Base Types                        --//
//---------------------------------------------------------//
class BoundingBox;
typedef const BoundingBox& cbbox;
class BoundingRect;
typedef const BoundingRect& cbrect;
class BoundingSphere;
class Frustum;
class Plane;
class Rect;

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
#include "Interfaces/FontsManager.h"
#include "Interfaces/Loader.h"
#include "Interfaces/Log.h"
#include "Interfaces/Networking.h"
#include "Interfaces/Types/Object.h"
#include "Interfaces/Types/ObjectsFactory.h"
#include "Interfaces/Plugins.h"
#include "Interfaces/SettingsManager.h"
#include "Interfaces/Visitor.h"
#include "Interfaces/WindowObject.h"
#include "Interfaces/XML.h"


//---------------------------------------------------------//
//--                   Render Types                      --//
//---------------------------------------------------------//
#include "Interfaces/Render/Types/RenderTypes.h"
#include "Interfaces/Render/Types/CustomVertexElement.h"
#include "Interfaces/Render/Types/InputSemantic.h"
#include "Interfaces/Render/Types/Ray.h"
#include "Interfaces/Render/Types/RaycastHit.h"
#include "Interfaces/Render/Types/Viewport.h"


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
#include "Interfaces/Render/IImageLoader.h"


//---------------------------------------------------------//
//--                    Scene Types                      --//
//---------------------------------------------------------//
#include "Interfaces/Scene/Types/Light.h"


//---------------------------------------------------------//
//--                 Scene Interfaces                    --//
//---------------------------------------------------------//
#include "Interfaces/Scene/IActions.h"
//#include "Interfaces/Scene/IProperties.h"
#include "Interfaces/Scene/ISceneNodeComponents.h"
#include "Interfaces/Scene/ISceneNode3D.h"
#include "Interfaces/Scene/ISceneNodeUI.h"
#include "Interfaces/Scene/IScene.h"
#include "Interfaces/Scene/ISceneNodeTools.h"
