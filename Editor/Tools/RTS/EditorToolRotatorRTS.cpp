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
	m_RotatorRoot->SetName("RotatorRTS");

	auto geom = GetRenderDevice().GetPrimitivesFactory().CreateTorus(1.0f, 0.05f);

	auto materialY = MakeShared(MaterialDebug, GetRenderDevice());
	materialY->SetDiffuseColor(glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

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

	auto btn = dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorRTSBtn;
	btn->setChecked(IsEnabled());

	if (m_MovingNode = GetEditor().GetFirstSelectedNode())
	{
		m_RotatorRoot->SetTranslate(m_MovingNode->GetTranslation());
		m_RotatorRoot->SetScale(glm::vec3(m_MovingNode->GetColliderComponent()->GetBounds().getRadius() * 2.0f));
	}
}

void CEditorToolRotatorRTS::Disable()
{
	CEditorToolBase::Disable();

	auto btn = dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorRTSBtn;
	btn->setChecked(IsEnabled());

	m_RotatorRoot->SetTranslate(glm::vec3(-1000000.0));

	Clear();
	m_MovingNode.reset();
}

bool CEditorToolRotatorRTS::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_MovingNode == nullptr)
		return false;

	if (IsChildOf(m_RotatorRoot, m_MovingNode))
		return false;

	auto nodes = GetScene()->GetFinder().FindIntersection(RayToWorld, nullptr, m_RotatorRoot);
	if (nodes.empty())
		return false;


	for (const auto& it : nodes)
	{
		if (it.second == m_RotatorY)
			m_RotatorNuber = 2;
	}

	if (m_RotatorNuber > 0)
	{
		auto nodePosition = m_MovingNode->GetTranslation();
		auto pos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), nodePosition.y));

		m_RotatorInitialAngle = m_MovingNode->GetRotation().y;//= glm::angle(glm::normalize(pos.xz()), glm::normalize(nodePosition.xz()));

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
		if (m_RotatorNuber == 2)
		{
			auto nodePosition = m_MovingNode->GetTranslation();
			auto pos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), nodePosition.y));

			float ang = glm::angle(glm::normalize(pos.xz()), glm::normalize(nodePosition.xz()));

			newRot.y = m_RotatorInitialAngle + ang;
		}

		m_MovingNode->SetRotation(newRot);
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



//
// Protected
//
void CEditorToolRotatorRTS::Clear()
{
	m_RotatorNuber = 0;
	m_IsRotateNow = false;
}
