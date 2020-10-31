#include "stdafx.h"

// General
#include "EditorToolRotatorRTS.h"

// Additional
#include "Materials/MaterialEditorTool.h"

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

	auto materialY = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialY->SetColor(glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

	m_RotatorY = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorY->SetName("RotatorRTS_Y");
	m_RotatorY->GetComponent<IModelsComponent3D>()->SetModel(modelY);
	m_RotatorY->GetComponent<IColliderComponent3D>()->SetBounds(geom->GetBounds());
}

void CEditorToolRotatorRTS::Finalize()
{
}

void CEditorToolRotatorRTS::Enable()
{
	CEditorToolBase::Enable();

	auto btn = dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorRTSBtn;
	btn->setChecked(IsEnabled());

	if (auto node = GetEditor().GetFirstSelectedNode())
	{
		m_MovingNode = node;
		m_RotatorRoot->SetTranslate(node->GetTranslation());
		m_RotatorRoot->SetScale(glm::vec3(node->GetComponent<IColliderComponent3D>()->GetBounds().getRadius()));
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
	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
		return false;

	if (IsChildOf(m_RotatorRoot, rotatingNode))
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
		auto nodePosition = GetRotatingNode()->GetTranslation();
		auto pos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), nodePosition.y));

		m_RotatorInitialAngle = GetRotatingNode()->GetRotation().y;//= glm::angle(glm::normalize(pos.xz()), glm::normalize(nodePosition.xz()));
		m_LastAbs = e.Y;

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
	if (false == m_IsRotateNow)
		return;
	
	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
		return;

	float rotatorInitialAngleDegrees = glm::degrees(m_RotatorInitialAngle);
	rotatorInitialAngleDegrees += (m_LastAbs - e.Y) / 2.0f ;

	rotatorInitialAngleDegrees /= 45.0f;
	rotatorInitialAngleDegrees = glm::round(rotatorInitialAngleDegrees);
	rotatorInitialAngleDegrees *= 45.0f;

	rotatorInitialAngleDegrees = glm::radians(rotatorInitialAngleDegrees);

	rotatingNode->SetRotation(glm::vec3(0.0f, rotatorInitialAngleDegrees, 0.0f));

	// Refresh selection bounds
	dynamic_cast<IEditorToolSelector&>(GetEditor().GetTools().GetTool(ETool::EToolSelector)).SelectNode(rotatingNode);
	
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

std::shared_ptr<ISceneNode3D> CEditorToolRotatorRTS::GetRotatingNode()
{
	return m_MovingNode.lock();
}
