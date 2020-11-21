#include "stdafx.h"

// General
#include "SceneNodeRTSBullet.h"

// Additional
#include "Materials/MaterialModel.h"
#include "Scene/Components/Particles/ParticlesComponent.h"


CSceneNodeRTSBullet::CSceneNodeRTSBullet(IScene & Scene)
	: CSceneNode(Scene)
	, m_Damage(10.0f)
	, m_Speed(1.0f)
{}

CSceneNodeRTSBullet::~CSceneNodeRTSBullet()
{}



//
// ISceneNodeRTSBullet
//
void CSceneNodeRTSBullet::SetTarget(std::shared_ptr<ISceneNodeRTSUnit> Target)
{
	m_Target = Target;
}

std::shared_ptr<ISceneNode> CSceneNodeRTSBullet::GetTarget() const
{
	return m_Target;
}

void CSceneNodeRTSBullet::SetDamage(float Damage)
{
	m_Damage = Damage;
}

float CSceneNodeRTSBullet::GetDamage() const
{
	return m_Damage;
}

void CSceneNodeRTSBullet::SetSpeed(float Speed)
{
	m_Speed = Speed;
}

float CSceneNodeRTSBullet::GetSpeed() const
{
	return m_Speed;
}



//
// ISceneNode
//
void CSceneNodeRTSBullet::Initialize()
{
	__super::Initialize();
}

void CSceneNodeRTSBullet::Update(const UpdateEventArgs & e)
{
	__super::Update(e);

	if (m_Target == nullptr)
		MakeMeOrphan();
	
	float speedMult = GetSpeed() * float(e.DeltaTimeMultiplier);

	glm::vec3 destinationPoint = GetDestinationPoint();
	glm::vec3 direction = glm::normalize(destinationPoint - GetTranslation());

	glm::vec3 newPosition = GetTranslation();
	newPosition += direction * speedMult;
	SetTranslate(newPosition);

	if (glm::distance(GetTranslation(), destinationPoint) < speedMult * 2.0f)
	{
		m_Target->DealDamage(GetDamage());
		MakeMeOrphan();
	}
}

glm::vec3 CSceneNodeRTSBullet::GetDestinationPoint() const
{
	if (auto collider = m_Target->GetComponentT<IColliderComponent3D>())
	{
		const auto& worldBounds = collider->GetWorldBounds();
		if (false == worldBounds.IsInfinite())
			return worldBounds.getCenter();
	}
	return m_Target->GetTranslation();
}
