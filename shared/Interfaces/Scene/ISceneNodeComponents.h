#pragma once

#include "Interfaces/Scene/Types/Light.h"

// FORWARD BEGIN
ZN_INTERFACE IPropertiesGroup;
ZN_INTERFACE IXMLReader;
ZN_INTERFACE IXMLWriter;
// FORWARD END

typedef uint32 ComponentMessageType;


ZN_INTERFACE ZN_API ISceneNodeComponent
	: public virtual IObject
	, public virtual IObjectLoadSave
	, public std::enable_shared_from_this<ISceneNodeComponent>
{
	ZN_OBJECTCLASS(cSceneNodeComponent);

	virtual ~ISceneNodeComponent() {}

	// Callbacks
	virtual void OnMessage(const ISceneNodeComponent* FromComponent, ComponentMessageType Message) = 0;
	virtual std::shared_ptr<IPropertiesGroup> GetProperties() const = 0;

	// Visit functional
	virtual void Update(const UpdateEventArgs& e) = 0;
	virtual void Accept(IVisitor* visitor) = 0;
};
typedef std::unordered_map<ObjectClass, std::shared_ptr<ISceneNodeComponent>> ComponentsMap;

const ComponentMessageType UUID_OnParentChanged = 1;
const ComponentMessageType UUID_OnLocalTransformChanged = 2;
const ComponentMessageType UUID_OnWorldTransformChanged = 3;



//
// COLLIDER COMPONENT 3D
//
ZN_INTERFACE ZN_API IColliderComponent
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

	ZN_OBJECTCLASS(cSceneNodeColliderComponent);

	virtual ~IColliderComponent() {}

	virtual void SetBounds(BoundingBox Bounds) = 0;
	virtual void ExtendBounds(BoundingBox Bounds) = 0;
	virtual const BoundingBox& GetBounds() const = 0;
	virtual const BoundingBox& GetWorldBounds() const = 0;

	virtual void SetCullStrategy(ECullStrategy CullStrategy) = 0;
	virtual ECullStrategy GetCullStrategy() const = 0;
	virtual void SetCullDistance(float Distance) = 0;
	virtual float GetCullDistance() const = 0;
	virtual void SetDebugDrawMode(bool Value) = 0;
	virtual bool GetDebugDrawMode() const = 0;

	virtual bool IsCulled(const ICameraComponent3D* Camera) const = 0;
	virtual bool IsCulledByFrustum(const ICameraComponent3D* Camera) const = 0;
	virtual bool IsCulledByDistance2D(const ICameraComponent3D* Camera) const = 0;
	virtual bool IsCulledByDistance(const ICameraComponent3D* Camera) const = 0;
	virtual bool IsRayIntersects(const Ray& Ray) const = 0;
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
	virtual bool IsPositive(const glm::vec3& InvTranslateCamera) const = 0;

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

ZN_INTERFACE ZN_API IPortalsComponent3D
{
	ZN_OBJECTCLASS(cSceneNodePortalsComponent);

	virtual ~IPortalsComponent3D() {}
};



//
// SKELETON COMPONENT 3D
//
ZN_INTERFACE ZN_API ISkeletonComponentBone3D
{
	virtual ~ISkeletonComponentBone3D() {}

	// Static data
	virtual std::string GetName() const = 0;
	virtual const std::weak_ptr<ISkeletonComponentBone3D>& GetParentBone() const = 0;
	virtual const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& GetChilds() const = 0;
	virtual const ISkeletonBone& GetProtoBone() const = 0;

	// Dynamic data
	virtual const glm::mat4& GetCalculatedMatrix() const = 0;
};

ZN_INTERFACE ZN_API ISkeletonComponentBoneInternal3D
{
	virtual ~ISkeletonComponentBoneInternal3D() {}

	// Static data
	virtual void AddChildInternal(const std::shared_ptr<ISkeletonComponentBone3D>& Child) = 0;
	virtual void SetParentAndChildsInternals(const std::vector<std::shared_ptr<ISkeletonComponentBone3D>>& Bones) = 0;
	virtual void Calculate(const IModelComponent* ModelsComponent, const ICameraComponent3D* Camera) = 0;
	virtual void Reset() = 0;
};



struct SBoneInstance
{
	SBoneInstance()
		: Bone(nullptr)
		, IsCalculated(false)
		, BoneMatrix(glm::mat4(1.0f))
	{}

	const ISkeletonBone* Bone;
	bool IsCalculated;
	glm::mat4 BoneMatrix;
};


//
// MODELS COMPONENT 3D
//
ZN_INTERFACE ZN_API IModelComponent
{
	ZN_OBJECTCLASS(cSceneNodeModelsComponent);

	virtual ~IModelComponent() {}

	virtual void SetModel(const std::shared_ptr<IModel>& Model) = 0;
	virtual void ResetModel() = 0;
	virtual std::shared_ptr<IModel> GetModel() const = 0;

	virtual void SetCastShadows(bool Value) = 0;
	virtual bool IsCastShadows() const = 0;

	// Bones functional
	//virtual std::shared_ptr<ISkeletonComponentBone3D> GetRootBone() const = 0;
	virtual const SBoneInstance& GetCalculatedBone(size_t Index) const = 0;
	virtual const std::vector<SBoneInstance>& GetCalculatedBones() const = 0;
	virtual std::shared_ptr<IStructuredBuffer> GetBonesSkinBuffer() const = 0;
	virtual void CreatePose(size_t BoneStartIndex = 0, size_t BonesCount = 0) = 0;

	// Animation functional
	virtual void PlayAnimation(const std::string& AnimationName, bool Loop) = 0;
	virtual void SetAnimationEndedCallback(std::function<void(const IAnimation*)> Func) = 0;
	virtual bool IsAnimationPlayed() const = 0;
	virtual const IAnimation* GetCurrentAnimation() const = 0;
	virtual size_t GetCurrentAnimationIndex() const = 0;
	virtual uint32 GetCurrentAnimationFrame() const = 0;
};
const ComponentMessageType UUID_OnModelSetted = 30;



//
// LIGHT COMPONENT 3D
//

// DON'T FORGET ABOUT PADDINGS!
// DON'T USE 'bool'!

enum class ZN_API ELightType : uint32_t // Don't delete uint32_t becouse mapped to render
{
	Point = 0,
	Spot,
	Directional
};

struct __declspec(align(16)) ZN_API SGPULight
{
	SGPULight()
		: AmbientColor(0.1f, 0.1f, 0.1f, 1.0f)
		, Color(1.0f, 1.0f, 1.0f, 1.0f)
		, Type(ELightType::Point)
		, Range(5000.0f)
		, Intensity(1.0f)
		, SpotlightAngle(45.0f)
	{}

	glm::vec4 AmbientColor;// Ambient color of the light.
	//--------------------------------------------------------------(16 bytes )
	glm::vec4 Color;       // Color of the light. Diffuse and specular colors are not separated.
	//--------------------------------------------------------------(16 bytes )
	ELightType Type; // The type of the light.
	float Range; // The range of the light.
	float Intensity; // The intensity of the light.
	float SpotlightAngle; // The half angle of the spotlight cone.
	//--------------------------------------------------------------(16 bytes )
};

ZN_INTERFACE ZN_API ILight3D
{
	virtual ~ILight3D() {}

	virtual void SetEnabled(bool Value) = 0;
	virtual bool IsEnabled() const = 0;
	virtual void SetCastShadows(bool Value) = 0;
	virtual bool IsCastShadows() const = 0;
	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const = 0;
	virtual Frustum GetFrustum() const = 0;
	virtual const SGPULight& GetGPULightStruct() const = 0;
};

ZN_INTERFACE ZN_API ILightComponent3D
{
	ZN_OBJECTCLASS(cSceneNodeLightComponent);

	virtual ~ILightComponent3D() {}

	virtual void SetAmbientColor(glm::vec3 Value) = 0;
	virtual glm::vec3 GetAmbientColor() const = 0;

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
ZN_INTERFACE ZN_API ICameraComponent3D
{
	ZN_OBJECTCLASS(cSceneNodeCameraComponent);

	virtual ~ICameraComponent3D() {}

	virtual void DoMoveFront(float Value = 1.0f) = 0;
	virtual void DoMoveBack(float Value = 1.0f) = 0;
	virtual void DoMoveLeft(float Value = 1.0f) = 0;
	virtual void DoMoveRight(float Value = 1.0f) = 0;

	virtual void SetTranslation(glm::vec3 Translation) = 0;
	virtual glm::vec3 GetTranslation() const = 0;
	virtual void SetDirection(glm::vec3 Direction) = 0;
	virtual glm::vec3 GetDirection() const = 0;

	virtual void SetYaw(float Yaw) = 0;
	virtual void AddYaw(float Yaw) = 0;
	virtual float GetYaw() const = 0;

	virtual void SetPitch(float Pitch) = 0;
	virtual void AddPitch(float Pitch) = 0;
	virtual float GetPitch() const = 0;

	virtual void SetPerspectiveProjection(float fovy, float aspect, float zNear, float zFar) = 0;
	virtual void SetOrthographicProjection(float aspect, float left, float right, float top, float bottom, float zNear, float zFar) = 0;

	virtual const glm::mat4& GetViewMatrix() const = 0;
	virtual const glm::mat4& GetInverseViewMatrix() const = 0;

	virtual const glm::mat4& GetProjectionMatrix() const = 0;
	virtual const glm::mat4& GetInverseProjectionMatrix() const = 0;

	virtual const Frustum& GetFrustum() const = 0;
	virtual const glm::vec3& GetCameraUpDirection() const = 0;
};











//
// PARTICLE COMPONENT 3D
//
struct __declspec(align(16)) ZN_API SGPUParticle
{
	SGPUParticle()
		: Position(glm::vec3(0.0f))
		, Color(glm::vec4(1.0f))
		, Size(glm::vec2(10.0f))
		, TexCoordBegin(glm::vec2(0.0f))
		, TexCoordEnd(glm::vec2(1.0f))
	{}

	explicit SGPUParticle(const glm::vec3& Position_, const glm::vec4& Color_, const glm::vec2& Size_)
		: Position(Position_)
		, Color(Color_)
		, Size(Size_)
		, TexCoordBegin(glm::vec2(0.0f))
		, TexCoordEnd(glm::vec2(1.0f))
	{}

	glm::vec3 Position;
	float __padding0;
	//--------------------------------------------------------------( 16 bytes )

	glm::vec2 TexCoordBegin;
	glm::vec2 TexCoordEnd;
	//--------------------------------------------------------------( 16 bytes )

	glm::vec4 Color;
	//--------------------------------------------------------------( 16 bytes )

	glm::vec2 Size;
	float __padding1[2];
	//--------------------------------------------------------------( 16 bytes )
};


struct ZN_API SCPUParticle
{
	SCPUParticle(float MaxLifeTime_)
		: Color(glm::vec4(1.0f))
		, Size(glm::vec2(3.0f))
		, TexCoordBegin(glm::vec2(0.0f))
		, TexCoordEnd(glm::vec2(1.0f))

		, Position(glm::vec3(0.0f))
		, StartPosition(glm::vec3(0.0f))
		, Direction(glm::vec3(0.0f))
		, Speed(1.0f)
		
		, CurrentLifeTime(0.0f)
		, MaxLifeTime(MaxLifeTime_)
	{}

	glm::vec4 Color;
	glm::vec2 Size;
	
	glm::vec2 TexCoordBegin;
	glm::vec2 TexCoordEnd;
	
	glm::vec3 Position;
	glm::vec3 StartPosition;
	glm::vec3 Direction;
	float     Speed;
	
	float     CurrentLifeTime;
	float     MaxLifeTime;

	SGPUParticle ToGPUParticle() const
	{
		return SGPUParticle(Position, Color, Size);
	}
};


ZN_INTERFACE ZN_API IParticleSystem
{
	virtual ~IParticleSystem() {}

	virtual void Update(const UpdateEventArgs& e) = 0;

	virtual const std::vector<SGPUParticle>& GetGPUParticles() const = 0;

	virtual void SetEnableCreatingNewParticles(bool Value) = 0;
	virtual bool IsEnableCreatingNewParticles() const = 0;

	virtual void SetTexture(const std::shared_ptr<ITexture>& Texture) = 0;
	virtual std::shared_ptr<ITexture> GetTexture() const = 0;

	virtual std::shared_ptr<IBlendState> GetBlendState() const = 0;
};

ZN_INTERFACE ZN_API IParticleComponent3D
{
	ZN_OBJECTCLASS(cSceneNodeParticleComponent);
	virtual ~IParticleComponent3D() {}

	virtual void Attach(std::shared_ptr<IParticleSystem> ParticleSystem) = 0;
	virtual std::shared_ptr<IParticleSystem> Detach(std::shared_ptr<IParticleSystem> ParticleSystem) = 0;
	virtual const std::vector<std::shared_ptr<IParticleSystem>>& GetParticleSystems() const = 0;
};









//
// PHYSICS BODY COMPONENT 3D
//

ZN_INTERFACE ZN_API IPhysicsBody
{
	virtual ~IPhysicsBody() {}

	virtual glm::vec3 GetPosition() const = 0;
};

ZN_INTERFACE ZN_API IPhysicsComponent
{
	ZN_OBJECTCLASS(cSceneNodePhysicsComponent);

	virtual ~IPhysicsComponent() {}

	virtual glm::vec3 GetPhysicsPosition() const = 0;
};
