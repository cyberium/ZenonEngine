#include "stdafx.h"

// General
#include "SceneNodeRTSTower.h"

// Additional
#include "Materials/MaterialModel.h"



//
// CSceneNodeRTSBullet
//
CSceneNodeRTSBullet::CSceneNodeRTSBullet(IScene & Scene)
	: CSceneNode(Scene)
	, m_Damage(1.0f)
	, m_Speed(1.3f)
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

	float fpsMultiplier = (e.DeltaTime) / (1000.0f / 60.0f);
	if (e.DeltaTime == e.TotalTime)
		fpsMultiplier = 1.0f; // First frame

	glm::vec3 destinationPoint = GetDestinationPoint();
	glm::vec3 direction = glm::normalize(destinationPoint - GetTranslation());

	glm::vec3 newPosition = GetTranslation();
	newPosition += direction * GetSpeed() * fpsMultiplier;
	SetTranslate(newPosition);

	if (glm::distance(GetTranslation(), destinationPoint) < GetSpeed() * 2.0f)
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




//
// CSceneNodeRTSTower
//
CSceneNodeRTSTower::CSceneNodeRTSTower(IScene & Scene)
	: CSceneNode(Scene)
	, m_AttackDamage(1.0f)
	, m_AttackInterval(666.0f)
	, m_AttackRange(50.0f)

	, m_LastAttackTime(0.0f)
{}

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

	auto geom = GetRenderDevice().GetPrimitivesFactory().CreateSphere();

	std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
	textMaterial->SetDiffuseColor(glm::vec3(1.0f, 0.6f, 0.4f));

	m_MissileModel = GetRenderDevice().GetObjectsFactory().CreateModel();
	m_MissileModel->AddConnection(textMaterial, geom);
}

void CSceneNodeRTSTower::Update(const UpdateEventArgs & e)
{
	__super::Update(e);

	if (m_LastAttackTime + m_AttackInterval > e.TotalTime)
		return;

	glm::vec3 sourcePoint = GetParent()->GetWorldTransfom() * glm::vec4(GetTranslation(), 1.0f);
	auto nodes = GetScene().GetFinder().FindNearestNodes(sourcePoint, GetAttackRange(), [](const std::shared_ptr<ISceneNode>& Node) -> bool {return Node->GetClass() == cSceneNodeRTSUnit; });
	if (nodes.empty())
		return;

	auto firstNodeIt = nodes.begin();
	auto nearestNode = firstNodeIt->second;
	_ASSERT(firstNodeIt->first < GetAttackRange());

	auto bullet = GetScene().CreateSceneNodeT<ISceneNodeRTSBullet>();
	bullet->SetTranslate(GetParent()->GetWorldTransfom() * glm::vec4(GetTranslation(), 1.0f));
	bullet->SetTarget(std::dynamic_pointer_cast<ISceneNodeRTSUnit>(nearestNode));
	bullet->GetComponentT<IModelsComponent3D>()->SetModel(m_MissileModel);

	m_LastAttackTime = e.TotalTime;
}



