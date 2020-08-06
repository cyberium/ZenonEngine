#include "stdafx.h"

// General
#include "EditorToolMover.h"

CEditorToolMover::CEditorToolMover(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_MoverValue(1.0f)
	, m_MoverNuber(-1)
	, m_IsMovingNow(false)
{
}

CEditorToolMover::~CEditorToolMover()
{
}

void CEditorToolMover::Initialize()
{
	m_MoverRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), GetScene()->GetRootNode3D());
	m_MoverRoot->SetName("Mover");

	std::shared_ptr<IFBXScene> fbxScene = GetBaseManager().GetManager<IFBXManager>()->LoadFBX("C:\\_engine\\ZenonEngine_gamedata\\arrow.FBX");

	auto fbxModels = fbxScene->GetFBXModels();
	_ASSERT(fbxModels.size() == 1);
	auto model = (*fbxModels.begin())->GetModel();
	auto geom = model->GetConnections().begin()->Geometry;
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
	{
		std::shared_ptr<IFile> file = std::make_shared<CFile>("C:\\_engine\\ZenonEngine_gamedata\\arrow.znmdl");
		loadable->Save(file);

		GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("PCEveryFileAccess")->SaveFile(file);
	}

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


	m_MoverX = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverX->SetName("Mover_X");
	m_MoverX->SetRotation(glm::vec3(0.0f, 0.0f, -glm::half_pi<float>()));
	//moverX->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	m_MoverX->GetModelsComponent()->SetModel(modelX);
	m_MoverX->GetColliderComponent()->SetBounds(model->GetBounds());

	m_MoverY = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverY->SetName("Mover_Y");
	//moverY->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	m_MoverY->GetModelsComponent()->SetModel(modelY);
	m_MoverY->GetColliderComponent()->SetBounds(model->GetBounds());

	m_MoverZ = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverZ->SetName("Mover_Z");
	//moverZ->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	m_MoverZ->SetRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
	m_MoverZ->GetModelsComponent()->SetModel(modelZ);
	m_MoverZ->GetColliderComponent()->SetBounds(model->GetBounds());
}

void CEditorToolMover::Finalize()
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
		m_MoverRoot->SetScale(glm::vec3(m_MovingNode->GetColliderComponent()->GetBounds().getRadius() * 1.0f / 10.0f));
	}
}

void CEditorToolMover::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolMoverBtn->setChecked(IsEnabled());

	m_MoverRoot->SetTranslate(glm::vec3(-1000000.0, -10000000.0f, -10000000.0f));

	Clear();
	m_MovingNode.reset();
}

bool CEditorToolMover::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto nodes = GetScene()->FindIntersection(RayToWorld, m_MoverRoot);
	if (nodes.empty())
		return false;

	
	if (m_MovingNode == nullptr)
		return false;

	_ASSERT(!IsChildOf(m_MoverRoot, m_MovingNode));

	{
		auto pos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), m_MovingNode->GetTranslation().y));
		auto cameraPosX0Z = GetScene()->GetCameraController()->GetCamera()->GetTranslation();
		cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
		auto movedObjectPosX0Z = glm::vec3(m_MovingNode->GetTranslation().x, 0.0f, m_MovingNode->GetTranslation().z);
		auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);
		auto posYYY = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
		m_MoverOffset = m_MoverRoot->GetTranslation() - glm::vec3(pos.x, posYYY.y, pos.z);
	}

	for (const auto& it : nodes)
	{
		if (it.second == m_MoverX)
		{
			m_MoverNuber = 1;
		}
		else if (it.second == m_MoverY)
		{
			m_MoverNuber = 2;
		}
		else if (it.second == m_MoverZ)
		{
			m_MoverNuber = 3;
		}
	}

	if (m_MoverNuber > 0)
	{
		m_MovingObjectPos = m_MovingNode->GetTranslation();
		m_IsMovingNow = true;
		return true;
	}

	return false;
}

void CEditorToolMover::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_IsMovingNow)
		Clear();
}

void CEditorToolMover::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (!m_IsMovingNow)
		return;

	glm::vec3 oldPos = m_MovingNode->GetTranslation();
	glm::vec3 newPos = glm::vec3(0.0f);

	if (m_MoverNuber == 1)
	{
		auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
		newPos = glm::vec3(mousePos.x + m_MoverOffset.x, oldPos.y, oldPos.z);
	}
	else if (m_MoverNuber == 3)
	{
		auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
		newPos = glm::vec3(oldPos.x, oldPos.y, mousePos.z + m_MoverOffset.z);
	}
	else if (m_MoverNuber == 2)
	{
		auto cameraPosX0Z = GetScene()->GetCameraController()->GetCamera()->GetTranslation();
		cameraPosX0Z = glm::vec3(cameraPosX0Z.x, 0.0f, cameraPosX0Z.z);
		auto movedObjectPosX0Z = glm::vec3(oldPos.x, 0.0f, oldPos.z);
		auto planeNormal = glm::normalize(movedObjectPosX0Z - cameraPosX0Z);

		auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(planeNormal, 0.0f));
		newPos = glm::vec3(oldPos.x, mousePos.y + m_MoverOffset.y, oldPos.z);
	}

	m_MovingNode->SetTranslate(FixBoxCoords(newPos));
	m_MoverRoot->SetTranslate(m_MovingNode->GetTranslation());

	// Refresh selection bounds
	dynamic_cast<IEditorToolSelector&>(GetEditor().GetTools().GetTool(ETool::EToolSelector)).SelectNode(m_MovingNode);
}



//
// IEditorToolUI
//
void CEditorToolMover::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
	m_MoverValues.insert(std::make_pair("<disabled>", 0.001f));
	m_MoverValues.insert(std::make_pair("x1.0", 1.0f));
	m_MoverValues.insert(std::make_pair("x5.0", 5.0f));
	m_MoverValues.insert(std::make_pair("x10.0", 10.0f));

	// Add items to Combo Box
	for (const auto& v : m_MoverValues)
		QtUIFrame.getUI().Editor3DFrame_MoverStep->addItem(v.first.c_str());

	QtUIFrame.getQObject().connect(QtUIFrame.getUI().editorToolMoverBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolMover);
	});

	QtUIFrame.getQObject().connect(QtUIFrame.getUI().Editor3DFrame_MoverStep, qOverload<const QString&>(&QComboBox::currentIndexChanged), [this](const QString& String) {
		auto it = m_MoverValues.find(String.toStdString());
		if (it == m_MoverValues.end())
			_ASSERT(FALSE);

		SetMoverValue(it->second);
	});
}


glm::ivec3 CEditorToolMover::ToBoxCoords(const glm::vec3 & Position)
{
	return glm::round(Position / m_MoverValue);
}

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

void CEditorToolMover::Clear()
{
	m_MoverNuber = 0;
	m_IsMovingNow = false;
	m_MovingObjectPos = glm::vec3(0.0f);
}
