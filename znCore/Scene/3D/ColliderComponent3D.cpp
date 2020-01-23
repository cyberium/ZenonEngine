#include "stdafx.h"

// General
#include "ColliderComponent3D.h"

CColliderComponent3D::CColliderComponent3D(const ISceneNode3D* OwnerNode)
    : CComponentBase(OwnerNode)
{
}

CColliderComponent3D::~CColliderComponent3D()
{
}


//
// IColliderComponent3D
//
void CColliderComponent3D::SetBounds(BoundingBox _bbox)
{
    m_Bounds = _bbox;
}
cbbox CColliderComponent3D::GetBounds() const
{
    return m_Bounds;
}

bool CColliderComponent3D::CheckFrustum(const ICameraComponent3D* Camera) const
{
    _ASSERT(Camera != nullptr);

	return true;// !Camera->GetFrustum()->cullBox(GetBounds());
}

bool CColliderComponent3D::CheckDistance2D(const ICameraComponent3D* Camera, float _distance) const
{
    _ASSERT(Camera != nullptr);

    //glm::vec3 cameraPosition = Camera->GetTranslation();
    //float distToCamera2D = glm::length(Fix_X0Z(cameraPosition) - Fix_X0Z(GetBounds().getCenter())) - GetBounds().getRadius();
    return true;//distToCamera2D < _distance;
}

bool CColliderComponent3D::CheckDistance(const ICameraComponent3D* Camera, float _distance) const
{
    _ASSERT(Camera != nullptr);

    //glm::vec3 cameraPosition = Camera->GetTranslation();
    //float distToCamera = glm::length(cameraPosition - GetBounds().getCenter()) - GetBounds().getRadius();
    return true;//distToCamera < _distance;
}

void CColliderComponent3D::OnMessage(ISceneNodeComponent* Component, ComponentMessageType Message)
{
    switch (Message)
    {
        //case UUID_TransformComponent_OnWorldTransformChanged:
        //{
        //    UpdateBounds();
        //}
        //break;

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
