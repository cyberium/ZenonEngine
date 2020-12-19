#include "stdafx.h"

// General
#include "EditorToolMoverRTS.h"

// Additional
#include "Editor3D/EditorMaterials/MaterialEditorTool.h"

CEditorToolMoverRTS::CEditorToolMoverRTS(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_MoverNumber(EMoverDirection::None)
{
}

CEditorToolMoverRTS::~CEditorToolMoverRTS()
{
}

void CEditorToolMoverRTS::Enable()
{
	CEditorToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolMoverRTSBtn->setChecked(IsEnabled());

	if (auto node = GetEditor().GetFirstSelectedNode())
	{
		m_MovingNode = node;
		m_MoverRoot->SetPosition(node->GetPosition());
		m_MoverRoot->SetScale(glm::vec3(node->GetComponentT<IColliderComponent>()->GetBounds().getRadius() * 1.0f / 50.0f));
	}
}

void CEditorToolMoverRTS::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolMoverRTSBtn->setChecked(IsEnabled());

	Clear();

	m_MovingNode.reset();
	m_MoverRoot->SetPosition(glm::vec3(Math::MinFloat));
}

void CEditorToolMoverRTS::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget)
{
	m_MoverRoot = GetScene().CreateSceneNodeT<ISceneNode>();
	m_MoverRoot->SetName("MoverRTSRoot");
	/*
	auto model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("arrow.FBX");
	auto geom = model->GetConnections().begin()->Geometry;

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


	m_MoverX = GetScene().CreateSceneNodeT<ISceneNode>(m_MoverRoot);
	m_MoverX->SetName("MoverRTSX");
	m_MoverX->SetRotationEuler(glm::vec3(0.0f, glm::half_pi<float>(), 0.0f));
	m_MoverX->GetComponentT<IModelComponent>()->SetModel(modelX);

	m_MoverZ = GetScene().CreateSceneNodeT<ISceneNode>(m_MoverRoot);
	m_MoverZ->SetName("RotatorRTSZ");
	m_MoverZ->SetRotationEuler(glm::vec3(0.0f, 0.0f, 0.0f));
	m_MoverZ->GetComponentT<IModelComponent>()->SetModel(modelZ);
	*/
}

bool CEditorToolMoverRTS::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto movingNode = GetMovingNode();
	if (movingNode == nullptr)
		return false;

	_ASSERT(false == IsChildOf(m_MoverRoot, movingNode));

	auto nodes = GetScene().GetFinder().FindIntersection(RayToWorld, nullptr, m_MoverRoot);
	if (nodes.empty())
		return false;

	{
		auto pos = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), movingNode->GetPosition().y));
		auto cameraPosX0Z = GetScene().GetCameraController()->GetCamera()->GetTranslation();
		cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
		auto movedObjectPosX0Z = glm::vec3(movingNode->GetPosition().x, 0.0f, movingNode->GetPosition().z);
		auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);
		auto posYYY = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
		m_MoverOffset = m_MoverRoot->GetPosition() - glm::vec3(pos.x, posYYY.y, pos.z);
	}

	for (const auto& it : nodes)
	{
		if (it.second == m_MoverX)
		{
			m_MoverNumber = EMoverDirection::X;
		}
		else if (it.second == m_MoverZ)
		{
			m_MoverNumber = EMoverDirection::Z;
		}
	}

	if (m_MoverNumber != EMoverDirection::None)
		return true;

	return false;
}

void CEditorToolMoverRTS::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_MoverNumber == EMoverDirection::None)
		return;

	Clear();
}

void CEditorToolMoverRTS::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (m_MoverNumber == EMoverDirection::None)
		return;

	auto movingNode = GetMovingNode();
	if (movingNode == nullptr)
		return;

	glm::vec3 oldPos = movingNode->GetPosition();
	glm::vec3 newPos = glm::vec3(0.0f);

	auto mousePos = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
	if (m_MoverNumber == EMoverDirection::X)
	{
		newPos = glm::vec3(mousePos.x + m_MoverOffset.x, oldPos.y, oldPos.z);
	}
	else if (m_MoverNumber == EMoverDirection::Z)
	{
		newPos = glm::vec3(oldPos.x, oldPos.y, mousePos.z + m_MoverOffset.z);
	}

	glm::vec3 fixed3DTranslate = GetEditor().GetTools().GetToolT<IEditorToolMover>(ETool::EToolMover).FixBoxCoords(newPos);

	glm::vec2 fixed2DTranslate = glm::vec2(fixed3DTranslate.xz);
	movingNode->SetPosition(glm::vec3(fixed2DTranslate.x, oldPos.y, fixed2DTranslate.y));
	m_MoverRoot->SetPosition(movingNode->GetPosition());

	// Refresh selection bounds
	GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(movingNode);
}



//
// IEditorTool
//
void CEditorToolMoverRTS::DoInitializeUI(IEditorQtUIFrame & QtUIFrame)
{
	auto btn = QtUIFrame.getUI().editorToolMoverRTSBtn;

	btn->connect(btn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolMoverRTS);
	});
}



//
// Protected
//
void CEditorToolMoverRTS::Clear()
{
	m_MoverNumber = EMoverDirection::None;
}

std::shared_ptr<ISceneNode> CEditorToolMoverRTS::GetMovingNode()
{
	return m_MovingNode.lock();
}
