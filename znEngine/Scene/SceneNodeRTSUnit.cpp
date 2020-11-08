#include "stdafx.h"

// General
#include "SceneNodeRTSUnit.h"

CSceneNodeRTSUnit::CSceneNodeRTSUnit(IScene & Scene)
	: CSceneNode(Scene)
	, m_Health(75.0f)
	, m_MaxHealth(100.0f)
	, m_Speed(0.25f)
	, m_PathCurrentPoint(0)
{
	// Unit properties
	{
		auto unitPropsGroup = MakeShared(CPropertiesGroup, "Unit");

		auto health = MakeShared(CPropertyWrapped<float>, "Health");
		health->SetValueSetter(std::bind(&CSceneNodeRTSUnit::SetHealth, this, std::placeholders::_1));
		health->SetValueGetter(std::bind(&CSceneNodeRTSUnit::GetHealth, this));
		unitPropsGroup->AddProperty(health);

		auto maxHealth = MakeShared(CPropertyWrapped<float>, "MaxHealth");
		maxHealth->SetValueSetter(std::bind(&CSceneNodeRTSUnit::SetMaxHealth, this, std::placeholders::_1));
		maxHealth->SetValueGetter(std::bind(&CSceneNodeRTSUnit::GetMaxHealth, this));
		unitPropsGroup->AddProperty(maxHealth);

		auto movementSpeed = MakeShared(CPropertyWrapped<float>, "MovementSpeed");
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

void CSceneNodeRTSUnit::DealDamage(float Damage)
{
	_ASSERT(Damage > 0.0f);
	m_Health -= Damage;
	//if (m_Health < 0.0f)
	//	MakeMeOrphan();
}



//
// ISceneNode
//
void CSceneNodeRTSUnit::Initialize()
{
	__super::Initialize();

	SetScale(glm::vec3(1.0f));

	auto model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("models/cactus_tall.znmdl");
	GetComponentT<IModelsComponent3D>()->SetModel(model);
}

void CSceneNodeRTSUnit::Update(const UpdateEventArgs & e)
{
	__super::Update(e);

	float fpsMultiplier = (e.DeltaTime) / (1000.0f / 60.0f);
	if (e.DeltaTime == e.TotalTime)
		fpsMultiplier = 1.0f; // First frame

	auto nextPoint = m_Path->GetPoint(m_PathCurrentPoint + 1);
	if (nextPoint == nullptr)
	{
		SetTranslate(m_Path->GetPoint(0)->GetTranslation());
		m_PathCurrentPoint = 0;
		return;
	}

	glm::vec3 direction = glm::normalize(nextPoint->GetTranslation() - GetTranslation());

	glm::vec3 newPosition = GetTranslation();
	newPosition += direction * GetMovementSpeed() * fpsMultiplier;

	SetTranslate(newPosition);
	SetRotationQuaternion(glm::conjugate(glm::toQuat(glm::lookAt(GetTranslation(), GetTranslation() + direction, glm::vec3(0.0f, 1.0f, 0.0f)))));

	if (glm::distance(GetTranslation(), nextPoint->GetTranslation()) < GetMovementSpeed() * 2.0f)
	{
		m_PathCurrentPoint++;
	}
}
