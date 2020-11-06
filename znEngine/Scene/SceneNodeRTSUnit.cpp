#include "stdafx.h"

// General
#include "SceneNodeRTSUnit.h"

CSceneNodeRTSUnit::CSceneNodeRTSUnit(IScene & Scene)
	: CSceneNode(Scene)
	, m_PathCurrentPoint(0)
{

}

CSceneNodeRTSUnit::~CSceneNodeRTSUnit()
{
}



//
// ISceneNodeRTSUnit 
//
void CSceneNodeRTSUnit::SetPath(std::shared_ptr<ISceneNodeRTSPath> Path)
{
	m_Path = Path;
	SetTranslate(Path->GetPoints()[0]->GetTranslation());
}



//
// ISceneNode
//
void CSceneNodeRTSUnit::Initialize()
{
	SetScale(glm::vec3(2.0f));

	auto model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("models/cactus_tall.znmdl");
	GetComponentT<IModelsComponent3D>()->SetModel(model);
}

void CSceneNodeRTSUnit::Update(const UpdateEventArgs & e)
{
	float fpsMultiplier = (e.DeltaTime) / (1000.0f / 60.0f);

	auto nextPoint = m_Path->GetPoint(m_PathCurrentPoint + 1);
	if (nextPoint == nullptr)
	{
		SetTranslate(m_Path->GetPoint(0)->GetTranslation());
		m_PathCurrentPoint = 0;
		return;
	}

	glm::vec3 direction = glm::normalize(nextPoint->GetTranslation() - GetTranslation());

	glm::vec3 myPos = GetTranslation();
	myPos += direction * 0.15f * fpsMultiplier;

	SetTranslate(myPos);
	SetRotationQuaternion(glm::conjugate(glm::toQuat(glm::lookAt(GetTranslation(), GetTranslation() + direction, glm::vec3(0.0f, 1.0f, 0.0f)))));

	if (glm::distance(GetTranslation(), nextPoint->GetTranslation()) < 0.3f)
		m_PathCurrentPoint++;

	__super::Update(e);
}
