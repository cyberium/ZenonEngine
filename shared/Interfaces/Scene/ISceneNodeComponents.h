#pragma once

#include "Interfaces/Scene/Types/Light.h"

// FORWARD BEGIN
ZN_INTERFACE IPropertiesGroup;
ZN_INTERFACE IXMLReader;
ZN_INTERFACE IXMLWriter;
// FORWARD END

typedef uint32 ComponentMessageType;

ZN_INTERFACE ZN_API ISceneNodeComponent 
	: public std::enable_shared_from_this<ISceneNodeComponent>
{
	virtual ~ISceneNodeComponent() {}

    // Callbacks
    virtual void OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) = 0;
	virtual IPropertiesGroup* GetPropertiesGroup() const = 0;

	// Visit functional
	virtual void Update(const UpdateEventArgs& e) = 0;
    virtual void Accept(IVisitor* visitor) = 0;
};
typedef std::unordered_map<GUID, std::shared_ptr<ISceneNodeComponent>> ComponentsMap;

const ComponentMessageType UUID_OnParentChanged = 1;
const ComponentMessageType UUID_OnLocalTransformChanged = 2;
const ComponentMessageType UUID_OnWorldTransformChanged = 3;



//
// COLLIDER COMPONENT 3D
//
#define UUID_ColliderComponent uuid("78BD7168-51CB-4760-ADD2-218CF4E88CE2")
ZN_INTERFACE __declspec(UUID_ColliderComponent) ZN_API IColliderComponent3D
{
	enum class ZN_API ECullStrategy
	{
		None = 0,
		ByDistance,
		ByDistance2D,
		ByFrustrum,
		ByFrustrumAndDistance2D,
		ByFrustrumAndDistance
	};

	virtual ~IColliderComponent3D() {}

	virtual void SetCullStrategy(ECullStrategy CullStrategy) = 0;
	virtual ECullStrategy GetCullStrategy() const = 0;
	virtual void SetCullDistance(float Distance) = 0;
	virtual float GetCullDistance() const = 0;
	virtual void SetBounds(BoundingBox _bbox) = 0;
	virtual cbbox GetBounds() const = 0;
	virtual cbbox GetWorldBounds() const = 0;
	virtual void SetDebugDrawMode(bool Value) = 0;
	virtual bool GetDebugDrawMode() const = 0;

	virtual bool IsCulled(const ICameraComponent3D* Camera) const = 0;
	virtual bool IsCulledByFrustum(const ICameraComponent3D* Camera) const = 0;
	virtual bool IsCulledByDistance2D(const ICameraComponent3D* Camera) const = 0;
	virtual bool IsCulledByDistance(const ICameraComponent3D* Camera) const = 0;
};
const ComponentMessageType UUID_OnBoundsChanget = 10;



//
// PORTAL COMPONENT
//

ZN_INTERFACE IPortalRoom;

ZN_INTERFACE ZN_API IPortal
{
	enum class ZN_API Side
	{
		None,
		Inner,
		Outer
	};

	virtual ~IPortal() {}

	virtual Frustum CreatePolyFrustum(glm::vec3 Eye) const = 0;

	virtual bool IsVisible(const Frustum& Frustum) const = 0;
	virtual bool IsVisible(const std::vector<Plane>& Planes) const = 0;
	virtual bool IsPositive(cvec3 InvTranslateCamera) const = 0;

	virtual std::shared_ptr<IPortalRoom> GetRoomObject(glm::vec3 Eye) const = 0;
};


/**
  * В каждой комнате содержаться объекты.
  * Объект видим если соблюдены следующие условия:
  *   - комната видима
  *     а) комната видима через портал
  *     б) комната видима из-за того что игрок находится в ней
  *   - объект видим через портал
*/
ZN_INTERFACE ZN_API IPortalRoomObject
{
	virtual ~IPortalRoomObject() {}

	virtual BoundingBox GetBoundingBox() const = 0;
	virtual void SetVisibilityState(bool Value) = 0;
};


/**
  * Комната в которой содержаться объекты и которые отделены 
  * друг от друга и от внешнего мира порталами. 
  * Внутри комнаты могут быть и другие комнаты.
*/
ZN_INTERFACE ZN_API IPortalRoom
{
	virtual ~IPortalRoom() {}

	virtual void AddPortal(const std::shared_ptr<IPortal>& Portal) = 0;
	virtual const std::vector<std::shared_ptr<IPortal>>& GetPortals() const = 0;
	virtual void AddRoomObject(const std::weak_ptr<IPortalRoomObject>& RoomObject) = 0;
	virtual const std::vector<std::weak_ptr<IPortalRoomObject>>& GetRoomObjects() const = 0;

	virtual void Reset() = 0;
	virtual BoundingBox GetBoundingBox() const = 0;
	virtual void SetVisibilityState(bool Value) = 0;
	virtual void SetCalculatedState(bool Value) = 0;
	virtual bool IsCalculated() const = 0;
};

#define UUID_PortalsComponent uuid("C4B0195E-B6E4-458E-A371-C0698335A5A7")
ZN_INTERFACE __declspec(UUID_PortalsComponent) ZN_API IPortalsComponent3D
{
	virtual ~IPortalsComponent3D() {}
};



//
// MODELS COMPONENT 3D
//
#define UUID_ModelsComponent uuid("403E886D-7BD7-438B-868D-AC4380830716")
ZN_INTERFACE __declspec(UUID_ModelsComponent) ZN_API IModelsComponent3D
{
public:
	virtual ~IModelsComponent3D() {}

	virtual void AddModel(const std::shared_ptr<IModel>& Model) = 0;
	virtual void RemoveModel(const std::shared_ptr<IModel>& Model) = 0;
	virtual const ModelsList& GetModels() = 0;
};
const ComponentMessageType UUID_OnModelAdded = 30;
const ComponentMessageType UUID_OnModelRemoved = 30;



//
// LIGHT COMPONENT 3D
//
ZN_INTERFACE ZN_API ILight3D
{
	virtual ~ILight3D() {}

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const = 0;
	virtual const SLight& GetLightStruct() const = 0;
};

#define UUID_LightComponent uuid("2198326E-A00F-43C8-9EF5-4F60A8ABBBAE")
ZN_INTERFACE __declspec(UUID_LightComponent) ZN_API ILightComponent3D
{
	virtual ~ILightComponent3D() {}

	virtual void SetColor(glm::vec3 Value) = 0;
	virtual glm::vec3 GetColor() const = 0;

	virtual void SetType(ELightType Value) = 0;
	virtual ELightType GetType() const = 0;

	virtual void SetRange(float Value) = 0;
	virtual float GetRange() const = 0;

	virtual void SetIntensity(float Value) = 0;
	virtual float GetIntensity() const = 0;

	virtual void SetSpotlightAngle(float Value) = 0;
	virtual float GetSpotlightAngle() const = 0;
};



//
// CAMERA COMPONENT 3D
//
#define UUID_CameraComponent uuid("1F7DED3C-7622-46FA-BA17-4E405BA982DC")
ZN_INTERFACE __declspec(UUID_CameraComponent) ZN_API ICameraComponent3D
{
	enum class ZN_API EPerspectiveProjectionHand
	{
		Left = 0,
		Right
	};

	virtual ~ICameraComponent3D() {}

	virtual void DoMoveFront(float Value = 1.0f) = 0;
	virtual void DoMoveBack(float Value = 1.0f) = 0;
	virtual void DoMoveLeft(float Value = 1.0f) = 0;
	virtual void DoMoveRight(float Value = 1.0f) = 0;

	virtual void SetTranslation(glm::vec3 Translation) const = 0;
	virtual glm::vec3 GetTranslation() const = 0;
	virtual void SetDirection(glm::vec3 Direction) const = 0;
	virtual glm::vec3 GetDirection() const = 0;

	virtual void SetYaw(float Yaw) = 0;
	virtual void AddYaw(float Yaw) = 0;
	virtual float GetYaw() const = 0;

	virtual void SetPitch(float Pitch) = 0;
	virtual void AddPitch(float Pitch) = 0;
	virtual float GetPitch() const = 0;

	virtual void SetPerspectiveProjection(EPerspectiveProjectionHand PerspectiveProjectionHand, float fovy, float aspect, float zNear, float zFar) = 0;
	virtual void SetOrthographicProjection(float left, float right, float top, float bottom, float zNear, float zFar) = 0;

	virtual const glm::mat4& GetViewMatrix() const = 0;
	virtual const glm::mat4& GetInverseViewMatrix() const = 0;

	virtual const glm::mat4& GetProjectionMatrix() const = 0;
	virtual const glm::mat4& GetInverseProjectionMatrix() const = 0;

	virtual const glm::mat4& GetProjectionViewMatrix() const = 0;
	virtual const glm::mat4& GetInverseProjectionViewMatrix() const = 0;

	virtual const Frustum& GetFrustum() const = 0;
	virtual const glm::vec3& GetCameraUpDirection() const = 0;
};



//
// SKELETON COMPONENT 3D
//
ZN_INTERFACE ZN_API ISkeletonBone3D
{
	virtual ~ISkeletonBone3D() {}

	// Static data
	virtual const std::weak_ptr<ISkeletonBone3D>& GetParentBone() const = 0;
	virtual const std::vector<std::shared_ptr<ISkeletonBone3D>>& GetChilds() const = 0;
	virtual glm::vec3 GetPivotPoint() const = 0;

	// Dynamic data
	virtual const glm::mat4& GetMatrix() const = 0;
	virtual const glm::mat4& GetRotateMatrix() const = 0;
};

#define UUID_SkeletonComponent uuid("6A913E4D-B4E9-4E7C-8A09-F606D7A85CD5")
ZN_INTERFACE __declspec(UUID_SkeletonComponent) ZN_API ISkeletonComponent3D
{
	virtual ~ISkeletonComponent3D() {}

	virtual std::shared_ptr<ISkeletonBone3D> GetBone(size_t Index) const = 0;
	virtual void Calculate(uint32 GlobalTime) = 0;
};


//
// PARTICLE COMPONENT 3D
//

struct __declspec(novtable, align(16)) ZN_API SParticle
{
	SParticle()
		: Position(glm::vec3(0.0f))
		, Color(glm::vec4(1.0f))
		, Size(glm::vec2(10.0f))
	{}

	glm::vec3 Position;
	float __padding0;
	//--------------------------------------------------------------( 16 bytes )

	glm::vec4 Color;
	//--------------------------------------------------------------( 16 bytes )

	glm::vec2 Size;
	float __padding1[2];
	//--------------------------------------------------------------( 16 bytes )
};

ZN_INTERFACE ZN_API IParticleSystem
{
	virtual ~IParticleSystem() {}

	virtual void AddParticle(const SParticle& Particle) = 0;
	virtual const std::vector<SParticle>& GetParticles() const = 0;
};

#define UUID_ParticleComponent uuid("B0168C5F-60C0-4210-B3EF-740FEB89FFDD")
ZN_INTERFACE __declspec(UUID_ParticleComponent) ZN_API IParticleComponent3D
{
	virtual ~IParticleComponent3D() {}

};
