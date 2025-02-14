#include "stdafx.h"

// General
#include "EditorToolMover.h"

// Additional
#include "Editor3D/EditorMaterials/MaterialEditorTool.h"

CEditorToolMover::CEditorToolMover(IEditor& Editor)
	: CEditorToolBase(Editor)
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
		OnNodeSelected(node);
}

void CEditorToolMover::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolMoverBtn->setChecked(IsEnabled());

	Clear();
	m_MovingNode.reset();
	m_MoverRoot->SetPosition(glm::vec3(Math::MinFloat));
}

void CEditorToolMover::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget)
{
	m_MoverRoot = GetScene().CreateSceneNodeT<ISceneNode>();
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

	m_MoverX = GetScene().CreateSceneNodeT<ISceneNode>(m_MoverRoot); 
	m_MoverX->SetName("Mover_X");
	m_MoverX->SetLocalPosition(glm::vec3(1.0f, 0.0f, 0.0f));
	m_MoverX->SetLocalRotationEuler(glm::vec3(0.0f, 90.0f, 0.0f));
	m_MoverX->GetComponentT<IModelComponent>()->SetModel(modelX);

	m_MoverY = GetScene().CreateSceneNodeT<ISceneNode>(m_MoverRoot);
	m_MoverY->SetName("Mover_Y");
	m_MoverY->SetLocalPosition(glm::vec3(0.0f, 1.0f, 0.0f));
	m_MoverY->SetLocalRotationEuler(glm::vec3(- 90.0f, 0.0f, 0.0f));
	m_MoverY->GetComponentT<IModelComponent>()->SetModel(modelY);

	m_MoverZ = GetScene().CreateSceneNodeT<ISceneNode>(m_MoverRoot);
	m_MoverZ->SetName("Mover_Z");
	m_MoverZ->SetLocalPosition(glm::vec3(0.0f, 0.0f, 1.0f));
	m_MoverZ->SetLocalRotationEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	m_MoverZ->GetComponentT<IModelComponent>()->SetModel(modelZ);
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
		auto pos = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), movingNode->GetPosition().y));
		auto cameraPosX0Z = GetScene().GetCameraController()->GetCamera()->GetPosition();
		cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
		auto movedObjectPosX0Z = glm::vec3(movingNode->GetPosition().x, 0.0f, movingNode->GetPosition().z);
		auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);
		auto posYYY = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
		m_MoverOffset = m_MoverRoot->GetPosition() - glm::vec3(pos.x, posYYY.y, pos.z);
	}

	auto nodes = GetScene().GetFinder().FindIntersection(RayToWorld, nullptr, m_MoverRoot);
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

	glm::vec3 oldPos = movingNode->GetPosition();
	glm::vec3 newPos = oldPos;

	if (m_MoverNumber == EMoverDirection::X)
	{
		auto mousePos = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
		newPos = glm::vec3(mousePos.x + m_MoverOffset.x, oldPos.y, oldPos.z);
	}
	else if (m_MoverNumber == EMoverDirection::Z)
	{
		auto mousePos = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
		newPos = glm::vec3(oldPos.x, oldPos.y, mousePos.z + m_MoverOffset.z);
	}
	else if (m_MoverNumber == EMoverDirection::Y)
	{
		auto cameraPosX0Z = GetScene().GetCameraController()->GetCamera()->GetPosition();
		cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
		auto movedObjectPosX0Z = glm::vec3(oldPos.x, 0.0f, oldPos.z);
		auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);

		auto mousePos = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
		newPos = glm::vec3(oldPos.x, mousePos.y + m_MoverOffset.y, oldPos.z);
	}

	movingNode->SetPosition(GetEditor().GetUIFrame().FixMoverCoords(newPos));
	m_MoverRoot->SetPosition(movingNode->GetPosition());

	// Refresh selection bounds
	GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(movingNode);
}

void CEditorToolMover::OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode)
{
	m_MovingNode = SelectedNode;
	m_MoverRoot->SetPosition(SelectedNode->GetPosition());

	glm::vec3 scale(1.0f);
	if (auto collider = SelectedNode->GetComponentT<IColliderComponent>())
	{
		const auto& worldBounds = collider->GetWorldBounds();
		if (false == worldBounds.IsInfinite())
			scale = glm::vec3(worldBounds.getRadius());
	}

	m_MoverRoot->SetScale(scale / 50.0f);

	//m_MoverRoot->SetScale(glm::vec3(SelectedNode->GetComponentT<IColliderComponent>()->GetBounds().getRadius()) / 50.0f);
}



//
// IEditorToolUI
//
void CEditorToolMover::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
	std::unordered_map<std::string, float> moverValues;
	moverValues.insert(std::make_pair("<disabled>", 0.001f));
	moverValues.insert(std::make_pair("0.25 unit", 0.25f));
	moverValues.insert(std::make_pair("0.5 unit", 0.5f));
	moverValues.insert(std::make_pair("1.0 unit", 1.0f));
	moverValues.insert(std::make_pair("5.0 unit", 5.0f));
	moverValues.insert(std::make_pair("10.0 unit", 10.0f));

	GetEditor().GetUIFrame().SetMoverValue(5.0f);

	QComboBox * comboBox = QtUIFrame.getUI().MoverStepComboBox;

	// Add items to Combo Box
	for (const auto& v : moverValues)
		comboBox->addItem(v.first.c_str(), QVariant(v.second));

	// Select default item
	int index = QtUIFrame.getUI().MoverStepComboBox->findData(QVariant(GetEditor().GetUIFrame().GetMoverValue()));
	if (index != -1)
		comboBox->setCurrentIndex(index);
	else
		_ASSERT(false);

	QtUIFrame.getQObject().connect(comboBox, qOverload<const QString&>(&QComboBox::currentIndexChanged), [this, comboBox](const QString& String) {
		int index = comboBox->findText(String);
		if (index != -1)
			GetEditor().GetUIFrame().SetMoverValue(comboBox->itemData(index).toFloat());
		else
			_ASSERT(false);
	});



	QtUIFrame.getQObject().connect(QtUIFrame.getUI().editorToolMoverBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolMover);
	});
}



//
// Protected
// 
void CEditorToolMover::Clear()
{
	m_MoverNumber = EMoverDirection::None;
}

std::shared_ptr<ISceneNode> CEditorToolMover::GetMovingNode()
{
	return m_MovingNode.lock();
}
