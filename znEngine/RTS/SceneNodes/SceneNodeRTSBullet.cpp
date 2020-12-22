#include "stdafx.h"

// General
#include "SceneNodeRTSBullet.h"

// Additional
#include "Materials/MaterialModel.h"

#include "Scene/Components/LightComponent/Light.h"

#include "Scene/Components/Particles/ParticlesComponent.h"
#include "Scene/Components/Particles/ParticleSystem.h"

CSceneNodeRTSBullet::CSceneNodeRTSBullet(IScene & Scene)
	: CSceneNode(Scene)

	, m_Damage(10.0f)
	, m_Speed(60.0f)

	, m_Target(std::weak_ptr<ISceneNodeRTSUnit>())
	, m_TargetLastPosition(0.0f)
{}

CSceneNodeRTSBullet::~CSceneNodeRTSBullet()
{}



//
// ISceneNodeRTSBullet
//
void CSceneNodeRTSBullet::SetTarget(std::shared_ptr<ISceneNodeRTSUnit> Target)
{
	m_Target = Target;

	// TODO: Move me
	{
		std::shared_ptr<IParticleComponent3D> particles = MakeShared(CParticlesComponent, *this);
		AddComponentT(particles);

		m_ParticleSystem = MakeShared(CParticleSystem, GetBaseManager());
		m_ParticleSystem->SetNode(this);
		m_ParticleSystem->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("star_09.png"));

		//std::dynamic_pointer_cast<CParticleSystem>(m_ParticleSystem)->m_Lifetime = glm::length(Target->GetTranslationAbs().xz - GetTranslationAbs().xz) / GetSpeed() * 60.0f;

		particles->AddParticleSystem(m_ParticleSystem);
	}
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

		GetComponentT<IModelComponent>()->SetModel(bulletModel);
	}*/

	{
		AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ILightComponent3D>(cSceneNodeLightComponent, *this));
		GetComponentT<ILightComponent3D>()->SetLight(MakeShared(CLight, GetBaseManager()));
		GetComponentT<ILightComponent3D>()->GetLight()->SetType(ELightType::Point);
		GetComponentT<ILightComponent3D>()->GetLight()->SetAmbientColor(ColorRGB(0.0f));
		GetComponentT<ILightComponent3D>()->GetLight()->SetColor(ColorRGB(1.0f, 0.8f, 0.4f));
		GetComponentT<ILightComponent3D>()->GetLight()->SetRange(25.0f);
		GetComponentT<ILightComponent3D>()->GetLight()->SetIntensity(1.0f);
	}
}

void CSceneNodeRTSBullet::Update(const UpdateEventArgs & e)
{
	__super::Update(e);
	
	glm::vec3 destinationPoint = GetDestinationPoint();
	
	glm::vec3 direction = glm::normalize(destinationPoint - GetPosition());

	float speedMult = (GetSpeed() / 60.0f) * float(e.DeltaTimeMultiplier);

	glm::vec3 newPosition = GetPosition();
	newPosition += direction * speedMult;
	SetPosition(newPosition);

	if (glm::distance(GetPosition(), destinationPoint) < speedMult * 2.0f)
	{
		if (auto target = GetTarget())
			target->DealDamage(GetDamage());
		
		// After we reach position, deal damage
		MakeMeOrphan();

		m_ParticleSystem->SetEnableCreatingNewParticles(false);

		GetComponentT<IParticleComponent3D>()->RemoveParticleSystem(m_ParticleSystem);

		GetScene().GetRootSceneNode()->GetComponentT<IParticleComponent3D>()->AddParticleSystem(m_ParticleSystem);
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

	m_TargetLastPosition = target->GetPosition();
	if (auto collider = target->GetComponentT<IColliderComponent>())
	{
		const auto& worldBounds = collider->GetWorldBounds();
		if (false == worldBounds.IsInfinite())
			m_TargetLastPosition = worldBounds.getCenter();
	}

	return m_TargetLastPosition + glm::vec3(0.0f, 2.0f, 0.0f);
}
