#include "stdafx.h"

// Include
#include "SceneFunctional/Base/SceneNode.h"

// General
#include "ColliderComponent3D.h"

// Additonal
#include "TransformComponent3D.h"

CColliderComponent3D::CColliderComponent3D(std::shared_ptr<SceneNode> OwnerNode)
    : CComponentBase(OwnerNode)
{
}

CColliderComponent3D::~CColliderComponent3D()
{
}


// Bounds
void CColliderComponent3D::SetBounds(BoundingBox _bbox)
{
    m_Bounds = _bbox;
}
cbbox CColliderComponent3D::GetBounds() const
{
    return m_Bounds;
}

bool CColliderComponent3D::checkFrustum(const Camera* _camera) const
{
    assert1(_camera != nullptr);
    return !_camera->GetFrustum().cullBox(GetBounds());
}

bool CColliderComponent3D::checkDistance2D(cvec3 _camPos, float _distance) const
{
    // Check distance to camera
    float distToCamera2D = glm::length(Fix_X0Z(_camPos) - Fix_X0Z(GetBounds().getCenter())) - GetBounds().getRadius();
    return distToCamera2D < _distance;
}

bool CColliderComponent3D::checkDistance(cvec3 _camPos, float _distance) const
{
    // Check distance to camera
    float distToCamera = glm::length(_camPos - GetBounds().getCenter()) - GetBounds().getRadius();
    return distToCamera < _distance;
}

void CColliderComponent3D::OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message)
{
    switch (Message)
    {
        case UUID_TransformComponent_OnWorldTransformChanged:
        {
            UpdateBounds();
        }
        break;

        default:
        {
            // do nothing
        }
        break;
    }
    
}



//
// Protected
//
void CColliderComponent3D::UpdateBounds()
{
    // do nothing
}
