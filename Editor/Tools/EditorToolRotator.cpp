#include "stdafx.h"

// General
#include "EditorToolRotator.h"

// Additional
#include "Materials/MaterialEditorTool.h"

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

	auto materialX = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialX->SetColor(glm::vec4(1.0f, 0.2f, 0.1f, 1.0f));
	auto modelX = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelX->AddConnection(materialX, geom);

	auto materialY = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialY->SetColor(glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

	auto materialZ = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialZ->SetColor(glm::vec4(0.1f, 0.2f, 1.0f, 1.0f));
	auto modelZ = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelZ->AddConnection(materialZ, geom);


	m_RotatorX = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorX->SetName("Rotator_X");
	m_RotatorX->SetRotation(glm::vec3(0.0f, 0.0f, glm::half_pi<float>()));
	m_RotatorX->GetComponent<IModelsComponent3D>()->SetModel(modelX);
	m_RotatorX->GetComponent<IColliderComponent3D>()->SetBounds(geom->GetBounds());

	m_RotatorY = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorY->SetName("Rotator_Y");
	m_RotatorY->GetComponent<IModelsComponent3D>()->SetModel(modelY);
	m_RotatorY->GetComponent<IColliderComponent3D>()->SetBounds(geom->GetBounds());

	m_RotatorZ = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorZ->SetName("Rotator_Z");
	m_RotatorZ->SetRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
	m_RotatorZ->GetComponent<IModelsComponent3D>()->SetModel(modelZ);
	m_RotatorZ->GetComponent<IColliderComponent3D>()->SetBounds(geom->GetBounds());
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
		m_RotatorRoot->SetScale(glm::vec3(node->GetComponent<IColliderComponent3D>()->GetBounds().getRadius()));
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
	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
		return false;

	if (rotatingNode != nullptr && !IsChildOf(m_RotatorRoot, rotatingNode))
	{	
		auto nodes = GetScene()->GetFinder().FindIntersection(RayToWorld, nullptr, m_RotatorRoot);
		if (nodes.empty())
			return false;

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
			m_MovingObjectPos = rotatingNode->GetRotation();
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
	if (false == m_IsRotateNow)
		return;

	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
		return;

	glm::vec3 newRot = rotatingNode->GetRotation();

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

	rotatingNode->SetRotation(newRot);
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

std::shared_ptr<ISceneNode3D> CEditorToolRotator::GetRotatingNode()
{
	return m_MovingNode.lock();
}
