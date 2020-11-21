#include "stdafx.h"

// General
#include "EditorToolRotator.h"

// Additional
#include "Materials/MaterialEditorTool.h"

CEditorToolRotator::CEditorToolRotator(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_RotatorNumber(EMoverDirection::None)
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
	{
		m_RotatingNode = node;
		m_RotatorRoot->SetTranslate(node->GetTranslation());
		m_RotatorRoot->SetScale(glm::vec3(node->GetComponentT<IColliderComponent3D>()->GetBounds().getRadius()));
	}
}

void CEditorToolRotator::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolRotatorBtn->setChecked(IsEnabled());

	Clear();
	m_RotatingNode.reset();
	m_RotatorRoot->SetTranslate(glm::vec3(Math::MinFloat));
}

void CEditorToolRotator::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget)
{
	m_RotatorRoot = GetScene().CreateSceneNodeT<ISceneNode>();
	m_RotatorRoot->SetName("RotatorRoot");

	{
		auto materialSphere = MakeShared(MaterialEditorTool, GetRenderDevice());
		materialSphere->SetColor(glm::vec4(0.8f, 0.8f, 0.8f, 0.5f));
		auto modelSphere = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelSphere->AddConnection(materialSphere, GetRenderDevice().GetPrimitivesFactory().CreateSphere(2.0f));

		auto rotatorSphere = GetScene().CreateSceneNodeT<ISceneNode>(m_RotatorRoot);
		rotatorSphere->SetName("RotatorSphere");
		rotatorSphere->GetComponentT<IModelsComponent3D>()->SetModel(modelSphere);
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
	m_RotatorX->SetRotation(glm::vec3(0.0f, 0.0f, glm::half_pi<float>()));
	m_RotatorX->GetComponentT<IModelsComponent3D>()->SetModel(modelX);

	m_RotatorY = GetScene().CreateSceneNodeT<ISceneNode>(m_RotatorRoot);
	m_RotatorY->SetName("RotatorY");
	m_RotatorY->GetComponentT<IModelsComponent3D>()->SetModel(modelY);

	m_RotatorZ = GetScene().CreateSceneNodeT<ISceneNode>(m_RotatorRoot);
	m_RotatorZ->SetName("RotatorZ");
	m_RotatorZ->SetRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
	m_RotatorZ->GetComponentT<IModelsComponent3D>()->SetModel(modelZ);
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
		return true;


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
	if (m_RotatorNumber == EMoverDirection::None)
		return;

	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
		return;

	glm::vec3 newRot = rotatingNode->GetRotation();

	if (m_RotatorNumber == EMoverDirection::X)
	{
		newRot.x += e.RelY / 360.0f;
	}
	else if (m_RotatorNumber == EMoverDirection::Y)
	{
		newRot.y += e.RelY / 360.0f;
	}
	else if (m_RotatorNumber == EMoverDirection::Z)
	{
		newRot.z += e.RelY / 360.0f;
	}

	rotatingNode->SetRotation(newRot);

	// Refresh selection bounds
	dynamic_cast<IEditorToolSelector&>(GetEditor().GetTools().GetTool(ETool::EToolSelector)).SelectNode(rotatingNode);
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

	m_RotatorValue = 45.0f;

	QComboBox * comboBox = QtUIFrame.getUI().RotatorStepComboBox;

	// Add items to Combo Box
	for (const auto& v : rotatorsValues)
		comboBox->addItem(v.first.c_str(), v.second);

	// Select default item
	int index = comboBox->findData(QVariant(m_RotatorValue));
	if (index != -1)
		comboBox->setCurrentIndex(index);
	else
		_ASSERT(false);

	QtUIFrame.getQObject().connect(comboBox, qOverload<const QString&>(&QComboBox::currentIndexChanged), [this, comboBox](const QString& String) {
		int index = comboBox->findText(String);
		if (index != -1)
			SetRotatorValue(comboBox->itemData(index).toFloat());
		else
			_ASSERT(false);
	});


	QtUIFrame.getQObject().connect(QtUIFrame.getUI().editorToolRotatorBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolRotator);
	});
}



//
// IEditorToolRotator
//
float CEditorToolRotator::FixAngle(float Angle)
{
	float rotatorInitialAngleDegrees = glm::degrees(Angle);
	rotatorInitialAngleDegrees /= m_RotatorValue;
	rotatorInitialAngleDegrees = glm::round(rotatorInitialAngleDegrees);
	rotatorInitialAngleDegrees *= m_RotatorValue;
	return glm::radians(rotatorInitialAngleDegrees);
}

void CEditorToolRotator::SetRotatorValue(float Value)
{
	m_RotatorValue = Value;
}

float CEditorToolRotator::GetRotatorValue() const
{
	return m_RotatorValue;
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
