#include "stdafx.h"

// General
#include "ColliderComponent3D.h"

CColliderComponent3D::CColliderComponent3D(const ISceneNode3D& OwnerNode)
	: CComponentBase(OwnerNode)
	, m_DebugDraw(true)
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
	RaiseComponentMessage(UUID_OnBoundsChanget);
}
cbbox CColliderComponent3D::GetBounds() const
{
	return m_Bounds;
}

cbbox CColliderComponent3D::GetWorldBounds() const
{
	return m_WorldBounds;
}

void CColliderComponent3D::SetDebugDrawMode(bool Value)
{
	m_DebugDraw = Value;
}

bool CColliderComponent3D::GetDebugDrawMode() const
{
	return m_DebugDraw;
}

bool CColliderComponent3D::IsCulledByFrustum(const ICameraComponent3D* Camera) const
{
	_ASSERT(Camera != nullptr);

	return Camera->GetFrustum().cullBox(GetWorldBounds());
}

bool CColliderComponent3D::IsCulledByDistance2D(const ICameraComponent3D* Camera, float _distance) const
{
	_ASSERT(Camera != nullptr);

	glm::vec3 cameraPosition = Camera->GetTranslation();
	float distToCamera2D = glm::length(Fix_X0Z(cameraPosition) - Fix_X0Z(GetWorldBounds().getCenter())) - GetWorldBounds().getRadius();
	return distToCamera2D > _distance;
}

bool CColliderComponent3D::IsCulledByDistance(const ICameraComponent3D* Camera, float _distance) const
{
	_ASSERT(Camera != nullptr);

	glm::vec3 cameraPosition = Camera->GetTranslation();
	float distToCamera = glm::length(cameraPosition - GetWorldBounds().getCenter()) - GetWorldBounds().getRadius();
	return distToCamera > _distance;
}

void CColliderComponent3D::OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message)
{
	switch (Message)
	{
	case UUID_OnWorldTransformChanged:
	{
		UpdateBounds();
	}
	break;
	case UUID_OnBoundsChanget:
	{
		_ASSERT(Component == this);
		UpdateBounds();
	}
	break;
	}

}



//
// Protected
//
void CColliderComponent3D::UpdateBounds()
{
	BoundingBox bounds = m_Bounds;
	bounds.transform(GetOwnerNode().GetWorldTransfom());
	m_WorldBounds = bounds;
}
