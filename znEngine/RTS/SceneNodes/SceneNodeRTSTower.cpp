#include "stdafx.h"

// General
#include "SceneNodeRTSTower.h"

// Additional
#include "Materials/MaterialModel.h"
#include "Scene/Components/Particles/ParticlesComponent.h"


CSceneNodeRTSTower::CSceneNodeRTSTower(IScene & Scene)
	: CSceneNode(Scene)
	, m_AttackDamage(1.0f)
	, m_AttackInterval(666.0f)
	, m_AttackRange(50.0f)

	, m_LastAttackTime(0.0f)
{
	// Unit properties
	{
		auto towerPropsGroup = MakeShared(CPropertiesGroup, "Tower", "descr");

		auto AttackDamage = MakeShared(CPropertyWrapped<float>, "AttackDamage", "descr", 10.0f);
		AttackDamage->SetValueSetter(std::bind(&CSceneNodeRTSTower::SetAttackDamage, this, std::placeholders::_1));
		AttackDamage->SetValueGetter(std::bind(&CSceneNodeRTSTower::GetAttackDamage, this));
		towerPropsGroup->AddProperty(AttackDamage);

		auto AttackInterval = MakeShared(CPropertyWrapped<float>, "AttackInterval", "descr", 1.0f);
		AttackInterval->SetValueSetter(std::bind(&CSceneNodeRTSTower::SetAttackInterval, this, std::placeholders::_1));
		AttackInterval->SetValueGetter(std::bind(&CSceneNodeRTSTower::GetAttackInterval, this));
		towerPropsGroup->AddProperty(AttackInterval);

		auto AttackRange = MakeShared(CPropertyWrapped<float>, "AttackRange", "descr", 250.0f);
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
void CSceneNodeRTSTower::SetAttackDamage(float Value)
{
	m_AttackDamage = Value;
}

float CSceneNodeRTSTower::GetAttackDamage() const
{
	return m_AttackDamage;
}

void CSceneNodeRTSTower::SetAttackInterval(float Value)
{
	m_AttackInterval = Value;
}

float CSceneNodeRTSTower::GetAttackInterval() const
{
	return m_AttackInterval;
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

	if (m_LastAttackTime + m_AttackInterval > e.TotalTime)
		return;

	auto currentTarget = GetCurrentTarget();
	if (currentTarget == nullptr)
		return;

	auto bullet = GetScene().CreateSceneNodeCast<ISceneNodeRTSBullet>(cSceneNodeRTSBullet);
	bullet->SetPosition(GetPosition());
	bullet->SetTarget(currentTarget);

	m_LastAttackTime = e.TotalTime;
}

std::shared_ptr<ISceneNodeRTSUnit> CSceneNodeRTSTower::GetCurrentTarget()
{
	if (m_CurrentTarget != nullptr)
	{
		glm::vec3 nodeTranslate = m_CurrentTarget->GetPosition();
		if (auto colliderComponent = m_CurrentTarget->GetComponentT<IColliderComponent3D>())
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



