#include "stdafx.h"

// General
#include "ColliderComponent.h"

CColliderComponent::CColliderComponent(const ISceneNode& OwnerNode)
	: CComponentBase(OwnerNode)
	, m_CullStrategy(ECullStrategy::ByFrustrum)
	, m_CullDistance(99999.0f) // Don't use FloatMax
	, m_DebugDraw(true)
{
	GetProperties()->SetName("ColliderComponent");

	{
		auto minBounds = MakeShared(CPropertyWrappedVec3, "BBoxMin", "descr", glm::vec3(-10.0f));
		minBounds->SetValueSetter(std::bind(&CColliderComponent::SetMinBounds, this, std::placeholders::_1));
		minBounds->SetValueGetter(std::bind(&CColliderComponent::GetMinBounds, this));
		GetProperties()->AddProperty(minBounds);

		auto maxBounds = MakeShared(CPropertyWrappedVec3, "BBoxMax", "descr", glm::vec3(10.0f));
		maxBounds->SetValueSetter(std::bind(&CColliderComponent::SetMaxBounds, this, std::placeholders::_1));
		maxBounds->SetValueGetter(std::bind(&CColliderComponent::GetMaxBounds, this));
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

CColliderComponent::~CColliderComponent()
{
}



//
// IColliderComponent
//

void CColliderComponent::SetBounds(BoundingBox Bounds)
{
	if (Bounds.IsInfinite())
		throw CException("Unable to set infinity bounds to node '%s'.", GetOwnerNode().GetName().c_str());

	m_Bounds = Bounds;

	RaiseComponentMessage(UUID_OnBoundsChanget);
}

void CColliderComponent::ExtendBounds(BoundingBox Bounds)
{
	if (Bounds.IsInfinite())
		throw CException("Unable to extend infinity bounds to node '%s'.", GetOwnerNode().GetName().c_str());

	if (m_Bounds.IsInfinite())
		m_Bounds = Bounds;
	else
		m_Bounds.makeUnion(Bounds);

	RaiseComponentMessage(UUID_OnBoundsChanget);
}

const BoundingBox& CColliderComponent::GetBounds() const
{
	return m_Bounds;
}

const BoundingBox& CColliderComponent::GetWorldBounds() const
{
	return m_WorldBounds;
}

void CColliderComponent::SetCullStrategy(ECullStrategy CullStrategy)
{
	m_CullStrategy = CullStrategy;
}

IColliderComponent::ECullStrategy CColliderComponent::GetCullStrategy() const
{
	return m_CullStrategy;
}

void CColliderComponent::SetCullDistance(float Distance)
{
	m_CullDistance = Distance;
}

float CColliderComponent::GetCullDistance() const
{
	return m_CullDistance;
}

void CColliderComponent::SetDebugDrawMode(bool Value)
{
	m_DebugDraw = Value;
}

bool CColliderComponent::GetDebugDrawMode() const
{
	return m_DebugDraw;
}

bool CColliderComponent::IsCulled(const ICameraComponent3D * Camera) const
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

bool CColliderComponent::IsCulledByFrustum(const ICameraComponent3D* Camera) const
{
	_ASSERT(Camera != nullptr);

	if (GetBounds().IsInfinite())
		return false;

	return Camera->GetFrustum().cullBox(GetWorldBounds());
}

bool CColliderComponent::IsCulledByDistance2D(const ICameraComponent3D* Camera) const
{
	_ASSERT(Camera != nullptr);

	if (GetBounds().IsInfinite())
		return false;

	const glm::vec3 cameraPosition = Camera->GetTranslation();
	const float distToCamera2D = glm::length(Fix_X0Z(cameraPosition) - Fix_X0Z(GetWorldBounds().getCenter())) - GetWorldBounds().getRadius();
	return distToCamera2D > m_CullDistance;
}

bool CColliderComponent::IsCulledByDistance(const ICameraComponent3D* Camera) const
{
	_ASSERT(Camera != nullptr);

	if (GetBounds().IsInfinite())
		return false;

	const glm::vec3 cameraPosition = Camera->GetTranslation();
	const float distToCamera = glm::length(cameraPosition - GetWorldBounds().getCenter()) - GetWorldBounds().getRadius();
	return distToCamera > m_CullDistance;
}

bool CColliderComponent::IsRayIntersects(const Ray & Ray) const
{
	if (GetBounds().IsInfinite())
		return false;

	return HitBoundingBox(GetWorldBounds().getMin(), GetWorldBounds().getMax(), Ray.GetOrigin(), Ray.GetDirection());
}



//
// ISceneNodeComponent
//
void CColliderComponent::OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message)
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
void CColliderComponent::CopyTo(std::shared_ptr<IObject> Destination) const
{
	auto destinationAsColliderComponent = std::dynamic_pointer_cast<IColliderComponent>(Destination);

	destinationAsColliderComponent->SetCullStrategy(GetCullStrategy());
	destinationAsColliderComponent->SetCullDistance(GetCullDistance());

	if (false == GetBounds().IsInfinite())
		destinationAsColliderComponent->SetBounds(GetBounds());

	destinationAsColliderComponent->SetDebugDrawMode(GetDebugDrawMode());
}

void CColliderComponent::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	BoundingBox bbox;
	bbox.Load(Reader);
	if (false == bbox.IsInfinite())
		SetBounds(bbox);
}

void CColliderComponent::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	GetBounds().Save(Writer);
}



//
// Protected
//
void CColliderComponent::UpdateBounds()
{
	if (GetBounds().IsInfinite())
		return;

	BoundingBox bounds = GetBounds();
	bounds.transform(GetOwnerNode().GetWorldTransfom());
	m_WorldBounds = bounds;
}

void CColliderComponent::SetMinBounds(const glm::vec3& Min)
{
	m_Bounds.setMin(Min);
}

glm::vec3 CColliderComponent::GetMinBounds() const
{
	return m_Bounds.getMin();
}

void CColliderComponent::SetMaxBounds(const glm::vec3 & Max)
{
	m_Bounds.setMax(Max);
}

glm::vec3 CColliderComponent::GetMaxBounds() const
{
	return m_Bounds.getMax();
}
