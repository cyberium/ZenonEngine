#include "stdafx.h"

// General
#include "ColliderComponent3D.h"

CColliderComponent3D::CColliderComponent3D(const ISceneNode& OwnerNode)
	: CComponentBase(OwnerNode)
	, m_CullStrategy(ECullStrategy::None)
	, m_CullDistance(99999.0f) // Don't use FloatMax
	, m_DebugDraw(true)
{
	GetProperties()->SetName("ColliderComponent");

	{
		auto minBounds = MakeShared(CPropertyWrappedVec3, "BBoxMin", "");
		minBounds->SetValueSetter(std::bind(&CColliderComponent3D::SetMinBounds, this, std::placeholders::_1));
		minBounds->SetValueGetter(std::bind(&CColliderComponent3D::GetMinBounds, this));
		GetProperties()->AddProperty(minBounds);

		auto maxBounds = MakeShared(CPropertyWrappedVec3, "BBoxMax", "");
		maxBounds->SetValueSetter(std::bind(&CColliderComponent3D::SetMaxBounds, this, std::placeholders::_1));
		maxBounds->SetValueGetter(std::bind(&CColliderComponent3D::GetMaxBounds, this));
		GetProperties()->AddProperty(maxBounds);
	}

	// Actions
	{
		std::shared_ptr<CAction> clear = MakeShared(CAction, "Clear", "Clean BBox. (set to infinite).");
		clear->SetAction([this] () -> bool {
			SetBounds(BoundingBox());
			return true;
		});

		GetProperties()->AddProperty(clear);
	}
}

CColliderComponent3D::~CColliderComponent3D()
{
}



//
// IColliderComponent3D
//

void CColliderComponent3D::SetBounds(BoundingBox Bounds)
{
	if (Bounds.IsInfinite())
		throw CException("Unable to set infinity bounds to node '%s'.", GetOwnerNode().GetName().c_str());

	m_Bounds = Bounds;

	RaiseComponentMessage(UUID_OnBoundsChanget);
}

void CColliderComponent3D::ExtendBounds(BoundingBox Bounds)
{
	if (Bounds.IsInfinite())
		throw CException("Unable to extend infinity bounds to node '%s'.", GetOwnerNode().GetName().c_str());

	if (m_Bounds.IsInfinite())
		m_Bounds = Bounds;
	else
		m_Bounds.makeUnion(Bounds);

	RaiseComponentMessage(UUID_OnBoundsChanget);
}

const BoundingBox& CColliderComponent3D::GetBounds() const
{
	return m_Bounds;
}

const BoundingBox& CColliderComponent3D::GetWorldBounds() const
{
	return m_WorldBounds;
}

void CColliderComponent3D::SetCullStrategy(ECullStrategy CullStrategy)
{
	m_CullStrategy = CullStrategy;
}

IColliderComponent3D::ECullStrategy CColliderComponent3D::GetCullStrategy() const
{
	return m_CullStrategy;
}

void CColliderComponent3D::SetCullDistance(float Distance)
{
	m_CullDistance = Distance;
}

float CColliderComponent3D::GetCullDistance() const
{
	return m_CullDistance;
}

void CColliderComponent3D::SetDebugDrawMode(bool Value)
{
	m_DebugDraw = Value;
}

bool CColliderComponent3D::GetDebugDrawMode() const
{
	return m_DebugDraw;
}

bool CColliderComponent3D::IsCulled(const ICameraComponent3D * Camera) const
{
	if (GetBounds().IsInfinite())
		return false;

	switch (m_CullStrategy)
	{
		case ECullStrategy::None:
			return false;
		case ECullStrategy::ByDistance:
			return IsCulledByDistance(Camera);
		case ECullStrategy::ByDistance2D:
			return IsCulledByDistance2D(Camera);
		case ECullStrategy::ByFrustrum:
			return IsCulledByFrustum(Camera);
		case ECullStrategy::ByFrustrumAndDistance:
			return IsCulledByDistance(Camera) || IsCulledByFrustum(Camera);
		case ECullStrategy::ByFrustrumAndDistance2D:
			return IsCulledByDistance2D(Camera) || IsCulledByFrustum(Camera);
	}

	throw CException("Unknown culling strategy.");
}

bool CColliderComponent3D::IsCulledByFrustum(const ICameraComponent3D* Camera) const
{
	_ASSERT(Camera != nullptr);

	if (GetBounds().IsInfinite())
		return false;

	return Camera->GetFrustum().cullBox(GetWorldBounds());
}

bool CColliderComponent3D::IsCulledByDistance2D(const ICameraComponent3D* Camera) const
{
	_ASSERT(Camera != nullptr);

	if (GetBounds().IsInfinite())
		return false;

	const glm::vec3 cameraPosition = Camera->GetTranslation();
	const float distToCamera2D = glm::length(Fix_X0Z(cameraPosition) - Fix_X0Z(GetWorldBounds().getCenter())) - GetWorldBounds().getRadius();
	return distToCamera2D > m_CullDistance;
}

bool CColliderComponent3D::IsCulledByDistance(const ICameraComponent3D* Camera) const
{
	_ASSERT(Camera != nullptr);

	if (GetBounds().IsInfinite())
		return false;

	const glm::vec3 cameraPosition = Camera->GetTranslation();
	const float distToCamera = glm::length(cameraPosition - GetWorldBounds().getCenter()) - GetWorldBounds().getRadius();
	return distToCamera > m_CullDistance;
}

bool CColliderComponent3D::IsRayIntersects(const Ray & Ray) const
{
	if (GetBounds().IsInfinite())
		return false;

	return HitBoundingBox(GetWorldBounds().getMin(), GetWorldBounds().getMax(), Ray.GetOrigin(), Ray.GetDirection());
}



//
// ISceneNodeComponent
//
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
// IObjectSaveLoad
//
void CColliderComponent3D::CopyTo(std::shared_ptr<IObject> Destination) const
{
	CComponentBase::CopyTo(Destination);

	auto destCast = std::dynamic_pointer_cast<CColliderComponent3D>(Destination);

	destCast->SetCullStrategy(GetCullStrategy());
	destCast->SetCullDistance(GetCullDistance());

	if (false == m_Bounds.IsInfinite())
		destCast->SetBounds(m_Bounds);

	destCast->SetDebugDrawMode(m_DebugDraw);
}

void CColliderComponent3D::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	CComponentBase::Load(Reader);

	BoundingBox bbox;
	bbox.Load(Reader);
	if (false == bbox.IsInfinite())
		SetBounds(bbox);
}

void CColliderComponent3D::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	CComponentBase::Save(Writer);

	GetBounds().Save(Writer);
}



//
// Protected
//
void CColliderComponent3D::UpdateBounds()
{
	if (GetBounds().IsInfinite())
		return;

	BoundingBox bounds = GetBounds();
	bounds.transform(GetOwnerNode().GetWorldTransfom());
	m_WorldBounds = bounds;
}

void CColliderComponent3D::SetMinBounds(const glm::vec3& Min)
{
	m_Bounds.setMin(Min);
}

glm::vec3 CColliderComponent3D::GetMinBounds() const
{
	return m_Bounds.getMin();
}

void CColliderComponent3D::SetMaxBounds(const glm::vec3 & Max)
{
	m_Bounds.setMax(Max);
}

glm::vec3 CColliderComponent3D::GetMaxBounds() const
{
	return m_Bounds.getMax();
}
