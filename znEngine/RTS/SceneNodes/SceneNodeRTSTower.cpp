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
		auto towerPropsGroup = MakeShared(CPropertiesGroup, "Tower");

		auto AttackDamage = MakeShared(CPropertyWrapped<float>, "AttackDamage");
		AttackDamage->SetValueSetter(std::bind(&CSceneNodeRTSTower::SetAttackDamage, this, std::placeholders::_1));
		AttackDamage->SetValueGetter(std::bind(&CSceneNodeRTSTower::GetAttackDamage, this));
		towerPropsGroup->AddProperty(AttackDamage);

		auto AttackInterval = MakeShared(CPropertyWrapped<float>, "AttackInterval");
		AttackInterval->SetValueSetter(std::bind(&CSceneNodeRTSTower::SetAttackInterval, this, std::placeholders::_1));
		AttackInterval->SetValueGetter(std::bind(&CSceneNodeRTSTower::GetAttackInterval, this));
		towerPropsGroup->AddProperty(AttackInterval);

		auto AttackRange = MakeShared(CPropertyWrapped<float>, "AttackRange");
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
	auto nodes = GetScene().GetFinder().FindNearestNodes(sourcePoint, GetAttackRange(), [](const std::shared_ptr<ISceneNode>& Node) -> bool {
		if (Node->GetClass() == cSceneNodeRTSUnit)
			if (auto rtsUnit = std::dynamic_pointer_cast<ISceneNodeRTSUnit>(Node))
				return false == rtsUnit->IsDead();
		return false;
	});
	if (nodes.empty())
		return;

	auto firstNodeIt = nodes.begin();
	auto nearestNode = firstNodeIt->second;
	_ASSERT(firstNodeIt->first < GetAttackRange());

	auto bullet = GetScene().CreateSceneNodeCast<ISceneNodeRTSBullet>(cSceneNodeRTSBullet);
	bullet->SetTranslate(GetParent()->GetWorldTransfom() * glm::vec4(GetTranslation(), 1.0f));
	bullet->SetTarget(std::dynamic_pointer_cast<ISceneNodeRTSUnit>(nearestNode));
	bullet->GetComponentT<IModelsComponent3D>()->SetModel(m_MissileModel);

	
	{
		std::shared_ptr<IParticleComponent3D> particles = MakeShared(CParticlesComponent, *bullet);
		bullet->AddComponentT(particles);
	}

	m_LastAttackTime = e.TotalTime;
}



