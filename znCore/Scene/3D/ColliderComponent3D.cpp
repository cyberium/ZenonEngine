#include "stdafx.h"

// General
#include "ColliderComponent3D.h"

CColliderComponent3D::CColliderComponent3D(const ISceneNode3D& OwnerNode)
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
	RaiseComponentMessage(UUID_OnBoundsChanget);
}
cbbox CColliderComponent3D::GetBounds() const
{
	return m_Bounds;
}

bool CColliderComponent3D::CheckFrustum(const ICameraComponent3D* Camera) const
{
	_ASSERT(Camera != nullptr);

	return !Camera->GetFrustum().cullBox(GetBounds());
}

bool CColliderComponent3D::CheckDistance2D(const ICameraComponent3D* Camera, float _distance) const
{
	_ASSERT(Camera != nullptr);

	glm::vec3 cameraPosition = Camera->GetTranslation();
	float distToCamera2D = glm::length(Fix_X0Z(cameraPosition) - Fix_X0Z(GetBounds().getCenter())) - GetBounds().getRadius();
	return distToCamera2D < _distance;
}

bool CColliderComponent3D::CheckDistance(const ICameraComponent3D* Camera, float _distance) const
{
	_ASSERT(Camera != nullptr);

	glm::vec3 cameraPosition = Camera->GetTranslation();
	float distToCamera = glm::length(cameraPosition - GetBounds().getCenter()) - GetBounds().getRadius();
	return distToCamera < _distance;
}

void CColliderComponent3D::OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message)
{
	switch (Message)
	{
	case UUID_OnTransformChanged:
	{
		UpdateBounds();
	}
	case UUID_OnParentChanged:
	{
		// Update THIS bounds
		if (GetOwnerNode().GetParent())
		{
			BoundingBox bbox = GetOwnerNode().GetParent()->GetComponent<IColliderComponent3D>()->GetBounds();
			bbox.makeUnion(GetBounds());
			GetOwnerNode().GetParent()->GetComponent<IColliderComponent3D>()->SetBounds(bbox);
		}
	}
	break;
	case UUID_OnBoundsChanget:
	{
		if (GetOwnerNode().GetParent())
		{
			// Update THIS bounds
			BoundingBox bbox = GetOwnerNode().GetParent()->GetComponent<IColliderComponent3D>()->GetBounds();
			bbox.makeUnion(GetBounds());
			GetOwnerNode().GetParent()->GetComponent<IColliderComponent3D>()->SetBounds(bbox);
		}
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
