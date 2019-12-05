#pragma once

#include "../../SceneFunctional/Base/ComponentBase.h"

#define UUID_ColliderComponent uuid("78BD7168-51CB-4760-ADD2-218CF4E88CE2")

class
    __declspec(UUID_ColliderComponent)
    OW_ENGINE_API
    CColliderComponent3D : public CComponentBase
{
public:
                                                    CColliderComponent3D(std::shared_ptr<ISceneNode> OwnerNode);
    virtual                                         ~CColliderComponent3D();

    // Bounds
    void                                            SetBounds(BoundingBox _bbox);
    cbbox                                           GetBounds() const;

    // Culling stuff
    bool                                            CheckFrustum(const ICamera* Camera) const;
    bool                                            CheckDistance2D(const ICamera* Camera, float _distance) const;
    bool                                            CheckDistance(const ICamera* Camera, float _distance) const;

    // CComponentBase
    void                                            OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message) override;

protected:
    virtual void                                    UpdateBounds();

private:
    BoundingBox                                     m_Bounds;
};