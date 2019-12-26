#pragma once

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

	// Visit functional
    virtual bool Accept(IVisitor* visitor) = 0;
};
typedef std::unordered_map<GUID, std::shared_ptr<ISceneNodeComponent>> ComponentsMap;



#define UUID_ColliderComponent uuid("78BD7168-51CB-4760-ADD2-218CF4E88CE2")
#define UUID_MeshComponent uuid("403E886D-7BD7-438B-868D-AC4380830716")



// COLLIDER COMPONENT 3D
ZN_INTERFACE __declspec(UUID_ColliderComponent)	ZN_API IColliderComponent3D
{
	virtual ~IColliderComponent3D() {}

	virtual void SetBounds(BoundingBox _bbox) = 0;
	virtual cbbox GetBounds() const = 0;

	virtual bool CheckFrustum(const ICamera* Camera) const = 0;
	virtual bool CheckDistance2D(const ICamera* Camera, float _distance) const = 0;
	virtual bool CheckDistance(const ICamera* Camera, float _distance) const = 0;
};



// MESH COMPONENT 3D
ZN_INTERFACE __declspec(UUID_MeshComponent)	ZN_API IMeshComponent3D 
{
public:
	typedef std::vector<std::shared_ptr<IMesh>> MeshList;

	virtual ~IMeshComponent3D() {}

	virtual void AddMesh(std::shared_ptr<IMesh> mesh) = 0;
	virtual void RemoveMesh(std::shared_ptr<IMesh> mesh) = 0;
	virtual const MeshList& GetMeshes() = 0;
};