#include "stdafx.h"

// General
#include "EditorToolRotator.h"

CEditorToolRotator::CEditorToolRotator(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_RotatorNuber(-1)
	, m_IsRotateNow(false)
{
}

CEditorToolRotator::~CEditorToolRotator()
{
}

void CEditorToolRotator::Initialize()
{
	m_RotatorRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), GetScene()->GetRootNode3D());
	m_RotatorRoot->SetName("Rotator");

	auto geom = GetRenderDevice().GetPrimitivesFactory().CreateTorus(1.0f, 0.05f);

	auto materialX = MakeShared(MaterialDebug, GetRenderDevice());
	materialX->SetDiffuseColor(glm::vec4(1.0f, 0.2f, 0.1f, 1.0f));
	auto modelX = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelX->AddConnection(materialX, geom);

	auto materialY = MakeShared(MaterialDebug, GetRenderDevice());
	materialY->SetDiffuseColor(glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

	auto materialZ = MakeShared(MaterialDebug, GetRenderDevice());
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

void CEditorToolRotator::Finalize()
{
}

void CEditorToolRotator::Enable()
{
	CEditorToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorBtn->setChecked(IsEnabled());

	if (auto node = GetEditor().GetFirstSelectedNode())
	{
		m_MovingNode = node;
		m_RotatorRoot->SetTranslate(node->GetTranslation());
		m_RotatorRoot->SetScale(glm::vec3(m_MovingNode->GetColliderComponent()->GetBounds().getRadius()));
	}
}

void CEditorToolRotator::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorBtn->setChecked(IsEnabled());

	m_RotatorRoot->SetTranslate(glm::vec3(-1000000.0, -10000000.0f, -10000000.0f));

	Clear();
	m_MovingNode.reset();
}

bool CEditorToolRotator::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto nodes = GetScene()->GetFinder().FindIntersection(RayToWorld, nullptr, m_RotatorRoot);
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
			m_IsRotateNow = true;
			return true;
		}
	}

	return false;
}

void CEditorToolRotator::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_IsRotateNow)
		Clear();
}

void CEditorToolRotator::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (m_IsRotateNow)
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



//
// IEditorToolUI
//
void CEditorToolRotator::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
	QtUIFrame.getQObject().connect(QtUIFrame.getUI().editorToolRotatorBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolRotator);
	});
}



void CEditorToolRotator::Clear()
{
	m_RotatorNuber = 0;
	m_IsRotateNow = false;
	m_MovingObjectPos = glm::vec3(0.0f);
}
