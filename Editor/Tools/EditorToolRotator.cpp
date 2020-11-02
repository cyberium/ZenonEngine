#include "stdafx.h"

// General
#include "EditorToolRotator.h"

// Additional
#include "Materials/MaterialEditorTool.h"

CEditorToolRotator::CEditorToolRotator(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_RotatorNumber(EMoverDirection::None)
	, m_RotatorValue(45.0f)
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
		m_RotatorRoot->SetScale(glm::vec3(node->GetComponent<IColliderComponent3D>()->GetBounds().getRadius()));
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

void CEditorToolRotator::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_RotatorRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), GetScene()->GetRootNode3D());
	m_RotatorRoot->SetName("RotatorRoot");


	auto materialSphere = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialSphere->SetColor(glm::vec4(0.8f, 0.8f, 0.8f, 0.5f));
	auto modelSphere = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelSphere->AddConnection(materialSphere, GetRenderDevice().GetPrimitivesFactory().CreateSphere(2.0f));

	auto rotatorSphere = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	rotatorSphere->SetName("RotatorSphere");
	rotatorSphere->GetComponent<IModelsComponent3D>()->SetModel(modelSphere);
	rotatorSphere->GetComponent<IColliderComponent3D>()->SetBounds(modelSphere->GetBounds());



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
	m_RotatorX->SetName("RotatorX");
	m_RotatorX->SetRotation(glm::vec3(0.0f, 0.0f, glm::half_pi<float>()));
	m_RotatorX->GetComponent<IModelsComponent3D>()->SetModel(modelX);
	m_RotatorX->GetComponent<IColliderComponent3D>()->SetBounds(geom->GetBounds());

	m_RotatorY = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorY->SetName("RotatorY");
	m_RotatorY->GetComponent<IModelsComponent3D>()->SetModel(modelY);
	m_RotatorY->GetComponent<IColliderComponent3D>()->SetBounds(geom->GetBounds());

	m_RotatorZ = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_RotatorRoot);
	m_RotatorZ->SetName("RotatorZ");
	m_RotatorZ->SetRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
	m_RotatorZ->GetComponent<IModelsComponent3D>()->SetModel(modelZ);
	m_RotatorZ->GetComponent<IColliderComponent3D>()->SetBounds(geom->GetBounds());
}

bool CEditorToolRotator::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto rotatingNode = GetRotatingNode();
	if (rotatingNode == nullptr)
		return false;

	_ASSERT(false == IsChildOf(m_RotatorRoot, rotatingNode));

	auto nodes = GetScene()->GetFinder().FindIntersection(RayToWorld, nullptr, m_RotatorRoot);
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
	m_RotatorsValues.insert(std::make_pair("<disabled>", 0.001f));
	m_RotatorsValues.insert(std::make_pair("5 deg", 5.0f));
	m_RotatorsValues.insert(std::make_pair("10 deg", 10.0f));
	m_RotatorsValues.insert(std::make_pair("15 deg", 15.0f));
	m_RotatorsValues.insert(std::make_pair("30 deg", 30.0f));
	m_RotatorsValues.insert(std::make_pair("45 deg", 45.0f));
	

	// Add items to Combo Box
	for (const auto& v : m_RotatorsValues)
		QtUIFrame.getUI().RotatorStepComboBox->addItem(v.first.c_str());

	// Select default item
	int index = QtUIFrame.getUI().RotatorStepComboBox->findText("45 deg");
	//int index = QtUIFrame.getUI().Editor3DFrame_MoverStep->findData("x5.0");
	if (index != -1)
		QtUIFrame.getUI().RotatorStepComboBox->setCurrentIndex(index);
	else
		_ASSERT(false);

	QtUIFrame.getQObject().connect(QtUIFrame.getUI().RotatorStepComboBox, qOverload<const QString&>(&QComboBox::currentIndexChanged), [this](const QString& String) {
		auto it = m_RotatorsValues.find(String.toStdString());
		if (it == m_RotatorsValues.end())
			_ASSERT(false);

		SetRotatorValue(it->second);
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
	float rotatorInitialAngleDegrees = Angle;
	rotatorInitialAngleDegrees /= m_RotatorValue;
	rotatorInitialAngleDegrees = glm::round(rotatorInitialAngleDegrees);
	rotatorInitialAngleDegrees *= m_RotatorValue;
	return rotatorInitialAngleDegrees;
}

void CEditorToolRotator::SetRotatorValue(float Value)
{
	m_RotatorValue = Value;
}

float CEditorToolRotator::GetRotatorValue() const
{
	return m_RotatorValue;
}



void CEditorToolRotator::Clear()
{
	m_RotatorNumber = EMoverDirection::None;
}

std::shared_ptr<ISceneNode3D> CEditorToolRotator::GetRotatingNode()
{
	return m_RotatingNode.lock();
}
