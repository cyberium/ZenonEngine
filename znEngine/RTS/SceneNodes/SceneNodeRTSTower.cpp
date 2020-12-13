#include "stdafx.h"

// General
#include "SceneNodeRTSTower.h"

// Additional
#include "Materials/MaterialModel.h"
#include "Scene/Components/Particles/ParticlesComponent.h"

namespace
{
	const float cAttackDamageMinDefault = 2.0f;
	const float cAttackDamageMaxDefault = 4.0f;
	const float cAttackSpeedDefault = 60.0f;
	const float cAttackRangeDefault = 50.0f;
}

CSceneNodeRTSTower::CSceneNodeRTSTower(IScene & Scene)
	: CSceneNode(Scene)
	, m_AttackDamageMin(cAttackDamageMinDefault)
	, m_AttackDamageMax(cAttackDamageMaxDefault)
	, m_AttackSpeed(cAttackSpeedDefault)
	, m_AttackRange(cAttackRangeDefault)

	, m_LastAttackTime(0.0f)
{
	// Unit properties
	{
		auto towerPropsGroup = MakeShared(CPropertiesGroup, "Tower", "descr");

		auto attackDamageMin = MakeShared(CPropertyWrapped<float>, "AttackDamageMin", "descr", cAttackDamageMinDefault);
		attackDamageMin->SetValueSetter(std::bind(&CSceneNodeRTSTower::SetAttackDamageMin, this, std::placeholders::_1));
		attackDamageMin->SetValueGetter(std::bind(&CSceneNodeRTSTower::GetAttackDamageMin, this));
		towerPropsGroup->AddProperty(attackDamageMin);

		auto attackDamageMax = MakeShared(CPropertyWrapped<float>, "AttackDamageMax", "descr", cAttackDamageMaxDefault);
		attackDamageMax->SetValueSetter(std::bind(&CSceneNodeRTSTower::SetAttackDamageMax, this, std::placeholders::_1));
		attackDamageMax->SetValueGetter(std::bind(&CSceneNodeRTSTower::GetAttackDamageMax, this));
		towerPropsGroup->AddProperty(attackDamageMax);

		auto AttackInterval = MakeShared(CPropertyWrapped<float>, "AttackSpeed", "descr", cAttackSpeedDefault);
		AttackInterval->SetValueSetter(std::bind(&CSceneNodeRTSTower::SetAttackSpeed, this, std::placeholders::_1));
		AttackInterval->SetValueGetter(std::bind(&CSceneNodeRTSTower::GetAttackSpeed, this));
		towerPropsGroup->AddProperty(AttackInterval);

		auto AttackRange = MakeShared(CPropertyWrapped<float>, "AttackRange", "descr", cAttackRangeDefault);
		AttackRange->SetValueSetter(std::bind(&CSceneNodeRTSTower::SetAttackRange, this, std::placeholders::_1));
		AttackRange->SetValueGetter(std::bind(&CSceneNodeRTSTower::GetAttackRange, this));
		towerPropsGroup->AddProperty(AttackRange);

		GetProperties()->AddProperty(towerPropsGroup);
	}
}

CSceneNodeRTSTower::~CSceneNodeRTSTower()
{}



//
// ISceneNodeRTSTower 
//
void CSceneNodeRTSTower::SetAttackDamageMin(float AttackDamageMin)
{
	m_AttackDamageMin = AttackDamageMin;
}

float CSceneNodeRTSTower::GetAttackDamageMin() const
{
	return m_AttackDamageMin;
}

void CSceneNodeRTSTower::SetAttackDamageMax(float AttackDamageMax)
{
	m_AttackDamageMax = AttackDamageMax;
}

float CSceneNodeRTSTower::GetAttackDamageMax() const
{
	return m_AttackDamageMax;
}

void CSceneNodeRTSTower::SetAttackSpeed(float AttackSpeed)
{
	m_AttackSpeed = AttackSpeed;
}

float CSceneNodeRTSTower::GetAttackSpeed() const
{
	return m_AttackSpeed;
}

void CSceneNodeRTSTower::SetAttackRange(float Value)
{
	m_AttackRange = Value;
}

float CSceneNodeRTSTower::GetAttackRange() const
{
	return m_AttackRange;
}



//
// ISceneNode
//
void CSceneNodeRTSTower::Initialize()
{
	__super::Initialize();
}

void CSceneNodeRTSTower::Update(const UpdateEventArgs & e)
{
	__super::Update(e);

	if (m_LastAttackTime + GetAttackIntervalMS() > e.TotalTime)
		return;

	auto currentTarget = GetCurrentTarget();
	if (currentTarget == nullptr)
		return;

	auto bullet = GetScene().CreateSceneNodeCast<ISceneNodeRTSBullet>(cSceneNodeRTSBullet);
	bullet->SetPosition(GetPosition());
	bullet->SetTarget(currentTarget);

	m_LastAttackTime = e.TotalTime;
}



//
// Protected
//
float CSceneNodeRTSTower::GetAttackIntervalMS() const
{
	return 60.0f / GetAttackSpeed() * 1000.0f;
}

std::shared_ptr<ISceneNodeRTSUnit> CSceneNodeRTSTower::GetCurrentTarget()
{
	if (m_CurrentTarget != nullptr)
	{
		glm::vec3 nodeTranslate = m_CurrentTarget->GetPosition();
		if (auto colliderComponent = m_CurrentTarget->GetComponentT<IColliderComponent>())
		{
			const auto& worldBounds = colliderComponent->GetWorldBounds();
			if (false == worldBounds.IsInfinite())
				nodeTranslate = worldBounds.getCenter();
		}

		// Dead targets - is not targets
		if (m_CurrentTarget->IsDead())
			m_CurrentTarget = nullptr;

		// Target out of attack range
		else if (glm::length(GetPosition().xz - nodeTranslate.xz) > GetAttackRange())
			m_CurrentTarget = nullptr;
	}

	if (m_CurrentTarget != nullptr)
		return m_CurrentTarget;

	// Find nodes in attack range
	auto nodes = GetScene().GetFinder().FindNearestNodes(GetPosition(), GetAttackRange(), [](const std::shared_ptr<ISceneNode>& Node) -> bool {
		if (Node->GetClass() == cSceneNodeRTSUnit)
			if (auto rtsUnit = std::dynamic_pointer_cast<ISceneNodeRTSUnit>(Node))
				return false == rtsUnit->IsDead();
		return false;
	});

	// First node - is our result
	if (false == nodes.empty())
	{
		auto firstNodeIt = nodes.begin();
		_ASSERT(firstNodeIt->first < GetAttackRange());

		m_CurrentTarget = std::dynamic_pointer_cast<ISceneNodeRTSUnit>(firstNodeIt->second);
	}

	return m_CurrentTarget;
}



