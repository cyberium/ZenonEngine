#include "stdafx.h"

// General
#include "SceneNodeRTSUnit.h"

// Additional
#include "Scene/Components/Particles/ParticlesComponent.h"

CSceneNodeRTSUnit::CSceneNodeRTSUnit(IScene & Scene)
	: CSceneNode(Scene)
	, m_Health(75.0f)
	, m_MaxHealth(100.0f)
	, m_Speed(0.15f)
	, m_PathCurrentPoint(0)
{
	// Unit properties
	{
		auto unitPropsGroup = MakeShared(CPropertiesGroup, "Unit", "descr");

		auto health = MakeShared(CPropertyWrapped<float>, "Health", "descr", 100.0f);
		health->SetValueSetter(std::bind(&CSceneNodeRTSUnit::SetHealth, this, std::placeholders::_1));
		health->SetValueGetter(std::bind(&CSceneNodeRTSUnit::GetHealth, this));
		unitPropsGroup->AddProperty(health);

		auto maxHealth = MakeShared(CPropertyWrapped<float>, "MaxHealth", "descr", 100.0f);
		maxHealth->SetValueSetter(std::bind(&CSceneNodeRTSUnit::SetMaxHealth, this, std::placeholders::_1));
		maxHealth->SetValueGetter(std::bind(&CSceneNodeRTSUnit::GetMaxHealth, this));
		unitPropsGroup->AddProperty(maxHealth);

		auto movementSpeed = MakeShared(CPropertyWrapped<float>, "MovementSpeed", "descr", 1.0f);
		movementSpeed->SetValueSetter(std::bind(&CSceneNodeRTSUnit::SetMovementSpeed, this, std::placeholders::_1));
		movementSpeed->SetValueGetter(std::bind(&CSceneNodeRTSUnit::GetMovementSpeed, this));
		unitPropsGroup->AddProperty(movementSpeed);

		GetProperties()->AddProperty(unitPropsGroup);
	}
}

CSceneNodeRTSUnit::~CSceneNodeRTSUnit()
{
}



//
// ISceneNodeRTSUnit 
//
void CSceneNodeRTSUnit::SetHealth(float Health)
{
	m_Health = Health;
}

float CSceneNodeRTSUnit::GetHealth() const
{
	return m_Health;
}

void CSceneNodeRTSUnit::SetMaxHealth(float MaxHealth)
{
	m_MaxHealth = MaxHealth;
}

float CSceneNodeRTSUnit::GetMaxHealth() const
{
	return m_MaxHealth;
}

void CSceneNodeRTSUnit::SetMovementSpeed(float Speed)
{
	m_Speed = Speed;
}

float CSceneNodeRTSUnit::GetMovementSpeed() const
{
	return m_Speed;
}

void CSceneNodeRTSUnit::SetPath(std::shared_ptr<ISceneNodeRTSPath> Path)
{
	m_Path = Path;
	m_PathCurrentPoint = 0;
	SetTranslate(Path->GetPoints()[0]->GetTranslation());
}

void CSceneNodeRTSUnit::SetLastPathPointReached(std::function<void(const ISceneNodeRTSUnit* Unit)> Func)
{
	_ASSERT(m_OnLastPathPointReached == nullptr);
	m_OnLastPathPointReached = Func;
}

void CSceneNodeRTSUnit::DealDamage(float Damage)
{
	if (IsDead())
		return;

	_ASSERT(Damage > 0.0f);
	m_Health -= Damage;
	if (m_Health <= 0.0f)
		OnDeath();
}

bool CSceneNodeRTSUnit::IsDead()
{
	return m_Health <= 0.0f;
}



//
// ISceneNode
//
void CSceneNodeRTSUnit::Initialize()
{
	__super::Initialize();

	//SetScale(glm::vec3(0.08f));

	//auto znMdlFile = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("OrcWithAnims.znmdl");

	//GetComponentT<IModelsComponent3D>()->SetModel(znMdlFile);
	//GetComponentT<IModelsComponent3D>()->PlayAnimation("run", true);

	//std::shared_ptr<IParticleComponent3D> particlesComponent = MakeShared(CParticlesComponent, *this);
	//AddComponentT(particlesComponent);
}

void CSceneNodeRTSUnit::Update(const UpdateEventArgs & e)
{
	__super::Update(e);

	if (IsDead())
		return;

	if (m_Path == nullptr)
		return;

	auto nextPoint = m_Path->GetPoint(m_PathCurrentPoint + 1);
	if (nextPoint == nullptr)
	{
		if (m_OnLastPathPointReached)
			m_OnLastPathPointReached(this);
		MakeMeOrphan();
		// TODO: Remove
		return;
	}

	glm::vec3 direction = glm::normalize(nextPoint->GetTranslation() - GetTranslation());

	glm::vec3 newPosition = GetTranslation();
	newPosition += direction * GetMovementSpeed() * float(e.DeltaTimeMultiplier);

	// Translations
	SetTranslate(newPosition);
	
	// Rotation
	glm::quat t = glm::quat(glm::vec3(0, glm::pi<float>(), 0));
	t *= glm::conjugate(glm::toQuat(glm::lookAt(GetTranslation(), GetTranslation() + direction, glm::vec3(0.0f, 1.0f, 0.0f))));
	SetRotationQuaternion(t);

	if (glm::distance(GetTranslation(), nextPoint->GetTranslation()) < GetMovementSpeed() * float(e.DeltaTimeMultiplier) * 2.0f)
	{
		m_PathCurrentPoint++;
	}
}



//
// Protected
//
void CSceneNodeRTSUnit::OnDeath()
{
	try
	{
		GetComponentT<IModelsComponent3D>()->PlayAnimation("death", false);
		GetComponentT<IModelsComponent3D>()->SetAnimationEndedCallback([this](const IAnimation* Animation) {
			MakeMeOrphan();
		});
	}
	catch (const CException& e)
	{
		Log::Error("Unit::OnDeath error.");
		Log::Error("--->%s", e.MessageCStr());
	}
}
