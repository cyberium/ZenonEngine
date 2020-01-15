#pragma once

#include "Interfaces/Scene/Types/Light.h"

// FORWARD BEGIN
ZN_INTERFACE IPropertiesGroup;
ZN_INTERFACE IXMLReader;
ZN_INTERFACE IXMLWriter;
// FORWARD END

typedef uint32 ComponentMessageType;

ZN_INTERFACE ZN_API ISceneNodeComponent : public std::enable_shared_from_this<ISceneNodeComponent>
{
	virtual ~ISceneNodeComponent() {}

    // Callbacks
    virtual void OnParentChanged() = 0;
    virtual void OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) = 0;
	virtual std::shared_ptr<IPropertiesGroup> GetPropertiesGroup() const = 0;

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
ZN_INTERFACE __declspec(UUID_ColliderComponent)	ZN_API IColliderComponent3D
{
	virtual ~IColliderComponent3D() {}

	virtual void SetBounds(BoundingBox _bbox) = 0;
	virtual cbbox GetBounds() const = 0;

	virtual bool CheckFrustum(const ICamera* Camera) const = 0;
	virtual bool CheckDistance2D(const ICamera* Camera, float _distance) const = 0;
	virtual bool CheckDistance(const ICamera* Camera, float _distance) const = 0;
};



//
// MESH COMPONENT 3D
//
#define UUID_MeshComponent uuid("403E886D-7BD7-438B-868D-AC4380830716")
ZN_INTERFACE __declspec(UUID_MeshComponent)	ZN_API IMeshComponent3D 
{
public:
	typedef std::vector<std::shared_ptr<IMesh>> MeshList;

	virtual ~IMeshComponent3D() {}

	virtual void AddMesh(std::shared_ptr<IMesh> mesh) = 0;
	virtual void RemoveMesh(std::shared_ptr<IMesh> mesh) = 0;
	virtual const MeshList& GetMeshes() = 0;
};



//
// LIGHT COMPONENT 3D
//

#define UUID_LightComponent uuid("2198326E-A00F-43C8-9EF5-4F60A8ABBBAE")
ZN_INTERFACE __declspec(UUID_LightComponent) ZN_API ILightComponent3D
{
	virtual ~ILightComponent3D() {}

	virtual void SetColor(glm::vec3 Value) = 0;
	virtual glm::vec3 GetColor() const = 0;

	virtual void SetEnabled(bool Value) = 0;
	virtual bool GetEnabled() const = 0;

	virtual void SetRange(float Value) = 0;
	virtual float GetRange() const = 0;

	virtual void SetIntensity(float Value) = 0;
	virtual float GetIntensity() const = 0;

	virtual void SetSpotlightAngle(float Value) = 0;
	virtual float GetSpotlightAngle() const = 0;

	virtual void SetType(ELightType Value) = 0;
	virtual ELightType GetType() const = 0;

	virtual glm::mat4 GetViewMatrix() const = 0;
	virtual glm::mat4 GetProjectionMatrix() const = 0;

	virtual const SLight& GetLightStruct() const = 0;
};