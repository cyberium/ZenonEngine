#include "stdafx.h"

// Include
#include "SceneFunctional\\SceneNode3D.h"

// General
#include "ColliderComponent.h"

CColliderComponent::CColliderComponent(std::shared_ptr<SceneNode3D> OwnerNode)
    : CComponentBase(OwnerNode)
{
}

CColliderComponent::~CColliderComponent()
{
}


// Bounds
void CColliderComponent::SetBounds(BoundingBox _bbox)
{
    m_Bounds = _bbox;
}
cbbox CColliderComponent::GetBounds() const
{
    return m_Bounds;
}

//void CColliderComponent::UpdateWorldTransform()
//{
//    base::UpdateWorldTransform();
//
//    UpdateBounds();
//}

bool CColliderComponent::checkFrustum(const Camera* _camera) const
{
    assert1(_camera != nullptr);
    return !_camera->GetFrustum().cullBox(GetBounds());
}

bool CColliderComponent::checkDistance2D(cvec3 _camPos, float _distance) const
{
    // Check distance to camera
    float distToCamera2D = glm::length(Fix_X0Z(_camPos) - Fix_X0Z(GetBounds().getCenter())) - GetBounds().getRadius();
    return distToCamera2D < _distance;
}

bool CColliderComponent::checkDistance(cvec3 _camPos, float _distance) const
{
    // Check distance to camera
    float distToCamera = glm::length(_camPos - GetBounds().getCenter()) - GetBounds().getRadius();
    return distToCamera < _distance;
}



//
// Protected
//
void CColliderComponent::UpdateBounds()
{
    // do nothing
}
