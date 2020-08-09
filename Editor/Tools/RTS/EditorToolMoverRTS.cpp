#include "stdafx.h"

// General
#include "EditorToolMoverRTS.h"

CEditorToolMoverRTS::CEditorToolMoverRTS(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_MoverValue(1.0f)
	, m_MoverNuber(-1)
	, m_IsMovingNow(false)
{
}

CEditorToolMoverRTS::~CEditorToolMoverRTS()
{
	Log::Info("Deleted");
}

void CEditorToolMoverRTS::Initialize()
{
	m_MoverRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), GetScene()->GetRootNode3D());
	m_MoverRoot->SetName("MoverRTS");

	std::shared_ptr<IFBXScene> fbxScene = GetBaseManager().GetManager<IFBXManager>()->LoadFBX("C:\\_engine\\ZenonEngine_gamedata\\arrow.FBX");

	auto fbxModels = fbxScene->GetFBXModels();
	_ASSERT(fbxModels.size() == 1);
	auto model = (*fbxModels.begin())->GetModel();
	auto geom = model->GetConnections().begin()->Geometry;
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
	{
		std::shared_ptr<IFile> file = MakeShared(CFile, "C:\\_engine\\ZenonEngine_gamedata\\arrow.znmdl");
		loadable->Save(file);

		GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("PCEveryFileAccess")->SaveFile(file);
	}

	auto materialX = MakeShared(MaterialDebug, GetRenderDevice());
	materialX->SetDiffuseColor(glm::vec4(1.0f, 0.2f, 0.1f, 1.0f));
	auto modelX = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelX->AddConnection(materialX, geom);

	auto materialY = MakeShared(MaterialDebug, GetRenderDevice());
	materialY->SetDiffuseColor(glm::vec4(0.1f, 1.0f, 0.1f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

	auto materialZ = MakeShared(MaterialDebug, GetRenderDevice());
	materialZ->SetDiffuseColor(glm::vec4(0.1f, 0.2f, 1.0f, 1.0f));
	auto modelZ = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelZ->AddConnection(materialZ, geom);


	m_MoverX = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverX->SetName("MoverRTS_X");
	m_MoverX->SetRotation(glm::vec3(0.0f, 0.0f, -glm::half_pi<float>()));
	//moverX->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	m_MoverX->GetModelsComponent()->SetModel(modelX);
	m_MoverX->GetColliderComponent()->SetBounds(model->GetBounds());

	m_MoverZ = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene(), m_MoverRoot);
	m_MoverZ->SetName("RotatorRTS_Z");
	//moverZ->SetScale(glm::vec3(0.5f, 1.0f, 0.5f));
	m_MoverZ->SetRotation(glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
	m_MoverZ->GetModelsComponent()->SetModel(modelZ);
	m_MoverZ->GetColliderComponent()->SetBounds(model->GetBounds());
}

void CEditorToolMoverRTS::Finalize()
{
}

void CEditorToolMoverRTS::Enable()
{
	CEditorToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolMoverRTSBtn->setChecked(IsEnabled());

	if (auto node = GetEditor().GetFirstSelectedNode())
	{
		m_MovingNode = node;
		m_MoverRoot->SetTranslate(node->GetTranslation());
		m_MoverRoot->SetScale(glm::vec3(m_MovingNode->GetColliderComponent()->GetBounds().getRadius() * 1.0f / 10.0f));
	}
}

void CEditorToolMoverRTS::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolMoverRTSBtn->setChecked(IsEnabled());

	m_MoverRoot->SetTranslate(glm::vec3(-1000000.0, -10000000.0f, -10000000.0f));

	Clear();
	m_MovingNode.reset();
}

bool CEditorToolMoverRTS::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_MovingNode == nullptr)
		return false;

	if (IsChildOf(m_MoverRoot, m_MovingNode))
		return false;

	auto nodes = GetScene()->GetFinder().FindIntersection(RayToWorld, nullptr, m_MoverRoot);
	if (nodes.empty())
		return false;

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

void CEditorToolMoverRTS::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_IsMovingNow)
		Clear();
}

void CEditorToolMoverRTS::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (m_IsMovingNow)
	{
		glm::vec3 oldPos = m_MovingNode->GetTranslation();
		glm::vec3 newPos = glm::vec3(0.0f);

		auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), oldPos.y));
		if (m_MoverNuber == 1)
		{
			newPos = glm::vec3(mousePos.x + m_MoverOffset.x, oldPos.y, oldPos.z);
		}
		else if (m_MoverNuber == 3)
		{
			newPos = glm::vec3(oldPos.x, oldPos.y, mousePos.z + m_MoverOffset.z);
		}

		m_MovingNode->SetTranslate(FixBoxCoords(newPos));
		m_MoverRoot->SetTranslate(m_MovingNode->GetTranslation());

		return;
	}
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

	btn->setContextMenuPolicy(Qt::CustomContextMenu);
	btn->connect(btn, &QPushButton::customContextMenuRequested, [this, btn](const QPoint& Point) {
		// Add context menu for scene node viewer
		auto contextMenu = new QMenu(btn);
		contextMenu->setTitle("Some context menu title.");
		contextMenu->clear();

		QAction* nameAction = new QAction("Test", btn);
		nameAction->setEnabled(false);

		contextMenu->addAction(nameAction);
		contextMenu->addSeparator();

		contextMenu->popup(btn->mapToGlobal(Point));
	});
}

glm::ivec3 CEditorToolMoverRTS::ToBoxCoords(const glm::vec3 & Position)
{
	return glm::round(Position / m_MoverValue);
}

glm::vec3 CEditorToolMoverRTS::FixBoxCoords(const glm::vec3 & Position)
{
	glm::vec3 newPosition = Position;
	newPosition /= m_MoverValue;
	newPosition = glm::round(newPosition);
	newPosition *= m_MoverValue;
	return newPosition;
}

void CEditorToolMoverRTS::SetMoverValue(float Value)
{
	m_MoverValue = Value;
}

void CEditorToolMoverRTS::Clear()
{
	m_MoverNuber = 0;
	m_IsMovingNow = false;
	//m_MovingNode.reset();
	m_MovingObjectPos = glm::vec3(0.0f);
}
