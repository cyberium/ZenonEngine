#pragma once

#include "../ComponentBase.h"

class __declspec(UUID_ColliderComponent) ZN_API CColliderComponent3D
	: public IColliderComponent3D
	, public CComponentBase
{
public:
	CColliderComponent3D(const ISceneNode3D& OwnerNode);
    virtual ~CColliderComponent3D();

    // IColliderComponent3D
    void SetBounds(BoundingBox _bbox) override;
    cbbox GetBounds() const override;
	cbbox GetWorldBounds() const override;
	void SetDebugDrawMode(bool Value) override;
	bool GetDebugDrawMode() const override;

	bool IsCulledByFrustum(const ICameraComponent3D* Camera) const override;
	bool IsCulledByDistance2D(const ICameraComponent3D* Camera, float Distance) const override;
	bool IsCulledByDistance(const ICameraComponent3D* Camera, float Distance) const override;

    // CComponentBase
    void OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message) override;

protected:
    virtual void UpdateBounds();

private:
    BoundingBox m_Bounds;
	BoundingBox m_WorldBounds;
	bool m_DebugDraw;
};