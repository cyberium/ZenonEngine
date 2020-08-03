#include "stdafx.h"

// General
#include "Editor3DToolRotatorRTS.h"

CEditor3DToolRotatorRTS::CEditor3DToolRotatorRTS(IEditor& Editor)
	: CEditor3DToolBase(Editor)
	, m_RotatorNuber(-1)
	, m_IsMoverEnable(false)
{
}

CEditor3DToolRotatorRTS::~CEditor3DToolRotatorRTS()
{
}

void CEditor3DToolRotatorRTS::Initialize()
{
	m_RotatorRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), GetScene()->GetRootNode3D());
	m_RotatorRoot->SetName("Rotator");

	auto geom = GetRenderDevice().GetPrimitivesFactory().CreateTorus(1.0f, 0.05f);

	auto materialX = std::make_shared<MaterialDebug>(GetRenderDevice());
	materialX->SetDiffuseColor(glm::vec4(1.0f, 0.2f, 0.1f, 1.0f));
	auto modelX = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelX->AddConnection(materialX, geom);

	auto materialY = std::make_shared<MaterialDebug>(GetRenderDevice());
	materialY->SetDiffuseColor(glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

	auto materialZ = std::make_shared<MaterialDebug>(GetRenderDevice());
	materialZ->SetDiffuseColor(glm::vec4(0.1f, 0.2f, 1.0f, 1.0f));
	auto modelZ = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelZ->AddConnection(materialZ, geom);


	m_RotatorX = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorX->SetName("Rotator_X");
	m_RotatorX->SetRotation(glm::vec3(0.0f, 0.0f, glm::half_pi<float>()));
	m_RotatorX->GetModelsComponent()->SetModel(modelX);
	m_RotatorX->GetColliderComponent()->SetBounds(geom->GetBounds());

	m_RotatorY = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorY->SetName("Rotator_X");
	m_RotatorY->GetModelsComponent()->SetModel(modelY);
	m_RotatorY->GetColliderComponent()->SetBounds(geom->GetBounds());

	m_RotatorZ = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorZ->SetName("Rotator_X");
	m_RotatorZ->SetRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
	m_RotatorZ->GetModelsComponent()->SetModel(modelZ);
	m_RotatorZ->GetColliderComponent()->SetBounds(geom->GetBounds());
}

void CEditor3DToolRotatorRTS::Finalize()
{
}

void CEditor3DToolRotatorRTS::Enable()
{
	CEditor3DToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorBtn->setChecked(IsEnabled());

	if (auto node = GetEditor().GetFirstSelectedNode())
	{
		m_MovingNode = node;
		m_RotatorRoot->SetTranslate(node->GetTranslation());
		m_RotatorRoot->SetScale(glm::vec3(m_MovingNode->GetColliderComponent()->GetBounds().getRadius()));
	}
}

void CEditor3DToolRotatorRTS::Disable()
{
	CEditor3DToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorBtn->setChecked(IsEnabled());

	m_RotatorRoot->SetTranslate(glm::vec3(-1000000.0, -10000000.0f, -10000000.0f));

	Clear();
}

bool CEditor3DToolRotatorRTS::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (!IsEnabled())
		return false;

	auto nodes = GetScene()->FindIntersection(RayToWorld, m_RotatorRoot);
	if (nodes.empty())
		return false;

	if (m_MovingNode != nullptr && !IsChildOf(m_RotatorRoot, m_MovingNode))
	{
		for (const auto& it : nodes)
		{
			if (it.second == m_RotatorX)
				m_RotatorNuber = 1;
			else if (it.second == m_RotatorY)
				m_RotatorNuber = 2;
			else if (it.second == m_RotatorZ)
				m_RotatorNuber = 3;
		}

		if (m_RotatorNuber > 0)
		{
			m_MovingObjectPos = m_MovingNode->GetRotation();
			m_IsMoverEnable = true;
			return true;
		}
	}

	return false;
}

void CEditor3DToolRotatorRTS::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (!IsEnabled())
		return;

	if (m_IsMoverEnable)
		Clear();
}

void CEditor3DToolRotatorRTS::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (!IsEnabled())
		return;

	if (m_IsMoverEnable)
	{
		glm::vec3 newRot = m_MovingNode->GetRotation();

		if (m_RotatorNuber == 1)
		{
			newRot.x += e.RelY / 360.0f;
		}
		else if (m_RotatorNuber == 2)
		{
			newRot.y += e.RelY / 360.0f;
		}
		else if (m_RotatorNuber == 3)
		{
			newRot.z += e.RelY / 360.0f;
		}

		m_MovingNode->SetRotation(newRot);

		return;
	}
}

void CEditor3DToolRotatorRTS::Clear()
{
	m_RotatorNuber = 0;
	m_IsMoverEnable = false;
	m_MovingObjectPos = glm::vec3(0.0f);
}
