#include "stdafx.h"

// General
#include "SceneNodeRTSBullet.h"

// Additional
#include "Materials/MaterialModel.h"

#include "Scene/Components/LightComponent3D.h"
#include "Scene/Components/Particles/ParticlesComponent.h"


CSceneNodeRTSBullet::CSceneNodeRTSBullet(IScene & Scene)
	: CSceneNode(Scene)

	, m_TargetLastPosition(0.0f)

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

std::shared_ptr<ISceneNodeRTSUnit> CSceneNodeRTSBullet::GetTarget() const
{
	if (auto target = m_Target.lock())
	{
		if (false == target->IsDead())
		{
			return target;
		}
	}
	return nullptr;
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

	// Model
	/*{
		auto missileGeom = GetRenderDevice().GetPrimitivesFactory().CreateSphere();

		std::shared_ptr<MaterialModel> missileMaterial = MakeShared(MaterialModel, GetBaseManager());
		missileMaterial->SetDiffuseColor(glm::vec3(1.0f, 0.6f, 0.4f));

		auto bulletModel = GetRenderDevice().GetObjectsFactory().CreateModel();
		bulletModel->AddConnection(missileMaterial, missileGeom);

		GetComponentT<IModelsComponent3D>()->SetModel(bulletModel);
	}*/

	{
		AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *this));
		GetComponentT<ILightComponent3D>()->SetType(ELightType::Point);
		GetComponentT<ILightComponent3D>()->SetAmbientColor(glm::vec3(0.0f));
		GetComponentT<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 0.7f, 0.5f));
		GetComponentT<ILightComponent3D>()->SetRange(150.0f);
		GetComponentT<ILightComponent3D>()->SetIntensity(1.0f);
	}

	// Particles
	{
		std::shared_ptr<IParticleComponent3D> particles = MakeShared(CParticlesComponent, *this);
		AddComponentT(particles);
	}
}

void CSceneNodeRTSBullet::Update(const UpdateEventArgs & e)
{
	__super::Update(e);
	
	glm::vec3 destinationPoint = GetDestinationPoint();
	
	glm::vec3 direction = glm::normalize(destinationPoint - GetTranslationAbs());

	float speedMult = GetSpeed() * float(e.DeltaTimeMultiplier);

	glm::vec3 newPosition = GetTranslationAbs();
	newPosition += direction * speedMult;
	SetTranslate(newPosition);

	if (glm::distance(GetTranslationAbs(), destinationPoint) < speedMult * 2.0f)
	{
		if (auto target = GetTarget())
			target->DealDamage(GetDamage());
		
		// After we reach position, deal damage
		MakeMeOrphan();
	}
}

glm::vec3 CSceneNodeRTSBullet::GetDestinationPoint()
{
	auto target = GetTarget();
	if (target == nullptr)
		return m_TargetLastPosition;

	if (target->IsDead())
	{
		m_Target.reset();
		return m_TargetLastPosition;
	}

	m_TargetLastPosition = target->GetTranslationAbs();
	if (auto collider = target->GetComponentT<IColliderComponent3D>())
	{
		const auto& worldBounds = collider->GetWorldBounds();
		if (false == worldBounds.IsInfinite())
			m_TargetLastPosition = worldBounds.getCenter();
	}

	return m_TargetLastPosition;
}
