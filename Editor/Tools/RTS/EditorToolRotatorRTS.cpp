#include "stdafx.h"

// General
#include "EditorToolRotatorRTS.h"

CEditorToolRotatorRTS::CEditorToolRotatorRTS(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_RotatorNuber(-1)
	, m_IsRotateNow(false)
{
}

CEditorToolRotatorRTS::~CEditorToolRotatorRTS()
{
}

void CEditorToolRotatorRTS::Initialize()
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

	m_RotatorY = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorY->SetName("RotatorRTS_Y");
	m_RotatorY->GetModelsComponent()->SetModel(modelY);
	m_RotatorY->GetColliderComponent()->SetBounds(geom->GetBounds());
}

void CEditorToolRotatorRTS::Finalize()
{
}

void CEditorToolRotatorRTS::Enable()
{
	CEditorToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorRTSBtn->setChecked(IsEnabled());

	if (auto node = GetEditor().GetFirstSelectedNode())
	{
		m_MovingNode = node;
		m_RotatorRoot->SetTranslate(node->GetTranslation());
		m_RotatorRoot->SetScale(glm::vec3(m_MovingNode->GetColliderComponent()->GetBounds().getRadius()));
	}
}

void CEditorToolRotatorRTS::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorRTSBtn->setChecked(IsEnabled());

	m_RotatorRoot->SetTranslate(glm::vec3(-1000000.0, -10000000.0f, -10000000.0f));

	Clear();
	m_MovingNode.reset();
}

bool CEditorToolRotatorRTS::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_MovingNode == nullptr)
		return false;

	if (IsChildOf(m_RotatorRoot, m_MovingNode))
		return false;

	auto nodes = GetScene()->FindIntersection(RayToWorld, m_RotatorRoot);
	if (nodes.empty())
		return false;

	for (const auto& it : nodes)
	{
		if (it.second == m_RotatorY)
			m_RotatorNuber = 2;
	}

	if (m_RotatorNuber > 0)
	{
		m_MovingObjectPos = m_MovingNode->GetRotation();
		m_IsRotateNow = true;
		return true;
	}

	return false;
}

void CEditorToolRotatorRTS::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_IsRotateNow)
		Clear();
}

void CEditorToolRotatorRTS::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
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
void CEditorToolRotatorRTS::DoInitializeUI(IEditorQtUIFrame & QtUIFrame)
{
	auto btn = QtUIFrame.getUI().editorToolRotatorRTSBtn;
	btn->connect(btn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolRotatorRTS);
	});
}

void CEditorToolRotatorRTS::Clear()
{
	m_RotatorNuber = 0;
	m_IsRotateNow = false;
	m_MovingObjectPos = glm::vec3(0.0f);
}
