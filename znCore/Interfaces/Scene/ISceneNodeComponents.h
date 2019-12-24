#pragma once

// FORWARD BEGIN
ZN_INTERFACE IPropertiesGroup;
// FORWARD END

typedef uint32 ComponentMessageType;

ZN_INTERFACE OW_ENGINE_API ISceneNodeComponent : public std::enable_shared_from_this<ISceneNodeComponent>
{
	virtual ~ISceneNodeComponent() {}

    // Callbacks
    virtual void OnParentChanged() = 0;
    virtual void OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) = 0;
	virtual std::shared_ptr<IPropertiesGroup> GetPropertiesGroup() const = 0;

    virtual bool Accept(IVisitor* visitor) = 0;
};
typedef std::unordered_map<GUID, std::shared_ptr<ISceneNodeComponent>> ComponentsMap;






#define UUID_TransformComponent uuid("D0E12217-352E-4B51-AE46-EC287CA9A19C")
#define UUID_ColliderComponent uuid("78BD7168-51CB-4760-ADD2-218CF4E88CE2")
#define UUID_MeshComponent uuid("403E886D-7BD7-438B-868D-AC4380830716")











ZN_INTERFACE __declspec(UUID_TransformComponent) OW_ENGINE_API ITransformComponent
{
	virtual ~ITransformComponent() {}

	virtual mat4 GetLocalTransform() const = 0;
	virtual mat4 GetInverseLocalTransform() const = 0;
	virtual void SetLocalTransform(cmat4 localTransform) = 0;

	virtual mat4 GetWorldTransfom() const = 0;
	virtual mat4 GetInverseWorldTransform() const = 0;
	virtual mat4 GetParentWorldTransform() const = 0;
	virtual void SetWorldTransform(cmat4 worldTransform) = 0;

	virtual void ForceRecalculateLocalTransform() = 0;
};














// TRANSFORM COMPONENT 3D
ZN_INTERFACE __declspec(UUID_TransformComponent) OW_ENGINE_API ITransformComponent3D
{
	virtual ~ITransformComponent3D() {}

	virtual void SetTranslate(cvec3 _translate) = 0;
	virtual cvec3 GetTranslation() const = 0;

	virtual void SetRotation(cvec3 _rotate) = 0;
	virtual cvec3 GetRotation() const = 0;

	virtual void SetRotationQuaternion(cquat _rotate) = 0;
	virtual cquat GetRotationQuaternion() const = 0;

	virtual void SetScale(cvec3 _scale) = 0;
	virtual cvec3 GetScale() const = 0;
};






// TRANSFORM COMPONENT UI
ZN_INTERFACE __declspec(UUID_TransformComponent) OW_ENGINE_API ITransformComponentUI
{
	virtual ~ITransformComponentUI() {}

	virtual void SetTranslate(cvec2 _translate) = 0;
	virtual cvec2 GetTranslation() const = 0;
	virtual glm::vec2 GetTranslationAbs() const = 0;

	virtual void SetRotation(cvec3 _rotate) = 0;
	virtual cvec3 GetRotation() const = 0;

	virtual void SetScale(cvec2 _scale) = 0;
	virtual cvec2 GetScale() const = 0;
	virtual glm::vec2 GetScaleAbs() const = 0;
};






// COLLIDER COMPONENT 3D
ZN_INTERFACE __declspec(UUID_ColliderComponent)	OW_ENGINE_API IColliderComponent3D
{
	virtual ~IColliderComponent3D() {}

	virtual void SetBounds(BoundingBox _bbox) = 0;
	virtual cbbox GetBounds() const = 0;

	virtual bool CheckFrustum(const ICamera* Camera) const = 0;
	virtual bool CheckDistance2D(const ICamera* Camera, float _distance) const = 0;
	virtual bool CheckDistance(const ICamera* Camera, float _distance) const = 0;
};



// MESH COMPONENT 3D
ZN_INTERFACE __declspec(UUID_MeshComponent)	OW_ENGINE_API IMeshComponent3D 
{
public:
	typedef std::vector<std::shared_ptr<IMesh>> MeshList;

	virtual ~IMeshComponent3D() {}

	virtual void AddMesh(std::shared_ptr<IMesh> mesh) = 0;
	virtual void RemoveMesh(std::shared_ptr<IMesh> mesh) = 0;
	virtual const MeshList& GetMeshes() = 0;
};