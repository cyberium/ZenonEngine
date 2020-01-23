#pragma once

#include "../ComponentBase.h"

class __declspec(UUID_ColliderComponent) ZN_API CColliderComponent3D
	: public IColliderComponent3D
	, public CComponentBase
{
public:
	CColliderComponent3D(const ISceneNode3D* OwnerNode);
    virtual ~CColliderComponent3D();

    // IColliderComponent3D
    void SetBounds(BoundingBox _bbox) override;
    cbbox GetBounds() const override;
    bool CheckFrustum(const ICameraComponent3D* Camera) const override;
    bool CheckDistance2D(const ICameraComponent3D* Camera, float _distance) const override;
    bool CheckDistance(const ICameraComponent3D* Camera, float _distance) const override;

    // CComponentBase
    void OnMessage(ISceneNodeComponent* Component, ComponentMessageType Message) override;

protected:
    virtual void UpdateBounds();

private:
    BoundingBox m_Bounds;
};