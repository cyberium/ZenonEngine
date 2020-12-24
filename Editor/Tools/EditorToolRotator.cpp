#include "stdafx.h"

// General
#include "EditorToolRotator.h"

// Additional
#include "Editor3D/EditorMaterials/MaterialEditorTool.h"

CEditorToolRotator::CEditorToolRotator(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_RotatorNumber(EMoverDirection::None)
	, m_InitialRotationDegrees(0.0f)
	, m_StartMousePosY(0)
{
}

CEditorToolRotator::~CEditorToolRotator()
{
}

void CEditorToolRotator::Enable()
{
	CEditorToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorBtn->setChecked(IsEnabled());

	if (auto node = GetEditor().GetFirstSelectedNode())
		OnNodeSelected(node);
}

void CEditorToolRotator::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorBtn->setChecked(IsEnabled());

	Clear();
	m_RotatingNode.reset();
	m_RotatorRoot->SetPosition(glm::vec3(Math::MinFloat));
}

void CEditorToolRotator::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget)
{
	m_RotatorRoot = GetScene().CreateSceneNodeT<ISceneNode>();
	m_RotatorRoot->SetName("RotatorRoot");

	{
		auto materialSphere = MakeShared(MaterialEditorTool, GetRenderDevice());
		materialSphere->SetColor(glm::vec4(0.8f, 0.8f, 0.8f, 0.5f));

		auto geometrySphere = GetRenderDevice().GetPrimitivesFactory().CreateSphere(2.0f);

		auto modelSphere = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelSphere->AddConnection(materialSphere, geometrySphere);

		auto rotatorSphere = GetScene().CreateSceneNodeT<ISceneNode>(m_RotatorRoot);
		rotatorSphere->SetName("RotatorSphere");
		rotatorSphere->GetComponentT<IModelComponent>()->SetModel(modelSphere);
	}


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

	m_RotatorX = GetScene().CreateSceneNodeT<ISceneNode>(m_RotatorRoot);
	m_RotatorX->SetName("RotatorX");
	m_RotatorX->SetLocalRotationEuler(glm::vec3(0.0f, 0.0f, 90.0f));
	m_RotatorX->GetComponentT<IModelComponent>()->SetModel(modelX);

	m_RotatorY = GetScene().CreateSceneNodeT<ISceneNode>(m_RotatorRoot);
	m_RotatorY->SetName("RotatorY");
	m_RotatorY->GetComponentT<IModelComponent>()->SetModel(modelY);

	m_RotatorZ = GetScene().CreateSceneNodeT<ISceneNode>(m_RotatorRoot);
	m_RotatorZ->SetName("RotatorZ");
	m_RotatorZ->SetLocalRotationEuler(glm::vec3(90.0f, 0.0f, 0.0f));
	m_RotatorZ->GetComponentT<IModelComponent>()->SetModel(modelZ);
}

bool CEditorToolRotator::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
		return false;

	_ASSERT(false == IsChildOf(m_RotatorRoot, rotatingNode));

	auto nodes = GetScene().GetFinder().FindIntersection(RayToWorld, nullptr, m_RotatorRoot);
	if (nodes.empty())
		return false;

	for (const auto& it : nodes)
	{
		if (it.second == m_RotatorX)
			m_RotatorNumber = EMoverDirection::X;
		else if (it.second == m_RotatorY)
			m_RotatorNumber = EMoverDirection::Y;
		else if (it.second == m_RotatorZ)
			m_RotatorNumber = EMoverDirection::Z;
	}

	if (m_RotatorNumber != EMoverDirection::None)
	{
		m_InitialRotationDegrees = rotatingNode->GetLocalRotationEuler();
		m_StartMousePosY = e.Y;

		return true;
	}

	return false;
}

void CEditorToolRotator::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_RotatorNumber == EMoverDirection::None)
		return;
	Clear();
}

void CEditorToolRotator::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
		return;

	if (m_RotatorNumber == EMoverDirection::X)
	{
		float rotatorInitialAngleDegreesX = m_InitialRotationDegrees.x + float(m_StartMousePosY - e.Y) / glm::pi<float>();
		rotatorInitialAngleDegreesX = GetEditor().GetUIFrame().FixRotatorCoords(rotatorInitialAngleDegreesX);
		rotatingNode->SetLocalRotationEuler(glm::vec3(rotatorInitialAngleDegreesX, m_InitialRotationDegrees.y, m_InitialRotationDegrees.z));
	}
	else if (m_RotatorNumber == EMoverDirection::Y)
	{
		float rotatorInitialAngleDegreesY = m_InitialRotationDegrees.y + float(m_StartMousePosY - e.Y) / glm::pi<float>();
		rotatorInitialAngleDegreesY = GetEditor().GetUIFrame().FixRotatorCoords(rotatorInitialAngleDegreesY);
		rotatingNode->SetLocalRotationEuler(glm::vec3(m_InitialRotationDegrees.x, rotatorInitialAngleDegreesY, m_InitialRotationDegrees.z));
	}
	else if (m_RotatorNumber == EMoverDirection::Z)
	{
		float rotatorInitialAngleDegreesZ = m_InitialRotationDegrees.z + float(m_StartMousePosY - e.Y) / glm::pi<float>();
		rotatorInitialAngleDegreesZ = GetEditor().GetUIFrame().FixRotatorCoords(rotatorInitialAngleDegreesZ);
		rotatingNode->SetLocalRotationEuler(glm::vec3(m_InitialRotationDegrees.x, m_InitialRotationDegrees.y, rotatorInitialAngleDegreesZ));
	}
	else
		return;

	m_RotatorRoot->SetRotationQuaternion(rotatingNode->GetRotationQuaternion());

	// Refresh selection bounds
	dynamic_cast<IEditorToolSelector&>(GetEditor().GetTools().GetTool(ETool::EToolSelector)).SelectNode(rotatingNode);
}

void CEditorToolRotator::OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode)
{
	m_RotatingNode = SelectedNode;
	m_RotatorRoot->SetPosition(SelectedNode->GetPosition());
	m_RotatorRoot->SetRotationQuaternion(SelectedNode->GetRotationQuaternion());
	m_RotatorRoot->SetScale(glm::vec3(SelectedNode->GetComponentT<IColliderComponent>()->GetBounds().getRadius()));
}



//
// IEditorToolUI
//
void CEditorToolRotator::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
	std::unordered_map<std::string, float> rotatorsValues;
	rotatorsValues.insert(std::make_pair("<disabled>", 0.01f));
	rotatorsValues.insert(std::make_pair("5 deg", 5.0f));
	rotatorsValues.insert(std::make_pair("10 deg", 10.0f));
	rotatorsValues.insert(std::make_pair("15 deg", 15.0f));
	rotatorsValues.insert(std::make_pair("30 deg", 30.0f));
	rotatorsValues.insert(std::make_pair("45 deg", 45.0f));

	GetEditor().GetUIFrame().SetRotatorValue(45.0f);

	QComboBox * comboBox = QtUIFrame.getUI().RotatorStepComboBox;

	// Add items to Combo Box
	for (const auto& v : rotatorsValues)
		comboBox->addItem(v.first.c_str(), v.second);

	// Select default item
	int index = comboBox->findData(QVariant(GetEditor().GetUIFrame().GetRotatorValue()));
	if (index != -1)
		comboBox->setCurrentIndex(index);
	else
		_ASSERT(false);

	QtUIFrame.getQObject().connect(comboBox, qOverload<const QString&>(&QComboBox::currentIndexChanged), [this, comboBox](const QString& String) {
		int index = comboBox->findText(String);
		if (index != -1)
			GetEditor().GetUIFrame().SetRotatorValue(comboBox->itemData(index).toFloat());
		else
			_ASSERT(false);
	});


	QtUIFrame.getQObject().connect(QtUIFrame.getUI().editorToolRotatorBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolRotator);
	});
}



//
// Protected
//
void CEditorToolRotator::Clear()
{
	m_RotatorNumber = EMoverDirection::None;
}

std::shared_ptr<ISceneNode> CEditorToolRotator::GetRotatingNode()
{
	return m_RotatingNode.lock();
}
