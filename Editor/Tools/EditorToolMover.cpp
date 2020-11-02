#include "stdafx.h"

// General
#include "EditorToolMover.h"

// Additional
#include "Materials/MaterialEditorTool.h"

CEditorToolMover::CEditorToolMover(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_MoverValue(5.0f)
	, m_MoverNumber(EMoverDirection::None)
{
}

CEditorToolMover::~CEditorToolMover()
{
}

void CEditorToolMover::Enable()
{
	CEditorToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolMoverBtn->setChecked(IsEnabled());

	if (auto node = GetEditor().GetFirstSelectedNode())
	{
		m_MovingNode = node;
		m_MoverRoot->SetTranslate(node->GetTranslation());
		m_MoverRoot->SetScale(glm::vec3(node->GetComponent<IColliderComponent3D>()->GetBounds().getRadius() * 1.0f / 50.0f));
	}
}

void CEditorToolMover::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolMoverBtn->setChecked(IsEnabled());

	Clear();
	m_MovingNode.reset();
	m_MoverRoot->SetTranslate(glm::vec3(Math::MinFloat));
}

void CEditorToolMover::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_MoverRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), GetScene()->GetRootNode3D());
	m_MoverRoot->SetName("Mover");

	auto model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("arrow.FBX");
	auto geom = model->GetConnections().begin()->Geometry;

	auto materialX = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialX->SetColor(glm::vec4(0.8f, 0.2f, 0.1f, 1.0f));
	auto modelX = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelX->AddConnection(materialX, geom);

	auto materialY = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialY->SetColor(glm::vec4(0.1f, 0.8f, 0.1f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

	auto materialZ = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialZ->SetColor(glm::vec4(0.1f, 0.2f, 0.8f, 1.0f));
	auto modelZ = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelZ->AddConnection(materialZ, geom);

	m_MoverX = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverX->SetName("Mover_X");
	m_MoverX->SetTranslate(glm::vec3(1.0f, 0.0f, 0.0f));
	m_MoverX->SetRotation(glm::vec3(0.0f, glm::half_pi<float>(), 0.0f));
	m_MoverX->GetComponent<IModelsComponent3D>()->SetModel(modelX);
	//m_MoverX->GetComponent<IColliderComponent3D>()->SetBounds(model->GetBounds());

	m_MoverY = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverY->SetName("Mover_Y");
	m_MoverY->SetTranslate(glm::vec3(0.0f, 1.0f, 0.0f));
	m_MoverY->SetRotation(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f));
	m_MoverY->GetComponent<IModelsComponent3D>()->SetModel(modelY);
	//m_MoverY->GetComponent<IColliderComponent3D>()->SetBounds(model->GetBounds());

	m_MoverZ = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverZ->SetName("Mover_Z");
	m_MoverZ->SetTranslate(glm::vec3(0.0f, 0.0f, 1.0f));
	m_MoverZ->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
	m_MoverZ->GetComponent<IModelsComponent3D>()->SetModel(modelZ);
	//m_MoverZ->GetComponent<IColliderComponent3D>()->SetBounds(model->GetBounds());
}

bool CEditorToolMover::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto movingNode = GetMovingNode();
	if (movingNode == nullptr)
	{
		Clear();
		return false;
	}

	_ASSERT(false == IsChildOf(m_MoverRoot, movingNode));

	{
		auto pos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), movingNode->GetTranslation().y));
		auto cameraPosX0Z = GetScene()->GetCameraController()->GetCamera()->GetTranslation();
		cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
		auto movedObjectPosX0Z = glm::vec3(movingNode->GetTranslation().x, 0.0f, movingNode->GetTranslation().z);
		auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);
		auto posYYY = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
		m_MoverOffset = m_MoverRoot->GetTranslation() - glm::vec3(pos.x, posYYY.y, pos.z);
	}

	auto nodes = GetScene()->GetFinder().FindIntersection(RayToWorld, nullptr, m_MoverRoot);
	if (nodes.empty())
		return false;

	for (const auto& it : nodes)
	{
		if (it.second == m_MoverX)
			m_MoverNumber = EMoverDirection::X;
		else if (it.second == m_MoverY)
			m_MoverNumber = EMoverDirection::Y;
		else if (it.second == m_MoverZ)
			m_MoverNumber = EMoverDirection::Z;
	}

	if (m_MoverNumber != EMoverDirection::None)
		return true;

	return false;
}

void CEditorToolMover::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_MoverNumber == EMoverDirection::None)
		return;

	Clear();
}

void CEditorToolMover::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (m_MoverNumber == EMoverDirection::None)
		return;

	auto movingNode = GetMovingNode();
	if (movingNode == nullptr)
	{
		Clear();
		return;
	}

	glm::vec3 oldPos = movingNode->GetTranslation();
	glm::vec3 newPos = oldPos;

	if (m_MoverNumber == EMoverDirection::X)
	{
		auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
		newPos = glm::vec3(mousePos.x + m_MoverOffset.x, oldPos.y, oldPos.z);
	}
	else if (m_MoverNumber == EMoverDirection::Z)
	{
		auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
		newPos = glm::vec3(oldPos.x, oldPos.y, mousePos.z + m_MoverOffset.z);
	}
	else if (m_MoverNumber == EMoverDirection::Y)
	{
		auto cameraPosX0Z = GetScene()->GetCameraController()->GetCamera()->GetTranslation();
		cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
		auto movedObjectPosX0Z = glm::vec3(oldPos.x, 0.0f, oldPos.z);
		auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);

		auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
		newPos = glm::vec3(oldPos.x, mousePos.y + m_MoverOffset.y, oldPos.z);
	}

	movingNode->SetTranslate(FixBoxCoords(newPos));
	m_MoverRoot->SetTranslate(movingNode->GetTranslation());

	// Refresh selection bounds
	GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(movingNode);
}



//
// IEditorToolUI
//
void CEditorToolMover::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
	m_MoverValues.insert(std::make_pair("<disabled>", 0.001f));
	m_MoverValues.insert(std::make_pair("x0.25", 0.25f));
	m_MoverValues.insert(std::make_pair("x0.5", 0.5f));
	m_MoverValues.insert(std::make_pair("x1.0", 1.0f));
	m_MoverValues.insert(std::make_pair("x5.0", 5.0f));
	m_MoverValues.insert(std::make_pair("x10.0", 10.0f));

	// Add items to Combo Box
	for (const auto& v : m_MoverValues)
		QtUIFrame.getUI().MoverStepComboBox->addItem(v.first.c_str());

	// Select default item
	int index = QtUIFrame.getUI().MoverStepComboBox->findText("x5.0");
	//int index = QtUIFrame.getUI().Editor3DFrame_MoverStep->findData("x5.0");
	if (index != -1)
		QtUIFrame.getUI().MoverStepComboBox->setCurrentIndex(index);
	else
		_ASSERT(false);

	QtUIFrame.getQObject().connect(QtUIFrame.getUI().MoverStepComboBox, qOverload<const QString&>(&QComboBox::currentIndexChanged), [this](const QString& String) {
		auto it = m_MoverValues.find(String.toStdString());
		if (it == m_MoverValues.end())
			_ASSERT(false);

		SetMoverValue(it->second);
	});



	QtUIFrame.getQObject().connect(QtUIFrame.getUI().editorToolMoverBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolMover);
	});
}



//
// IEditorToolMover
//
glm::vec3 CEditorToolMover::FixBoxCoords(const glm::vec3 & Position)
{
	glm::vec3 newPosition = Position;
	newPosition /= m_MoverValue;
	newPosition = glm::round(newPosition);
	newPosition *= m_MoverValue;
	return newPosition;
}

void CEditorToolMover::SetMoverValue(float Value)
{
	m_MoverValue = Value;
}

float CEditorToolMover::GetMoverValue() const
{
	return m_MoverValue; 
}



//
// Protected
// 
void CEditorToolMover::Clear()
{
	m_MoverNumber = EMoverDirection::None;
}

std::shared_ptr<ISceneNode3D> CEditorToolMover::GetMovingNode()
{
	return m_MovingNode.lock();
}
