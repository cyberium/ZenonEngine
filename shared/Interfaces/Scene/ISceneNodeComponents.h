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
    virtual void OnParentChanged() = 0;
    virtual void OnMessage(ISceneNodeComponent* Component, ComponentMessageType Message) = 0;
	virtual IPropertiesGroup* GetPropertiesGroup() const = 0;

	// Load & Save
	virtual bool Load(std::shared_ptr<IXMLReader> Reader) = 0;
	virtual bool Save(std::shared_ptr<IXMLWriter> Writer) = 0;

	// Called before all others calls
	virtual void DoUpdate(UpdateEventArgs& e) = 0;

	// Visit functional
    virtual bool Accept(IVisitor* visitor) = 0;
};
typedef std::unordered_map<GUID, std::shared_ptr<ISceneNodeComponent>> ComponentsMap;

const ComponentMessageType UUID_OnParentChanged = 1;
const ComponentMessageType UUID_OnTransformChanged = 2; // Change LocalTransform of parent, change world transform of this or parent, 



//
// COLLIDER COMPONENT 3D
//
#define UUID_ColliderComponent uuid("78BD7168-51CB-4760-ADD2-218CF4E88CE2")
ZN_INTERFACE __declspec(novtable, UUID_ColliderComponent) ZN_API IColliderComponent3D
{
	virtual ~IColliderComponent3D() {}

	virtual void SetBounds(BoundingBox _bbox) = 0;
	virtual cbbox GetBounds() const = 0;

	virtual bool CheckFrustum(const ICameraComponent3D* Camera) const = 0;
	virtual bool CheckDistance2D(const ICameraComponent3D* Camera, float _distance) const = 0;
	virtual bool CheckDistance(const ICameraComponent3D* Camera, float _distance) const = 0;
};



//
// MESH COMPONENT 3D
//
#define UUID_MeshComponent uuid("403E886D-7BD7-438B-868D-AC4380830716")
ZN_INTERFACE __declspec(novtable, UUID_MeshComponent) ZN_API IMeshComponent3D
{
public:
	typedef std::vector<std::shared_ptr<IModel>> MeshList;

	virtual ~IMeshComponent3D() {}

	virtual void AddMesh(std::shared_ptr<IModel> mesh) = 0;
	virtual void RemoveMesh(IModel* mesh) = 0;
	virtual const MeshList& GetMeshes() = 0;
};



//
// LIGHT COMPONENT 3D
//
#define UUID_LightComponent uuid("2198326E-A00F-43C8-9EF5-4F60A8ABBBAE")
ZN_INTERFACE __declspec(novtable, UUID_LightComponent) ZN_API ILightComponent3D
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

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const = 0;

	virtual const SLight& GetLightStruct() const = 0;
};



//
// CAMERA COMPONENT 3D
//
#define UUID_CameraComponent uuid("1F7DED3C-7622-46FA-BA17-4E405BA982DC")
ZN_INTERFACE __declspec(novtable, UUID_CameraComponent) ZN_API ICameraComponent3D
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
};
