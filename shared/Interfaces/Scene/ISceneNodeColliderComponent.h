#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNodeComponent;
ZN_INTERFACE ICameraComponent3D;
// FORWARD END


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